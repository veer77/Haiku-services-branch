//-----------------------------------------------------------------------
//  This software is part of the OpenBeOS distribution and is covered 
//  by the OpenBeOS license.
//
//  Copyright (c) 2003-2004 Waldemar Kornewald, Waldemar.Kornewald@web.de
//-----------------------------------------------------------------------

#include "_KPPPMRUHandler.h"

#include <KPPPConfigurePacket.h>
#include <KPPPDevice.h>

#include <netinet/in.h>


#define MRU_TYPE				0x1

typedef struct mru_item {
	uint8 type;
	uint8 length;
	uint16 MRU;
} _PACKED mru_item;

status_t ParseRequestedItem(mru_item *item, KPPPInterface& interface);


_KPPPMRUHandler::_KPPPMRUHandler(KPPPInterface& interface)
	: KPPPOptionHandler("MRU Handler", MRU_TYPE, interface, NULL)
{
	Reset();
}


status_t
_KPPPMRUHandler::AddToRequest(KPPPConfigurePacket& request)
{
	if(!Interface().Device() || Interface().MRU() == 1500)
		return B_OK;
	
	// add MRU request
	mru_item item;
	item.type = MRU_TYPE;
	item.length = 4;
	item.MRU = htons(fLocalMRU);
	return request.AddItem((ppp_configure_item*) &item) ? B_OK : B_ERROR;
}


status_t
_KPPPMRUHandler::ParseNak(const KPPPConfigurePacket& nak)
{
	mru_item *item = (mru_item*) nak.ItemWithType(MRU_TYPE);
	if(!item || item->length != 4)
		return B_OK;
	
	uint16 MRU = ntohs(item->MRU);
	if(MRU < fLocalMRU)
		fLocalMRU = MRU;
	
	return B_OK;
}


status_t
_KPPPMRUHandler::ParseReject(const KPPPConfigurePacket& reject)
{
	if(reject.ItemWithType(MRU_TYPE))
		return B_ERROR;
	
	return B_OK;
}


status_t
_KPPPMRUHandler::ParseAck(const KPPPConfigurePacket& ack)
{
	uint16 MRU = 1500;
	mru_item *item = (mru_item*) ack.ItemWithType(MRU_TYPE);
	
	if(item)
		MRU = ntohs(item->MRU);
	
	if(MRU < Interface().MRU())
		fLocalMRU = MRU;
	
	return B_OK;
}


status_t
_KPPPMRUHandler::ParseRequest(const KPPPConfigurePacket& request,
	int32 index, KPPPConfigurePacket& nak, KPPPConfigurePacket& reject)
{
	if(index == reject.CountItems())
		return B_OK;
	
	return ParseRequestedItem((mru_item*) request.ItemAt(index), Interface());
	
	return B_OK;
}


status_t
_KPPPMRUHandler::SendingAck(const KPPPConfigurePacket& ack)
{
	return ParseRequestedItem((mru_item*) ack.ItemWithType(MRU_TYPE), Interface());
}


// this function contains code shared by ParseRequest and SendingAck
status_t
ParseRequestedItem(mru_item *item, KPPPInterface& interface)
{
	uint16 MRU = 1500;
	
	if(item) {
		if(item->length != 4)
			return B_ERROR;
				// the request has a corrupted item
		
		MRU = ntohs(item->MRU);
	}
	
	if(MRU < interface.MRU())
		interface.SetMRU(MRU);
	
	return B_OK;
}


void
_KPPPMRUHandler::Reset()
{
	if(Interface().Device()) {
		fLocalMRU = Interface().Device()->MTU() - 2;
		Interface().SetMRU(fLocalMRU);
	} else {
		Interface().SetMRU(1500);
		fLocalMRU = 1500;
	}
}
