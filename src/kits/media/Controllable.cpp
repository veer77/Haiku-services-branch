/***********************************************************************
 * AUTHOR: Marcus Overhagen
 *   FILE: Controllable.cpp
 *  DESCR: 
 ***********************************************************************/
#include <Controllable.h>
#include "debug.h"
#include "Notifications.h"

/*************************************************************
 * protected BControllable
 *************************************************************/

BControllable::~BControllable()
{
	UNIMPLEMENTED();
}

/*************************************************************
 * public BControllable
 *************************************************************/

BParameterWeb *
BControllable::Web()
{
	UNIMPLEMENTED();
	return NULL;
}


bool
BControllable::LockParameterWeb()
{
	UNIMPLEMENTED();

	return false;
}

/*************************************************************
 * protected BControllable
 *************************************************************/

void
BControllable::UnlockParameterWeb()
{
	UNIMPLEMENTED();
}


BControllable::BControllable()
	: BMediaNode("XXX fixme")
{
	UNIMPLEMENTED();

	AddNodeKind(B_CONTROLLABLE);
}


status_t
BControllable::SetParameterWeb(BParameterWeb *web)
{
	UNIMPLEMENTED();
	
	BPrivate::media::notifications::WebChanged(Node());

	return B_ERROR;
}


status_t
BControllable::HandleMessage(int32 message,
							 const void *data,
							 size_t size)
{
	UNIMPLEMENTED();

	return B_ERROR;
}


status_t
BControllable::BroadcastChangedParameter(int32 id)
{
	CALLED();
	return BPrivate::media::notifications::ParameterChanged(Node(), id);
}


status_t
BControllable::BroadcastNewParameterValue(bigtime_t when,
										  int32 id,
										  void *newValue,
										  size_t valueSize)
{
	CALLED();
	return BPrivate::media::notifications::NewParameterValue(Node(), id, when, newValue, valueSize);
}


status_t
BControllable::StartControlPanel(BMessenger *out_messenger)
{
	UNIMPLEMENTED();

	return B_ERROR;
}


status_t
BControllable::ApplyParameterData(const void *value,
								  size_t size)
{
	UNIMPLEMENTED();

	return B_ERROR;
}


status_t
BControllable::MakeParameterData(const int32 *controls,
								 int32 count,
								 void *buf,
								 size_t *ioSize)
{
	UNIMPLEMENTED();

	return B_ERROR;
}

/*************************************************************
 * private BControllable
 *************************************************************/

/*
private unimplemented 
BControllable::BControllable(const BControllable &clone)
BControllable & BControllable::operator=(const BControllable &clone)
*/

status_t BControllable::_Reserved_Controllable_0(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_1(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_2(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_3(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_4(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_5(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_6(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_7(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_8(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_9(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_10(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_11(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_12(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_13(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_14(void *) { return B_ERROR; }
status_t BControllable::_Reserved_Controllable_15(void *) { return B_ERROR; }


