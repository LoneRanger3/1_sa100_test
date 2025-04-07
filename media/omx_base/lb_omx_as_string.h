/*
 * lb_omx_as_string.h - Standard functionality for lombo omx core.
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef LB_OMX_AS_STRING_H
#define LB_OMX_AS_STRING_H

static inline const char *omx_state_as_string(OMX_STATETYPE i)
{
	switch (i) {
	case OMX_StateInvalid:
		return "Invalid";
	case OMX_StateLoaded:
		return "Loaded";
	case OMX_StateIdle:
		return "Idle";
	case OMX_StateExecuting:
		return "Executing";
	case OMX_StatePause:
		return "Pause";
	case OMX_StateWaitForResources:
		return "WaitForResources";
	default:
		break;
	}
	return "unknown state??";
}

static inline const char *omx_cmd_as_string(OMX_COMMANDTYPE i)
{
	switch (i) {
	case OMX_CommandStateSet:
		return "StateSet";
	case OMX_CommandFlush:
		return "Flush";
	case OMX_CommandPortDisable:
		return "PortDisable";
	case OMX_CommandPortEnable:
		return "PortEnable";
	case OMX_CommandMarkBuffer:
		return "MarkBuffer";
	default:
		break;
	}
	return "unknown command?";
}

static inline const char *omx_evt_as_string(OMX_EVENTTYPE i)
{
	switch (i) {
	case OMX_EventCmdComplete:
		return "CmdComplete";
	case OMX_EventError:
		return "Error";
	case OMX_EventMark:
		return "Mark";
	case OMX_EventPortSettingsChanged:
		return "PortSettingsChanged";
	case OMX_EventBufferFlag:
		return "BufferFlag";
	case OMX_EventResourcesAcquired:
		return "ResourcesAcquired";
	case OMX_EventComponentResumed:
		return "ComponentResumed";
	case OMX_EventDynamicResourcesAvailable:
		return "DynamicResourcesAvailable";
	case OMX_EventPortFormatDetected:
		return "PortFormatDetected";
	case OMX_EventOutputRendered:
		return "OutputRendered";
	case OMX_EventDataSpaceChanged:
		return "DataSpaceChanged";
	default:
		break;
	}
	return "unknown event??";
}


#endif

