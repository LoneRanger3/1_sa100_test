/*
 * base_filter_component.c - demo code for filter component.
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <oscl.h>
#include <base_component.h>

void filter_buf_handle(OMX_HANDLETYPE comp,
	OMX_BUFFERHEADERTYPE *inbuf, OMX_BUFFERHEADERTYPE *outbuf)
{
	if (inbuf == NULL || outbuf == NULL)
		return;
	outbuf->nFilledLen = 0;
	if (inbuf->pBuffer == NULL || outbuf->pBuffer == NULL)
		return;
	if (inbuf->nFilledLen == 0)
		return;
	outbuf->nFlags = inbuf->nFlags;
	outbuf->nOffset = inbuf->nOffset;
	outbuf->nTimeStamp = inbuf->nTimeStamp;
	outbuf->nFilledLen = inbuf->nFilledLen;
	memcpy(outbuf->pBuffer + outbuf->nOffset, inbuf->pBuffer + inbuf->nOffset,
		inbuf->nFilledLen);
	inbuf->nFilledLen = 0;
	if (inbuf->nFlags & OMX_BUFFERFLAG_EOS) {
		OSCL_LOGE("filter_buf_handle get the end of stream\n");
		outbuf->nFlags |= OMX_BUFFERFLAG_EOS;
	}
}

OMX_ERRORTYPE filter_component_deinit(OMX_IN OMX_HANDLETYPE hComp)
{
	OMX_COMPONENTTYPE *base_cmp = (OMX_COMPONENTTYPE *)hComp;
	lb_omx_component_t *component;
	int ret;

	OSCL_TRACE("base_cmp_handle:%p\n", hComp);
	oscl_param_check(hComp != NULL, OMX_ErrorBadParameter, NULL);
	component = (lb_omx_component_t *)base_cmp->pComponentPrivate;

	component->port[OMX_DEFAULT_INPUT_PORT].deinit(
		&component->port[OMX_DEFAULT_INPUT_PORT]);
	component->port[OMX_DEFAULT_OUTPUT_PORT].deinit(
		&component->port[OMX_DEFAULT_OUTPUT_PORT]);
	ret = base_component_deinit(hComp);
	return ret;
}

OMX_ERRORTYPE filter_component_init(lb_omx_component_t *cmp_handle)
{
	OMX_ERRORTYPE ret = OMX_ErrorNone;

	OSCL_TRACE("filter_component_init:%p\n", cmp_handle);
	ret = base_component_init(cmp_handle);
	if (ret != OMX_ErrorNone)
		return ret;

	cmp_handle->num_ports = 2;
	ret = base_port_init(cmp_handle, &cmp_handle->port[OMX_DEFAULT_INPUT_PORT],
			OMX_DEFAULT_INPUT_PORT,
			OMX_DirInput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		return ret;
	}
	ret = base_port_init(cmp_handle, &cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT],
			OMX_DEFAULT_OUTPUT_PORT,
			OMX_DirOutput);
	if (ret != OMX_ErrorNone) {
		base_component_deinit(cmp_handle);
		cmp_handle->port[OMX_DEFAULT_INPUT_PORT].deinit(
			&cmp_handle->port[OMX_DEFAULT_INPUT_PORT]);
		return ret;
	}
	cmp_handle->port[OMX_DEFAULT_INPUT_PORT].port_param.nBufferSize =
		OMX_DEFAULT_BUF_SIZE;
	cmp_handle->port[OMX_DEFAULT_INPUT_PORT].port_param.eDomain =
		OMX_PortDomainOther;

	cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT].port_param.nBufferSize =
		OMX_DEFAULT_BUF_SIZE;
	cmp_handle->port[OMX_DEFAULT_OUTPUT_PORT].port_param.eDomain =
		OMX_PortDomainOther;

	cmp_handle->base_comp.ComponentDeInit = filter_component_deinit;
	cmp_handle->buf_handle = filter_buf_handle;
	OSCL_TRACE("filter_component_init:%d\n", ret);
	return ret;
}





