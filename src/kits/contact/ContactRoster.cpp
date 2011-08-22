/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "ContactRoster.h"

#include <FindDirectory.h>


BAddressBook::BAddressBook()
	:
	fQuery()
{
	_FindDir();
}


BAddressBook::~BAddressBook()
{
}


status_t
BAddressBook::InitCheck()
{
	return fInitCheck;
}


status_t
BAddressBook::AddContact(BContact* contact)
{
	if (!fInitCheck)
		return fInitCheck;

	int32 count = contact->CountFields();
	
	for (int i = 0; i < count; i++) {
		
	}

	return B_OK;
}


status_t
BAddressBook::RemoveContact(BContact* contact)
{
	if (!fInitCheck)
		return fInitCheck;

	return B_OK;
}


BContactList*
BAddressBook::AllContacts()
{
	if (!fInitCheck)
		return NULL;

	return NULL;
}


BContactList*
BAddressBook::ContactsByField(int32 typecode, const char* value = NULL)
{
	if (!fInitCheck)
		return NULL;
	return NULL;
}

/*
BContactList*
BAddressBook::ContactsByGroup()
{

}*/


status_t
BAddressBook::_FindDir()
{
	status_t ret = find_directory(B_USER_DIRECTORY,
		&fAddrBook, false, NULL);
	if (ret != B_OK)
		return ret;

	ret = fAddrBook.InitCheck();
	if (ret != B_OK)
		return ret;

	fAddrBook.Append("people");
	return B_OK;
}
