/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _CONTACT_GROUP_H
#define _CONTACT_GROUP_H

#include <Contact.h>
#include <SupportDefs.h>

typedef BObjectList<BContactGroup> BContactGroupList;


class BContactGroup {
public:
					BContactGroup(uint32 groupID);
					~BContactGroup();

	status_t		InitCheck();

	status_t		AddContact(BContactRef* contact);
	status_t		RemoveContact(BContactRef* contact);

	BContactRefList* AllContacts();
	BContactRefList* ContactsByField(ContactFieldType type,
						const char* value = NULL);
	//BContactList*	ContactsByQuery(BContactQuery* query);

	//BMessage* 	AsMessage();
private:
	BContactList 	fList;	
};

#endif // _CONTACT_GROUP_H
