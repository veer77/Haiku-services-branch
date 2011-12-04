/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include <ContactDefs.h>
#include <Contact.h>
#include <Path.h>
#include <SupportDefs.h>

class BAddressBook {
public:
							BAddressBook();
							~BAddressBook();
			status_t		InitCheck();

			status_t		AddContact(BContact* contact,
				const char* name = NULL);

			status_t		RemoveContact(BContact* contact);

			BContactList*	AllContacts();
			BContactList*	ContactsByField(type_code type);

//			BContactList*	ContactsByGroup(BContactGroup* group);
//			BContactList*	ContactsByQuery(BContactQuery* query);
//			BGroupList*		DefaultGroup();


	//static	BAddressBook* Default();
private:
			status_t		_FindDir();
			status_t		fInitCheck;

			BContactList*	fAddrList;
			BPath			fAddrBook;
};

#endif // ADDRESS_BOOK_H
