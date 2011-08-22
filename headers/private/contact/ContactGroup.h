/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _CONTACT_GROUP_H
#define _CONTACT_GROUP_H

#include <SupportDefs.h>
#include <Contact.h>

typedef BObjectList<BContactGroup> BContactGroupList;


class BContactGroup {
public:
					BContactGroup(int32 groupID);
					~BContactGroup();

	status_t		AddContact(BContact* contact);
	status_t		RemoveContact(BContact* contact);

	BContactList*	AllContacts();
	BContactList*	ContactsByField(ContactFieldType type,
						const char* value = NULL);
//	BContactList*	ContactsByQuery(BContactQuery* query);
private:
	BContactList* 	fList;	
};

#endif // _CONTACT_GROUP_H
