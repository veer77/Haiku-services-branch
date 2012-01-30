/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef ADDRESS_BOOK_H
#define ADDRESS_BOOK_H

#include <ContactDefs.h>
#include <Contact.h>
#include <ContactGroup.h>
#include <Path.h>
#include <SupportDefs.h>

class BAddressBook : public BContactGroup {
public:
							BAddressBook();
							~BAddressBook();
			status_t		InitCheck();

			status_t		AddContact(BContactRef* contact);
			status_t		RemoveContact(BContactRef* contact);
			int32			CountContacts() const;
			BContactRef*	ContactAt(int32 index) const;

			BContactRefList& AllContacts();
			BContactRefList& ContactsByField(type_code type);

			BPath			GetPath();

//			BContactList*	ContactsByGroup(BContactGroup* group);
//			BContactList*	ContactsByQuery(BContactQuery* query);
//			BGroupList*		DefaultGroup();


	//static	BAddressBook* Default();
private:
			status_t		_FindDir();
			status_t		fInitCheck;
			BPath			fAddrBook;
};

#endif // ADDRESS_BOOK_H
