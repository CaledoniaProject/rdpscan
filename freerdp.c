/**
 * FreeRDP: A Remote Desktop Protocol Client
 * FreeRDP Core
 *
 * Copyright 2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rdp.h"
#include "input.h"
#include "update.h"
#include "surface.h"
#include "transport.h"
#include "connection.h"

#include <freerdp/freerdp.h>
#include <freerdp/utils/memory.h>

boolean freerdp_connect(freerdp* instance)
{
	rdpRdp* rdp;
	boolean status = false;

	rdp = instance->context->rdp;

	IFCALLRET(instance->PreConnect, status, instance);

	if (status != true)
	{
		printf("freerdp_pre_connect failed\n");
		return false;
	}

	status = rdp_client_connect(rdp);

	if (status)
	{

		IFCALLRET(instance->PostConnect, status, instance);

		if (status != true)
		{
			printf("freerdp_post_connect failed\n");
			return false;
		}

		if (instance->settings->play_rfx)
		{
			STREAM* s;
			rdpUpdate* update;

			s = stream_new(1024);
			update = instance->update;

			xfree(s->data);
			return true;
		}
	}

	return status;
}

boolean freerdp_get_fds(freerdp* instance, void** rfds, int* rcount, void** wfds, int* wcount)
{
	rdpRdp* rdp;

	rdp = instance->context->rdp;
	transport_get_fds(rdp->transport, rfds, rcount);

	return true;
}

boolean freerdp_check_fds(freerdp* instance)
{
	int status;
	rdpRdp* rdp;

	rdp = instance->context->rdp;

	status = rdp_check_fds(rdp);

	if (status < 0)
		return false;

	return true;
}

void freerdp_send_keep_alive(freerdp* instance)
{
	input_send_synchronize_event(instance->context->rdp->input, 0);
}

static int freerdp_send_channel_data(freerdp* instance, int channel_id, uint8* data, int size)
{
	return rdp_send_channel_data(instance->context->rdp, channel_id, data, size);
}

boolean freerdp_disconnect(freerdp* instance)
{
	rdpRdp* rdp;

	rdp = instance->context->rdp;
	transport_disconnect(rdp->transport);

	return true;
}

boolean freerdp_shall_disconnect(freerdp* instance)
{

	return instance->context->rdp->disconnect;
}

void freerdp_get_version(int* major, int* minor, int* revision)
{
	if (major != NULL)
		*major = FREERDP_VERSION_MAJOR;

	if (minor != NULL)
		*minor = FREERDP_VERSION_MINOR;

	if (revision != NULL)
		*revision = FREERDP_VERSION_REVISION;
}

void freerdp_context_new(freerdp* instance)
{
	rdpRdp* rdp;

	rdp = rdp_new(instance);
	instance->input = rdp->input;
	instance->update = rdp->update;
	instance->settings = rdp->settings;

	instance->context = (rdpContext*) xzalloc(instance->context_size);
	instance->context->instance = instance;
	instance->context->rdp = rdp;

	instance->update->context = instance->context;
	instance->update->pointer->context = instance->context;
	instance->update->primary->context = instance->context;
	instance->update->secondary->context = instance->context;
	instance->update->altsec->context = instance->context;

	instance->input->context = instance->context;

	IFCALL(instance->ContextNew, instance, instance->context);
}

void freerdp_context_free(freerdp* instance)
{
	if (instance->context == NULL)
		return;

	IFCALL(instance->ContextFree, instance, instance->context);

	rdp_free(instance->context->rdp);
	xfree(instance->context);
	instance->context = NULL;
}

uint32 freerdp_error_info(freerdp* instance)
{
	return instance->context->rdp->errorInfo;
}

freerdp* freerdp_new()
{
	freerdp* instance;

	instance = xzalloc(sizeof(freerdp));

	if (instance != NULL)
	{
		instance->context_size = sizeof(rdpContext);
		instance->SendChannelData = freerdp_send_channel_data;
	}

	return instance;
}

void freerdp_free(freerdp* instance)
{
	if (instance)
	{
		xfree(instance);
	}
}
