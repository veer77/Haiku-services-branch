/*
 * Copyright 2010 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include <ContactRoster.h>

#include <FindDirectory.h>

BContactRoster::BContactRoster()
{
}


status_t
BContactRoster::RegisterContact(BContact* contact, uint32 group)
{

}


BContact*
BContactRoster::InstantiateContact(BContactRef& ref)
{
}


BContactGroupList&
BContactRoster::RegisteredGroups()
{
}


BAddressBook*
BContactRoster::AddressBook()
{
	return new BAddressBook();
}
