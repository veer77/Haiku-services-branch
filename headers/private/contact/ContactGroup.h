/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _CONTACT_GROUP_H
#define _CONTACT_GROUP_H

#include <ContactDefs.h>
#include <ContactRef.h>
#include <SupportDefs.h>


class BContactGroup {
public:
					BContactGroup(uint32 groupID = B_CONTACT_GROUP_NONE,
						bool custom = false);
					~BContactGroup();

	status_t		InitCheck() const;

	status_t		AddContact(BContactRef* contact);
	status_t		RemoveContact(BContactRef* contact);
	int32			CountContacts() const;
	BContactRef*	ContactAt(int32 index) const;

	//BContactList*	ContactsByQuery(BContactQuery* query);

	//BMessage* 		ToMessage();

	const BContactRefList& AllContacts() const;
	const BContactRefList& ContactsByField(ContactFieldType type,
						const char* value = NULL) const;
protected:
	BContactRefList fList;
private:
	status_t		fInitCheck;
	uint32			fGroupID;
	bool			fCustom;
};

typedef BObjectList<BContactGroup> BContactGroupList;

#endif // _CONTACT_GROUP_H
