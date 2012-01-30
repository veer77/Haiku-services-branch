/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "AddressBook.h"

#include <shared/AutoDeleter.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <NodeInfo.h>

#include <stdio.h>

// This class should be refactored

BAddressBook::BAddressBook()
	:
	BContactGroup(B_CONTACT_GROUP_ADDRESS_BOOK, true)
{
	fInitCheck = _FindDir();
}


BAddressBook::~BAddressBook()
{/*
	int count = fAddrList.CountItems();
	for (int i = 0; i < count; i++) {
		BContact* contact = fAddrList.ItemAt(i);
		delete contact;
	}*/
}


status_t
BAddressBook::InitCheck()
{
	if (fInitCheck == B_OK)
		return BContactGroup::InitCheck();
	else
		return fInitCheck;
}


BPath
BAddressBook::GetPath()
{
	return fAddrBook;
}


status_t
BAddressBook::AddContact(BContactRef* contact)
{
	/*if (fInitCheck != B_OK)
		return fInitCheck;

	BPath path = fAddrBook;

	if (filename == NULL) {
		BStringContactField* field 
			= (BStringContactField*)contact->FieldAt(B_CONTACT_NAME);

		if (field != NULL) {
			path.Append(field->Value());
		} else {
			return B_ERROR;
		}
	} else {
		path.Append(filename);
	}

	BContact* dest = new BContact(new BRawContact(B_PERSON_FORMAT,
		new BFile(path.Path(), B_READ_WRITE)));

	//ObjectDeleter<BContact> deleter(dest);

	int32 count = contact->CountFields();	
	for (int i = 0; i < count; i++)
		dest->AddField(contact->FieldAt(i));

	dest->Commit();

	if (fAddrList)
		fAddrList->AddItem(dest);*/

	return B_OK;
}


status_t
BAddressBook::RemoveContact(BContactRef* contact)
{
	if (fInitCheck != B_OK)
		return fInitCheck;

	return B_OK;
}


BContactRefList&
BAddressBook::AllContacts()
{
/*	if (fInitCheck != B_OK)
		return NULL;

	if (fAddrList != NULL)
		return fAddrList;

	fAddrList = new BContactList();

	BDirectory dir(fAddrBook.Path());

	BNodeInfo info;
	entry_ref ref;
	while (dir.GetNextRef(&ref) == B_OK) {
		BFile* file = new BFile(&ref, B_READ_WRITE);
		info.SetTo(file);
		char type[B_MIME_TYPE_LENGTH];
		info.GetType(type);
		if (strcmp("application/x-person", type) != 0)
			continue;

		printf("contact %s", ref.name);
		BContact* contact = new BContact(
			new BRawContact(B_PERSON_FORMAT, file));

		if (contact->InitCheck() == B_OK)
			fAddrList->AddItem(contact);
	}*/
	printf("addrl\n");
	return fList;
}


BContactRefList&
BAddressBook::ContactsByField(type_code type)
{/*
	if (fInitCheck != B_OK)
		return NULL;

	if (type == B_CONTACT_PHOTO)
		return NULL;

	BContactList* list = AllContacts();
*/
	BContactRefList* ret = new BContactRefList();
/*
	int count = list->CountItems();

	for (int i = 0; i < count; i++) {
		BContact* contact = list->ItemAt(i);
		for (int j = 0; j < contact->CountFields(); j++) {
			BContactField* field = contact->FieldAt(j);
			if (field->FieldType() == type)
				ret->AddItem(contact);
		}
	}*/

	return *ret;
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

	printf("%s\n", fAddrBook.Path());
	return B_OK;
}
