/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include <ContactDefs.h>
#include <Contact.h>
#include <Path.h>
#include <Query.h>
#include <SupportDefs.h>

class BAddressBook {
public:
							BAddressBook();
							~BAddressBook();
			status_t		InitCheck();

			status_t		AddContact(BContact* contact);
			status_t		RemoveContact(BContact* contact);

			BContactList*	AllContacts();
			BContactList*	ContactsByField(int32 typecode,
								const char* value = NULL);

//			BContactList*	ContactsByGroup(BContactGroup* group);
//			BContactList*	ContactsByQuery(BContactQuery* query);
//			BGroupList*		DefaultGroup();


	//static	BAddressBook* Default();
private:
			status_t		_FindDir();
			status_t		fInitCheck;

			BContactGroup*	fGroup;
			BPath			fAddrBook;
			BQuery			fLatestQuery;
};

#endif // ADDRESS_BOOK_H
