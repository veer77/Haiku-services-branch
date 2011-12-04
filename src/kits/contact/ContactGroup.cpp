/*
 * Copyright 2011 Dario Casalinuovo <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include <ContactGroup.h>

#include <Contact.h>

//TODO define error codes

BContactGroup::BContactGroup(int32 groupID)
{
}


BContactGroup::~BContactGroup()
{
}


status_t
BContact::InitCheck() const
{
	return fInitCheck;
}


status_t
BContactGroup::AddContact(BContact* contact)
{
}


status_t
BContactGroup::RemoveContact(BContact* contact)
{
}
