/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef B_CONTACT_H
#define B_CONTACT_H

#include <Archivable.h>
#include <ContactDefs.h>
#include <ContactField.h>
#include <ContactRef.h>
#include <Message.h>
#include <ObjectList.h>
#include <RawContact.h>


class BContact : public virtual BArchivable {
public:
							BContact(BRawContact* contact = NULL);
							BContact(BContactRef& ref);
							BContact(BMessage* archive);
			virtual			~BContact();

			status_t		Archive(BMessage* archive, bool deep) const;
	static BArchivable*		Instantiate(BMessage* data);

			status_t		InitCheck() const;
/*
			int32			ID() { return fID; }
			int32			GroupID() { return fGroupID; }
*/
			const BContactRef& ContactRef();

			status_t		AddField(BContactField* field);
			status_t		RemoveEquivalentFields(BContactField* field);
			status_t		RemoveField(BContactField* field);
			status_t		ReplaceField(BContactField* field);
			bool			HasField(BContactField* field);

			BContactField*	FieldAt(int index);
			BContactField*	FieldAt(type_code type, int32 index = 0);

			int32			CountFields() const;
			int32			CountFields(type_code code) const;

			const BContactFieldList& FieldList() const;

			status_t		CopyFieldsFrom(BContact& contact);
			status_t		CreateDefaultFields();

	// for the moment it supports only a BRawContact, in future
	// the following methods will help to merge many BRawContacts
	// into a BContact.
	// delete the actual BRawContact objects
			status_t		Append(BRawContact* contact);
			const BRawContact&	RawContact() const;

			status_t		Commit();
			status_t		Reload();
private:
	//		status_t 		SetInternalID(uint32 id);
	//		status_t		AppendGroup(uint32 id);
	//		status_t		AppendGroup(BContactGroup group);

			status_t		_FlattenFields(BMessage* msg) const;
			status_t		_UnflattenFields(BMessage* msg);
			
			BRawContact* 	fRawContact;
			status_t		fInitCheck;
			int32 			fID;
			int32			fGroupID;

			const BContactRef fContactRef;

			BContactFieldList fList;

//friend class BContactRoster;
};

typedef BObjectList<BContact> BContactList;

#endif // B_CONTACT_H
