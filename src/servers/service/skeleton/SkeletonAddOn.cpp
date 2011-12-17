/*
 * Copyright 2010-2011, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 */

#include "SkeletonAddon.h"


BServiceAddon *
instantiate_service_addon()
{
	return new (std::nothrow) SkeletonAddOn();
}


SkeletonAddOn::SkeletonAddOn()
{

}


BService*
SkeletonAddOn::ServiceAt(int32 index) const
{
}


int32
SkeletonAddOn::CountServices() const
{
}
