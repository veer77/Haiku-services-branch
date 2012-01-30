/*
 * Copyright 2011 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include <ContactRef.h>

BContactRef::BContactRef(int32 id, bool autoFill)
	:
	contactID(id),
	groupID(-1)
{
	//if (autoFill)
	//	BContactRoster::RefForID(this, contactID);
}


BContactRef::~BContactRef()
{
}
