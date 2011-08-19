/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef B_CONTACT_H
#define B_CONTACT_H

#include <Archivable.h>
#include <Message.h>
#include <ObjectList.h>

#include "ContactDefs.h"
#include "ContactField.h"
#include "RawContact.h"


class BContact : public BArchivable {
public:
							BContact(BRawContact* contact = NULL);
							BContact(BMessage* archive);
			virtual			~BContact();

			status_t		Archive(BMessage* archive, bool deep);
	static BArchivable*		Instantiate(BMessage* data);

			status_t		InitCheck() const;
/*
			int32			ID() { return fID; }
			int32			GroupID() { return fGroupID; }
*/
			status_t		AddField(BContactField* field);
			status_t		RemoveEquivalentFields(BContactField* field);
			status_t		RemoveField(BContactField* field);
			status_t		ReplaceField(BContactField* field);
			bool			HasField(BContactField* field);
			BContactField*	GetField(int index);
			int32			CountFields() const;
			status_t		CopyFieldsFrom(BContact& contact);

	// for the moment it supports only a BRawContact, in future
	// the following methods will help to merge many BRawContacts
	// into a BContact.
	//		status_t		Append(BRawContact* contact);
			const BRawContact&	RawContact() const;

			status_t		Commit();
private:
	//		status_t 		SetID(int32 id);
			status_t		_FlattenFields(BMessage* msg);
			status_t		_UnflattenFields(BMessage* msg);
			
			BRawContact* 	fRawContact;
			status_t		fInitCheck;
			int32 			fID;
			int32			fGroupID;

			BObjectList<BContactField>* fList;
//friend class BContactRoster;
};

#endif // B_CONTACT_H
