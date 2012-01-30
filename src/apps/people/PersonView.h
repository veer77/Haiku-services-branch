/*
 * Copyright 2010, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Robert Polic
 *		Stephan Aßmus <superstippi@gmx.de>
 *
 * Copyright 1999, Be Incorporated.   All Rights Reserved.
 * This file may be used under the terms of the Be Sample Code License.
 */
#ifndef PERSON_VIEW_H
#define PERSON_VIEW_H


#include <ContactDefs.h>
#include <Contact.h>
#include <ContactField.h>
#include <GridView.h>
#include <GroupView.h>
#include <ObjectList.h>
#include <String.h>
#include <TextControl.h>


class AddressView;
class ContactFieldTextControl;
class BFile;
class BPopUpMenu;
class PictureView;

enum {
	M_SAVE			= 'save',
	M_REVERT		= 'rvrt',
	M_SELECT		= 'slct',
	M_GROUP_MENU	= 'grmn',
};


class PersonView : public BGroupView {
public:
								PersonView(const char* name,
									BContact* contact, BFile* file);
	virtual						~PersonView();

	virtual	void				MakeFocus(bool focus = true);
	virtual	void				MessageReceived(BMessage* message);
//	virtual void				Draw(BRect updateRect);

//			void				BuildGroupMenu();

			void				CreateFile(const entry_ref* ref);

			bool				IsSaved() const;
			void				Save();

			void				AddField(BContactField* field);

			void				UpdatePicture(BBitmap* bitmap);
			void				UpdateData(BFile* file);
			void				Reload();

			bool				IsTextSelected() const;

private:
			void				_LoadFieldsFromContact();

			time_t				fLastModificationTime;
//			BPopUpMenu*			fGroups;
			typedef BObjectList<ContactFieldTextControl> FieldList;
			FieldList			fControls;

			AddressView*		fAddrView;

			BString				fCategoryAttribute;
			PictureView*		fPictureView;
			bool				fSaving;
			BContact*			fContact;

			BGridView*			fGridView;
			BPhotoContactField*	fPhotoField;
			BFile*				fFile;
};

#endif // PERSON_VIEW_H
