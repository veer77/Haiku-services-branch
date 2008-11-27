/*
 * Copyright 2008 Oliver Ruiz Dorantes, oliver.ruiz.dorantes_at_gmail.com
 * All rights reserved. Distributed under the terms of the MIT License.
 *
 */
#include <KernelExport.h>
#include <NetBufferUtilities.h>

#include "l2cap_internal.h"
#include "l2cap_signal.h"
#include "l2cap_command.h"
#include "l2cap_lower.h"

#include "L2capEndpoint.h"

#define BT_DEBUG_THIS_MODULE
#define SUBMODULE_NAME "upper"
#define SUBMODULE_COLOR 36
#include <btDebug.h>
#include <l2cap.h>


status_t
l2cap_l2ca_con_ind(L2capChannel* channel)
{
	L2capEndpoint* endpoint = L2capEndpoint::ForPsm(channel->psm);
	
	if (endpoint == NULL) { //TODO: refuse connection no endpoint bound
		debugf("No endpoint bound for psm %d\n", channel->psm);
		return B_ERROR;
	}

	// Pair Channel with endpoint
	channel->endpoint = endpoint;
	endpoint->BindToChannel(channel);

	debugf("Endpoint %p bound for psm %d, schannel %x dchannel %x\n",endpoint, channel->psm, channel->scid, channel->dcid);
	
	net_buffer* buf = l2cap_con_rsp(channel->ident, channel->scid, channel->dcid, L2CAP_SUCCESS, L2CAP_NO_INFO);
	L2capFrame* cmd = btCoreData->SpawnSignal(channel->conn, channel, buf, channel->ident, L2CAP_CON_RSP);
	if (cmd == NULL) {
		gBufferModule->free(buf);
		return ENOMEM;
	}
	// we can move to configuration...
	channel->state = L2CAP_CHAN_CONFIG;

	/* Link command to the queue */
	SchedConnectionPurgeThread(channel->conn);
	return B_OK;
}


status_t
l2cap_l2ca_cfg_rsp_ind(L2capChannel* channel)
{
	// if our configuration has not been yet send ..
	if(!(channel->cfgState & L2CAP_CFG_OUT_SENT)) {	
		// send config_rsp
		
		net_buffer* buf = l2cap_cfg_rsp(channel->ident, channel->dcid, 0, L2CAP_SUCCESS, NULL);
		L2capFrame* cmd = btCoreData->SpawnSignal(channel->conn, channel, buf, channel->ident, L2CAP_CFG_RSP);
		if (cmd == NULL) {
			gBufferModule->free(buf);			
			channel->state = L2CAP_CHAN_CLOSED;
			return ENOMEM;
		}
				
		flowf("Sending cfg resp\n");
		/* Link command to the queue */
		SchedConnectionPurgeThread(channel->conn);

		// set status
		channel->cfgState |= L2CAP_CFG_OUT_SENT;

	} else {
		

	}


	if ((channel->cfgState & L2CAP_CFG_BOTH) == L2CAP_CFG_BOTH) {
		// Channel can be declared open
		channel->state = L2CAP_CHAN_OPEN;
	} else {
		// send configuration Request by our side
		if (channel->endpoint->configurationSet) {
			// TODO: define complete configuration packet

		} else {
			// nothing special requested
			net_buffer* buf = l2cap_cfg_req(channel->ident, channel->dcid, 0, NULL);
			L2capFrame* cmd = btCoreData->SpawnSignal(channel->conn, channel, buf, channel->ident, L2CAP_CFG_REQ);
			if (cmd == NULL) {
				gBufferModule->free(buf);
				channel->state = L2CAP_CHAN_CLOSED;
				return ENOMEM;
			}
			
			flowf("Sending cfg req\n");
			// Link command to the queue
			SchedConnectionPurgeThread(channel->conn);

		}			
		channel->cfgState |= L2CAP_CFG_IN_SENT;			
	}

	return B_OK;
}


status_t
l2cap_upper_con_rsp(HciConnection* conn, L2capChannel* channel)
{
	flowf("\n");

	return B_OK;
}


status_t
l2cap_co_receive(HciConnection* conn, net_buffer* buffer, uint16 dcid)
{

	L2capChannel* channel = btCoreData->ChannelBySourceID(conn, dcid);

	if (channel == NULL) {
		debugf("dcid %d does not exist for handle %d\n", dcid, conn->handle);	
		return B_ERROR;
	}

	if (channel->endpoint == NULL) {
		debugf("dcid %d not bound to endpoint\n", dcid);	
		return B_ERROR;	
	}

	flowf("Enqueue to fifo\n");	
	return gStackModule->fifo_enqueue_buffer(&channel->endpoint->fReceivingFifo, buffer);

}
