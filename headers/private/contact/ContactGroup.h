/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _CONTACT_GROUP_H
#define _CONTACT_GROUP_H

#include <ContactRef.h>
#include <SupportDefs.h>

typedef BObjectList<BContactGroup> BContactGroupList;

class BContactGroup {
public:
					BContactGroup(uint32 groupID, bool custom = false);
					~BContactGroup();

	status_t		InitCheck();

	status_t		AddContact(BContactRef* contact);
	status_t		RemoveContact(BContactRef* contact);

	//BContactList*	ContactsByQuery(BContactQuery* query);

	//BMessage* 		ToMessage();

	const BContactRefList& AllContacts() const;
	//const BContactRefList& ContactsByField(ContactFieldType type,
	//					const char* value = NULL) const;
private:
	BContactRefList fList;
	uint32			fGroupID;
	bool			fCustom;
};

#endif // _CONTACT_GROUP_H
