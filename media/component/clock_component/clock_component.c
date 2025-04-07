#define DBG_LEVEL		DBG_ERR

#include <base_component.h>
#include <unistd.h>

#include "clock_component.h"
#include "clock_port.h"

/* #define AV_SYNC_LOG */

#ifdef AV_SYNC_LOG
static FILE *fd;
#define FLOG(fmt, ...) fprintf(fd, fmt, ##__VA_ARGS__)
#else
#define FLOG(fmt, ...)
#endif

#define DEFAULT_THRESH 2000 /* TODO: what is a good thresh to use */

OMX_ERRORTYPE clock_get_config(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_INOUT OMX_PTR data)
{
	lb_omx_component_t *component = NULL;
	clock_component_private_t *cmp_priv = NULL;
	OMX_TIME_CONFIG_CLOCKSTATETYPE     *clockstate;
	OMX_TIME_CONFIG_TIMESTAMPTYPE      *timestamp;
	OMX_TIME_CONFIG_SCALETYPE           *config_scale;
	OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE  *refclk;
	OMX_S32                             scale;
	OMX_TICKS                           walltime;

	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	cmp_priv = (clock_component_private_t *)component->component_private;
	scale = cmp_priv->config_scale.xScale >> 16;

	switch (index) {
	case OMX_IndexConfigTimeClockState:
		clockstate = (OMX_TIME_CONFIG_CLOCKSTATETYPE *)data;
		memcpy(clockstate, &cmp_priv->clock_state, sizeof(*clockstate));
		break;
	case OMX_IndexConfigTimeCurrentWallTime:
		timestamp = (OMX_TIME_CONFIG_TIMESTAMPTYPE *)data;
		timestamp->nTimestamp = oscl_get_usec();
		break;
	case OMX_IndexConfigTimeCurrentMediaTime:
		timestamp = (OMX_TIME_CONFIG_TIMESTAMPTYPE *)data;
		if (component->state == OMX_StatePause) {
			timestamp->nTimestamp = cmp_priv->mediatime;
			break;
		}
		if (cmp_priv->ref_clock.eClock == OMX_TIME_RefClockNone) {
			walltime = oscl_get_usec();
			timestamp->nTimestamp = cmp_priv->mediatime_base +
				scale * (walltime - cmp_priv->walltime_base);
			cmp_priv->mediatime = timestamp->nTimestamp;
		} else
			timestamp->nTimestamp = cmp_priv->mediatime;
		break;
	case OMX_IndexConfigTimeScale:
		config_scale = (OMX_TIME_CONFIG_SCALETYPE *)data;
		memcpy(config_scale, &cmp_priv->config_scale, sizeof(*config_scale));
		break;
	case OMX_IndexConfigTimeActiveRefClock:
		refclk = (OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE *)data;
		memcpy(refclk, &cmp_priv->ref_clock, sizeof(*refclk));
		break;
	default:
		OSCL_LOGE("error omx config index %d\n", index);
		return OMX_ErrorBadParameter;
		break;
	}
	return OMX_ErrorNone;
}

static void config_mediatime_request(lb_omx_component_t *component,
	OMX_IN OMX_PTR data)
{
	clock_component_private_t *cmp_priv = NULL;
	clock_port_private_t *port_priv = NULL;
	OMX_TIME_CONFIG_MEDIATIMEREQUESTTYPE *mediatime_request;
	OMX_U32                             portIndex;
	OMX_TICKS walltime, mediatime, mediatime_diff, walltime_diff;
	OMX_S32                             scale;
	unsigned int                        sleeptime;

	cmp_priv = (clock_component_private_t *)component->component_private;
	scale = cmp_priv->config_scale.xScale >> 16;

	if (cmp_priv->clock_state.eState != OMX_TIME_ClockStateStopped &&
		scale != 0) { /* todo: what happens if call in pause mode */
		/* update the mediatime request */
		mediatime_request = (OMX_TIME_CONFIG_MEDIATIMEREQUESTTYPE *)data;
		portIndex = mediatime_request->nPortIndex;
		port_priv =
			(clock_port_private_t *)component->port[portIndex].port_private;
		memcpy(&port_priv->mediatime_request,
			mediatime_request, sizeof(*mediatime_request));

		/* cal the cur mediatime and diff from request */
		walltime = oscl_get_usec();
		mediatime = cmp_priv->mediatime_base +
			scale * (walltime - cmp_priv->walltime_base);
		mediatime_diff = mediatime_request->nMediaTimestamp -
			mediatime_request->nOffset * scale - mediatime;
		FLOG("portindex %ld wt %lld mt %lld mtdiff %lld, request %lld\n",
			portIndex, walltime, mediatime, mediatime_diff,
			mediatime_request->nMediaTimestamp);
		if ((mediatime_diff < 0 && scale > 0) ||
			(mediatime_diff > 0 && scale < 0)) {
			/* if mediatime has already elapsed
			 * then request can not be fullfilled */
			port_priv->mediatime.eUpdateType =
				OMX_TIME_UpdateRequestFulfillment;
			port_priv->mediatime.nMediaTimestamp =
				mediatime_request->nMediaTimestamp;
			port_priv->mediatime.nOffset = -1;
		} else {
			walltime_diff = mediatime_diff / scale;
			if (mediatime_diff) {
				if (walltime_diff > DEFAULT_THRESH) {
					sleeptime = (unsigned int)(walltime_diff -
							DEFAULT_THRESH)/1000;
					oscl_mdelay(sleeptime);
				/* todo: make sure if we can use this as new diff */
					walltime_diff = DEFAULT_THRESH;
					walltime = oscl_get_usec();
					mediatime = cmp_priv->mediatime_base +
						scale * (walltime -
							cmp_priv->walltime_base);
				}
				port_priv->mediatime.nMediaTimestamp =
					mediatime_request->nMediaTimestamp;
				port_priv->mediatime.nWallTimeAtMediaTime =
					walltime + walltime_diff;
				port_priv->mediatime.nOffset = walltime_diff;
				port_priv->mediatime.xScale =
					cmp_priv->config_scale.xScale;
				port_priv->mediatime.eUpdateType =
					OMX_TIME_UpdateRequestFulfillment;

			}
		}
		/*Signal Buffer Management Thread*/
		sem_post(&cmp_priv->sem_clockevent);
		OSCL_LOGD("Waiting for fullfillment handled\n");
		sem_wait(&cmp_priv->sem_clkevent_complete);
	}
}

OMX_ERRORTYPE clock_set_config(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_IN OMX_PTR data)
{
	lb_omx_component_t *component = NULL;
	clock_component_private_t *cmp_priv = NULL;
	clock_port_private_t *port_priv = NULL;
	OMX_TIME_CONFIG_CLOCKSTATETYPE      *clockstate;
	OMX_TIME_CONFIG_TIMESTAMPTYPE       *ref_timestamp;
	OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE  *ref_clock;
	OMX_U32                             portIndex;
	base_port_t                         *port;
	OMX_TIME_CONFIG_SCALETYPE           *config_scale;
	OMX_U32                             mask;
	int                                 i;
	OMX_TICKS                           walltime;
	/* OMX_S32                             scale; */

	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);
	cmp_priv = component->component_private;

	switch (index) {
	case OMX_IndexConfigTimeClockState:
		clockstate = (OMX_TIME_CONFIG_CLOCKSTATETYPE *)data;
		switch (clockstate->eState) {
		case OMX_TIME_ClockStateRunning:
			if (cmp_priv->clock_state.eState == OMX_TIME_ClockStateRunning)
				OSCL_LOGI("receive OMX_TIME_ClockStateRunning again\n");
			memcpy(&cmp_priv->clock_state, clockstate, sizeof(*clockstate));
			cmp_priv->update_type = OMX_TIME_UpdateClockStateChanged;
			/* update state change in all port */
			for (i = 0; i < component->num_ports; i++) {
				port_priv = (clock_port_private_t *)
						component->port[i].port_private;
				port_priv->mediatime.eUpdateType =
					OMX_TIME_UpdateClockStateChanged;
				port_priv->mediatime.eState = OMX_TIME_ClockStateRunning;
				port_priv->mediatime.xScale =
					cmp_priv->config_scale.xScale;
			}
			/* Signal Buffer Management Thread */
			sem_post(&cmp_priv->sem_clockevent);
			OSCL_LOGD("Waiting for Clock Running Event for all ports\n");
			sem_wait(&cmp_priv->sem_clkevent_complete);
			break;
		case OMX_TIME_ClockStateWaitingForStartTime:
			if (cmp_priv->clock_state.eState == OMX_TIME_ClockStateRunning) {
				OSCL_LOGE("incorrect state transition\n");
				return OMX_ErrorIncorrectStateTransition;
			} else if (cmp_priv->clock_state.eState ==
				OMX_TIME_ClockStateWaitingForStartTime) {
				OSCL_LOGE("error same state\n");
				return OMX_ErrorSameState;
			}
			OSCL_LOGI("set to state waiting for starttime\n");
			memcpy(&cmp_priv->clock_state, clockstate, sizeof(*clockstate));
			cmp_priv->min_starttime.nTimestamp = -1;
			break;
		case OMX_TIME_ClockStateStopped:
			memcpy(&cmp_priv->clock_state, clockstate, sizeof(*clockstate));
			cmp_priv->update_type = OMX_TIME_UpdateClockStateChanged;
			/* update state change in all port */
			for (i = 0; i < component->num_ports; i++) {
				port_priv = (clock_port_private_t *)
					component->port[i].port_private;
				port_priv->mediatime.eUpdateType =
					OMX_TIME_UpdateClockStateChanged;
				port_priv->mediatime.eState = OMX_TIME_ClockStateStopped;
				port_priv->mediatime.xScale =
					cmp_priv->config_scale.xScale;
			}
			/* Signal Buffer Management Thread */
			sem_post(&cmp_priv->sem_clockevent);
			OSCL_LOGD("Waiting for Clock Running Event for all ports\n");
			sem_wait(&cmp_priv->sem_clkevent_complete);
			break;
		default:
			OSCL_LOGE("error clock state!\n");
			break;
		}
		break;
	case OMX_IndexConfigTimeClientStartTime:
		ref_timestamp = (OMX_TIME_CONFIG_TIMESTAMPTYPE *)data;
		portIndex = ref_timestamp->nPortIndex;
		if (portIndex > component->num_ports) {
			OSCL_LOGE("error port index %d\n", portIndex);
			return OMX_ErrorBadPortIndex;
		}
		port = &component->port[portIndex];
		if (!PORT_IS_ENABLED(port)) {
			OSCL_LOGE("port %d is disabled\n", portIndex);
			return OMX_ErrorBadParameter;
		}
		port_priv = (clock_port_private_t *)port->port_private;
		memcpy(&port_priv->timestamp, ref_timestamp, sizeof(*ref_timestamp));
		/* it's the first client to send the start time,
		 * so init the min_starttime */
		if (cmp_priv->min_starttime.nTimestamp == -1)
			cmp_priv->min_starttime.nTimestamp =
				port_priv->timestamp.nTimestamp;

		/* update the nWaitMask to clear the flag for the client
		 * which has sent its start time */
		if (cmp_priv->clock_state.nWaitMask) {
			mask = ~(0x1 << portIndex);
			cmp_priv->clock_state.nWaitMask &= mask;
			if (cmp_priv->min_starttime.nTimestamp >=
					port_priv->timestamp.nTimestamp) {
				cmp_priv->min_starttime.nTimestamp =
					port_priv->timestamp.nTimestamp;
				cmp_priv->min_starttime.nPortIndex =
					port_priv->timestamp.nPortIndex;
			}
		}
		/* if there is no port to wait for, then set clock state to running */
		if (!cmp_priv->clock_state.nWaitMask &&
			cmp_priv->clock_state.eState ==
				OMX_TIME_ClockStateWaitingForStartTime) {
			cmp_priv->clock_state.eState = OMX_TIME_ClockStateRunning;
			cmp_priv->clock_state.nStartTime =
				cmp_priv->min_starttime.nTimestamp;
			cmp_priv->mediatime_base = cmp_priv->min_starttime.nTimestamp;
			cmp_priv->mediatime = cmp_priv->min_starttime.nTimestamp;
			walltime = oscl_get_usec();
			cmp_priv->walltime_base = walltime;
			OSCL_LOGI("mediatime base %lld, walltime base %lld\n",
				cmp_priv->mediatime_base, cmp_priv->walltime_base);
			cmp_priv->update_type = OMX_TIME_UpdateClockStateChanged;
			/* update state change in all port */
			for (i = 0; i < component->num_ports; i++) {
				port_priv = (clock_port_private_t *)
					component->port[i].port_private;
				port_priv->mediatime.eUpdateType =
					OMX_TIME_UpdateClockStateChanged;
				port_priv->mediatime.eState = OMX_TIME_ClockStateRunning;
				port_priv->mediatime.xScale =
					cmp_priv->config_scale.xScale;
			}
			/* Signal Buffer Management Thread */
			sem_post(&cmp_priv->sem_clockevent);
			OSCL_LOGI("Waiting for Clock Running Event for all ports\n");
			sem_wait(&cmp_priv->sem_clkevent_complete);
			OSCL_LOGI("Waiting clock running event complete\n");
		}
		break;
	case OMX_IndexConfigTimeActiveRefClock:
		ref_clock = (OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE *)data;
		memcpy(&cmp_priv->ref_clock, ref_clock, sizeof(*ref_clock));
		if (cmp_priv->clock_state.eState == OMX_TIME_ClockStateRunning)
			cmp_priv->mediatime_base = cmp_priv->mediatime;
		break;
	case OMX_IndexConfigTimeCurrentAudioReference:
		/* update ref clock from audio ref clock */
		ref_timestamp = (OMX_TIME_CONFIG_TIMESTAMPTYPE *)data;
		portIndex = ref_timestamp->nPortIndex;
		if (portIndex > component->num_ports) {
			OSCL_LOGE("error port index %d\n", portIndex);
			return OMX_ErrorBadPortIndex;
		}
		port_priv = (clock_port_private_t *)
			component->port[portIndex].port_private;
		memcpy(&port_priv->timestamp, ref_timestamp, sizeof(*ref_timestamp));
		if (ref_timestamp->nTimestamp == 0xffffffff) {
			OSCL_LOGI("audio clock end!\n");
			cmp_priv->ref_clock.eClock = OMX_TIME_RefClockNone;
			cmp_priv->mediatime_base = cmp_priv->mediatime;
			cmp_priv->walltime_base = oscl_get_usec();
			return OMX_ErrorNone;
		}
		/* only update time base if refclk is audio */
		if (cmp_priv->ref_clock.eClock == OMX_TIME_RefClockAudio)
			cmp_priv->mediatime = ref_timestamp->nTimestamp;
		break;
	case OMX_IndexConfigTimeCurrentVideoReference:
		/* update ref clock from video ref clock */
		ref_timestamp = (OMX_TIME_CONFIG_TIMESTAMPTYPE *)data;
		portIndex = ref_timestamp->nPortIndex;
		if (portIndex > component->num_ports) {
			OSCL_LOGE("error port index %d\n", portIndex);
			return OMX_ErrorBadPortIndex;
		}
		port_priv = (clock_port_private_t *)
			component->port[portIndex].port_private;
		memcpy(&port_priv->timestamp, ref_timestamp, sizeof(*ref_timestamp));
		/* only update time base if refclk is video */
		if (cmp_priv->ref_clock.eClock == OMX_TIME_RefClockVideo)
			cmp_priv->mediatime = ref_timestamp->nTimestamp;
		break;
	case OMX_IndexConfigTimeScale:
		/* update the mediatime base and walltime base
		 * using the current scale value*/
		walltime = oscl_get_usec();
		cmp_priv->walltime_base = walltime;
		cmp_priv->mediatime_base = cmp_priv->mediatime;

		/* update the new scale value */
		config_scale = (OMX_TIME_CONFIG_SCALETYPE *)data;
		memcpy(&cmp_priv->config_scale, config_scale, sizeof(*config_scale));
		cmp_priv->update_type = OMX_TIME_UpdateScaleChanged;

		/* update the scale change in all ports */
		for (i = 0; i < component->num_ports; i++) {
			port_priv = (clock_port_private_t *)
				component->port[i].port_private;
			port_priv->mediatime.eUpdateType = OMX_TIME_UpdateScaleChanged;
			port_priv->mediatime.eState = OMX_TIME_ClockStateRunning;
			port_priv->mediatime.xScale = cmp_priv->config_scale.xScale;
			port_priv->mediatime.nMediaTimestamp = cmp_priv->mediatime;
			port_priv->mediatime.nWallTimeAtMediaTime = walltime;
		}
		/* Signal Buffer Management Thread */
		sem_post(&cmp_priv->sem_clockevent);
		OSCL_LOGD("Waiting for scale change handled for all ports\n");
		sem_wait(&cmp_priv->sem_clkevent_complete);
		OSCL_LOGD("time scale change to %d\n", config_scale->xScale);
		break;
	case OMX_IndexConfigTimeMediaTimeRequest:
		config_mediatime_request(component, data);
		break;
	default:
		OSCL_LOGE("error config index %d\n", index);
		return OMX_ErrorBadParameter;
		break;
	}
	return OMX_ErrorNone;
}

OMX_ERRORTYPE clock_get_parameter(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_INOUT OMX_PTR param_data)
{
	lb_omx_component_t *component = NULL;
	OMX_OTHER_PARAM_PORTFORMATTYPE *port_format;
	clock_port_private_t *port_priv;

	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);

	switch (index) {
	case OMX_IndexParamOtherPortFormat:
		port_format = (OMX_OTHER_PARAM_PORTFORMATTYPE *)param_data;
		if (port_format->nIndex < component->num_ports) {
			port_priv = (clock_port_private_t *)
				component->port[port_format->nIndex].port_private;
			memcpy(port_format, &port_priv->other_param,
				sizeof(*port_format));
		} else {
			OSCL_LOGE("bad port index %d\n", port_format->nIndex);
			return OMX_ErrorBadPortIndex;
		}
		break;
	default:
		return base_get_parameter(cmp_hdl, index, param_data);
		break;
	}

	return OMX_ErrorNone;
}

OMX_ERRORTYPE clock_set_parameter(OMX_IN OMX_HANDLETYPE cmp_hdl,
	OMX_IN OMX_INDEXTYPE index,
	OMX_IN OMX_PTR param_data)
{
	lb_omx_component_t *component = NULL;
	OMX_OTHER_PARAM_PORTFORMATTYPE *port_format;
	clock_port_private_t *port_priv;

	oscl_param_check(cmp_hdl != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(cmp_hdl);

	switch (index) {
	case OMX_IndexParamOtherPortFormat:
		port_format = (OMX_OTHER_PARAM_PORTFORMATTYPE *)param_data;
		if (port_format->nIndex < component->num_ports) {
			port_priv = (clock_port_private_t *)
				component->port[port_format->nIndex].port_private;
			memcpy(&port_priv->other_param,
				port_format, sizeof(*port_format));
		} else {
			OSCL_LOGE("bad port index %d\n", port_format->nIndex);
			return OMX_ErrorBadPortIndex;
		}
		break;
	default:
		return base_set_parameter(cmp_hdl, index, param_data);
	}

	return OMX_ErrorNone;
}

OMX_ERRORTYPE clock_set_state(OMX_HANDLETYPE hComp,
	OMX_U32 dest_state)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	lb_omx_component_t *component = NULL;
	clock_component_private_t *cmp_priv = NULL;

	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComp);
	cmp_priv = component->component_private;

	if (component->state == OMX_StateIdle &&
		dest_state == OMX_StateLoaded) {
		OSCL_LOGI("signal clock component end\n");
		sem_post(&cmp_priv->sem_clockevent);
#ifdef AV_SYNC_LOG
		fclose(fd);
#endif
	}

	if (component->state == OMX_StateIdle &&
		dest_state == OMX_StateExecuting) {
		cmp_priv->mediatime = 0;
		cmp_priv->mediatime_base = 0;
		cmp_priv->walltime_base = 0;
	}

	if (component->state == OMX_StatePause &&
		dest_state == OMX_StateExecuting) {
		cmp_priv->mediatime_base = cmp_priv->mediatime;
		cmp_priv->walltime_base = oscl_get_usec();
	}

	if ((component->state == OMX_StateExecuting ||
		component->state == OMX_StatePause) &&
		dest_state == OMX_StateIdle) {
		cmp_priv->clock_state.eState = OMX_TIME_ClockStateStopped;
		sem_post(component->mgmt_flush_sem);
	}

	if (component->state == OMX_StateLoaded &&
		dest_state == OMX_StateIdle) {
		cmp_priv->clock_state.eState = OMX_TIME_ClockStateStopped;
#ifdef AV_SYNC_LOG
		fd = fopen("clock_timestamps.log", "w");
#endif
	}

	ret = base_component_set_state(hComp, dest_state);
	return ret;
}


void clock_buffer_handle(OMX_HANDLETYPE cmp_hdl,
	OMX_BUFFERHEADERTYPE *inbuffer,
	OMX_BUFFERHEADERTYPE *outbuffer)
{
	lb_omx_component_t *component = NULL;
	clock_port_private_t *port_priv;

	component = get_lb_component(cmp_hdl);
	port_priv = (clock_port_private_t *)
		component->port[outbuffer->nOutputPortIndex].port_private;

	memcpy(outbuffer->pBuffer, &port_priv->mediatime,
		sizeof(OMX_TIME_MEDIATIMETYPE));
	outbuffer->nFilledLen = sizeof(OMX_TIME_MEDIATIMETYPE);
	/* clear the update type */
	port_priv->mediatime.eUpdateType = OMX_TIME_UpdateMax;
}


void *clock_component_buffer_manager(void *param)
{
	lb_omx_component_t *component = NULL;
	clock_component_private_t *cmp_priv = NULL;
	base_port_t          *port[MAX_CLOCK_PORTS];
	clock_port_private_t *port_priv[MAX_CLOCK_PORTS];
	sem_t                *output_sem[MAX_CLOCK_PORTS];
	oscl_queue_t         *output_queue[MAX_CLOCK_PORTS];
	OMX_BUFFERHEADERTYPE *output_buf[MAX_CLOCK_PORTS];
	int                  being_flushed = 0;
	int                  i;

	oscl_param_check((param != NULL), NULL, "null params");
	component = get_lb_component(param);
	cmp_priv = (clock_component_private_t *)component->component_private;
	for (i = 0; i < component->num_ports; i++) {
		port[i] = &component->port[i];
		port_priv[i] = (clock_port_private_t *)port[i]->port_private;
		output_sem[i] = &port[i]->buffer_sem;
		output_queue[i] = &port[i]->buffer_queue;
		output_buf[i] = NULL;
	}

	while (component->state == OMX_StateIdle
		|| component->state == OMX_StateExecuting
		|| component->state == OMX_StatePause
		|| component->target_state == OMX_StateIdle) {

		/* flush the ports if they are being flushed */
		pthread_mutex_lock(&component->flush_mutex);
		for (i = 0; i < component->num_ports; i++)
			being_flushed |= port[i]->is_flushed;
		while (being_flushed) {
			pthread_mutex_unlock(&component->flush_mutex);
			for (i = 0; i < component->num_ports; i++) {
				if (!PORT_IS_ENABLED(port[i]))
					continue;
				if (output_buf[i] && port[i]->is_flushed) {
					port[i]->return_buffer(port[i], output_buf[i]);
					output_buf[i] = NULL;
					OSCL_LOGI("port %d being flush\n", i);
				}
			}
			OSCL_LOGI("%s sem_wait component->flush_sem",
				get_component_name(component));
			sem_wait(component->flush_sem);
			pthread_mutex_lock(&component->flush_mutex);

			being_flushed = 0;
		}
		pthread_mutex_unlock(&component->flush_mutex);

		/* wait until the state change to executing */
		if (component->state != OMX_StateExecuting) {
			sem_wait(component->buf_mgnt_sem);
			continue;
		}

		OSCL_LOGD("waiting for clock event\n");
		sem_wait(&cmp_priv->sem_clockevent);
		OSCL_LOGD("clock event occured sem value:%d",
			sem_get(&cmp_priv->sem_clockevent));

		/* todo: maybe we need to do something for unturnnel mode */

		for (i = 0; i < component->num_ports; i++) {
			if (!PORT_IS_ENABLED(port[i])) {
				OSCL_LOGD("port %d disable, continue\n", i);
				continue;
			}
			if (component->state == OMX_StateLoaded ||
				component->state == OMX_StateInvalid) {
				OSCL_LOGW("=====error state, break\n");
				break;
			}
			if (port_priv[i]->mediatime.eUpdateType ==
				OMX_TIME_UpdateClockStateChanged ||
				port_priv[i]->mediatime.eUpdateType ==
				OMX_TIME_UpdateScaleChanged ||
				port_priv[i]->mediatime.eUpdateType ==
				OMX_TIME_UpdateRequestFulfillment) {

				if (sem_get(output_sem[i]) <= 0) {
					OSCL_LOGE("have clk event %d, wait for buffer\n",
						port_priv[i]->mediatime.eUpdateType);
				}

				OSCL_LOGD("output_sem %d value %d\n",
					i, sem_get(output_sem[i]));
				sem_wait(output_sem[i]);
				output_buf[i] = oscl_queue_dequeue(output_queue[i]);
				OSCL_LOGD("after wait output_sem %d value %d\n",
					i, sem_get(output_sem[i]));

				/* process output buffer on port i */
				if (output_buf[i] != NULL) {
					if (component->buf_handle)
						component->buf_handle(component, NULL,
							output_buf[i]);
					else
						output_buf[i]->nFilledLen = 0;
				}
				/* if buffer has been produced, then return it */
				if (output_buf[i] && output_buf[i]->nFilledLen != 0) {
					port[i]->return_buffer(port[i], output_buf[i]);
					output_buf[i] = NULL;
				}
			}
		}
		/* sent clock event for all port */
		sem_post(&cmp_priv->sem_clkevent_complete);
	}
	OSCL_LOGI("end of clock buffer manager thread!\n");
	return NULL;
}


OMX_ERRORTYPE clock_component_deinit(OMX_IN OMX_HANDLETYPE hComponent)
{
	OMX_U32 i;
	lb_omx_component_t *component = NULL;
	clock_component_private_t *private = NULL;

	oscl_param_check(hComponent != NULL, OMX_ErrorBadParameter, NULL);
	component = get_lb_component(hComponent);

	/* deinit clock port */
	for (i = 0; i < component->num_ports; i++)
		clock_port_deinit(&component->port[i]);

	private = component->component_private;
	sem_destroy(&private->sem_starttime);
	sem_destroy(&private->sem_clockevent);
	sem_destroy(&private->sem_clkevent_complete);

	oscl_free(component->component_private);
	base_component_deinit(hComponent);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE clock_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_U32 i, j;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	clock_component_private_t *private = NULL;

	private = oscl_zalloc(sizeof(*private));
	if (!private) {
		OSCL_LOGE("alloc clock_component_private_t error!\n");
		return OMX_ErrorInsufficientResources;
	}
	OSCL_LOGI("========here======\n");
	/* call base init component */
	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone) {
		OSCL_LOGE("base_component_init error!\n");
		goto error1;
	}
	cmp_handle->name = "OMX.LB.SOURCE.CLOCK";
	cmp_handle->component_private = private;
	cmp_handle->buf_manager = clock_component_buffer_manager;
	cmp_handle->buf_handle = clock_buffer_handle;
	cmp_handle->do_state_set = clock_set_state;
	cmp_handle->base_comp.ComponentDeInit = clock_component_deinit;
	cmp_handle->base_comp.SetParameter = clock_set_parameter;
	cmp_handle->base_comp.SetConfig = clock_set_config;
	cmp_handle->base_comp.GetConfig = clock_get_config;
	cmp_handle->base_comp.SetParameter = clock_set_parameter;
	cmp_handle->base_comp.GetParameter = clock_get_parameter;
	cmp_handle->num_ports = 3;

	/* init all clock port */
	for (i = 0; i < cmp_handle->num_ports; i++) {
		ret = clock_port_init(cmp_handle, &cmp_handle->port[i], i, OMX_DirOutput);
		if (ret != OMX_ErrorNone) {
			OSCL_LOGE("clock_port %d _init error!\n", i);
			goto error2;
		}
	}
	cmp_handle->port[1].log_level = 4;

	/* init clock private data */
	private->clock_state.nSize = sizeof(OMX_TIME_CONFIG_CLOCKSTATETYPE);
	private->clock_state.eState = OMX_TIME_ClockStateStopped;
	private->clock_state.nStartTime = 0;
	private->clock_state.nOffset = 0;
	private->clock_state.nWaitMask = 0xFF;

	private->min_starttime.nSize = sizeof(OMX_TIME_CONFIG_TIMESTAMPTYPE);
	private->min_starttime.nTimestamp = 0;
	private->min_starttime.nPortIndex = 0;

	private->config_scale.nSize = sizeof(OMX_TIME_CONFIG_SCALETYPE);
	private->config_scale.xScale = 1 << 16; /* normal play mode */

	private->ref_clock.nSize = sizeof(OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE);
	private->ref_clock.eClock = OMX_TIME_RefClockNone;

	sem_init(&private->sem_starttime, 0, 0);
	sem_init(&private->sem_clockevent, 0, 0);
	sem_init(&private->sem_clkevent_complete, 0, 0);

	return ret;

error2:
	for (j = 0; j < i; j++)
		clock_port_deinit(&cmp_handle->port[j]);
error1:
	oscl_free(private);
	return ret;
}

