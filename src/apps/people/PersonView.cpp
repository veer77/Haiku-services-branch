/*
 * Copyright 2010, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Robert Polic
 *		Stephan Aßmus <superstippi@gmx.de>
 * 		Casalinuovo Dario
 *
 * Copyright 1999, Be Incorporated.   All Rights Reserved.
 * This file may be used under the terms of the Be Sample Code License.
 */


#include "PersonView.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <BitmapStream.h>
#include <Catalog.h>
#include <fs_attr.h>
#include <Box.h>
#include <ControlLook.h>
#include <GridLayout.h>
#include <Locale.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <PopUpMenu.h>
#include <Query.h>
#include <TranslationUtils.h>
#include <Translator.h>
#include <VolumeRoster.h>
#include <Window.h>

#include "ContactFieldTextControl.h"
#include "AddressView.h"
#include "PictureView.h"


#undef B_TRANSLATE_CONTEXT
#define B_TRANSLATE_CONTEXT "People"


PersonView::PersonView(const char* name, BContact* contact, BFile* file)
	:
	BGridView(B_VERTICAL),
//	fGroups(NULL),
	fControls(20, false),
	fAddrView(NULL),
	fPictureView(NULL),
	fSaving(false),
	fContact(contact),
	fPhotoField(NULL),
	fFile(file)
{
	SetName(name);
	SetFlags(Flags() | B_WILL_DRAW);

	UpdatePicture(NULL);

	fInfoView = new BGridView(B_VERTICAL);

	fFieldsBox = new BBox("Contact informations",
		B_WILL_DRAW, B_FANCY_BORDER, fInfoView);

	//float spacing = be_control_look->DefaultItemSpacing();
	//fInfoView->GridLayout()->SetInsets(spacing);

	GridLayout()->AddView(fFieldsBox, 0, GridLayout()->CountRows());

	if (fFile)
		fFile->GetModificationTime(&fLastModificationTime);

	_LoadFieldsFromContact();
}


PersonView::~PersonView()
{
	delete fContact;
}


struct ContactVisitor : public BContactFieldVisitor {
public:
					ContactVisitor(PersonView* owner)
					:
					fOwner(owner)
					{
					}
	virtual		 	~ContactVisitor()
					{
					}

	virtual void 	Visit(BStringContactField* field)
	{
		ContactFieldTextControl* control = new ContactFieldTextControl(field);
	//	BGridLayout* layout = fOwner->fInfoView->GridLayout();
		fOwner->GridLayout()->AddView(control, 0, fOwner->fControls.CountItems());

		BGroupLayout* controlLayout = (BGroupLayout*)control->GetLayout();
		controlLayout->AlignLayoutWith(fOwner->GridLayout(), B_VERTICAL);

		fOwner->fControls.AddItem(control);
	}

	virtual void 	Visit(BAddressContactField* field)
	{
		if (fOwner->fAddrView != NULL 
			&& fOwner->fAddrView->Field()->Value() == field->Value())
				return;

		fOwner->fAddrView = new AddressView(field);

		BBox* box = new BBox("addrbox", B_WILL_DRAW,
			B_FANCY_BORDER, fOwner->fAddrView);

		box->SetLabel("Postal Address");
		fOwner->GridLayout()->AddView(box, 1, 0);
	}

	virtual void 	Visit(BPhotoContactField* field)
	{
		if (field != NULL) {
			BBitmap* bitmap = field->Photo();
			fOwner->UpdatePicture(bitmap);
		}
	}

private:
	PersonView* fOwner;
};


void
PersonView::AddField(BContactField* field)
{
	if (field == NULL)
		return;

	ContactVisitor visitor(this);

	field->Accept(&visitor);
}


void
PersonView::MakeFocus(bool focus)
{
	if (focus && fControls.CountItems() > 0)
		fControls.ItemAt(0)->MakeFocus();
	else
		BView::MakeFocus(focus);
}


void
PersonView::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case M_SAVE:
			Save();
			break;

		case M_REVERT:
			if (fPictureView)
				fPictureView->Revert();

			if (fAddrView)
				fAddrView->Reload();

			for (int32 i = fControls.CountItems() - 1; i >= 0; i--)
				fControls.ItemAt(i)->Reload();
			break;

		case M_SELECT:
			for (int32 i = fControls.CountItems() - 1; i >= 0; i--) {
				BTextView* text = fControls.ItemAt(i)->TextView();
				if (text->IsFocus()) {
					text->Select(0, text->TextLength());
					break;
				}
			}
			break;

		case M_GROUP_MENU:
		{
			/*const char* name = NULL;
			if (msg->FindString("group", &name) == B_OK)
				SetAttribute(fCategoryAttribute, name, false);*/
			break;
		}

	}
}


void
PersonView::Draw(BRect updateRect)
{
	if (!fPictureView)
		return;

	// Draw a alert/get info-like strip
	BRect stripeRect = Bounds();
	stripeRect.right = GridLayout()->HorizontalSpacing()
		+ fPictureView->Bounds().Width() / 2;
	SetHighColor(tint_color(ViewColor(), B_DARKEN_1_TINT));
	FillRect(stripeRect);
}


/*
void
PersonView::BuildGroupMenu()
{
	if (fGroups == NULL)
		return;

	BMenuItem* item;
	while ((item = fGroups->ItemAt(0)) != NULL) {
		fGroups->RemoveItem(item);
		delete item;
	}

	int32 count = 0;

	BVolumeRoster volumeRoster;
	BVolume volume;
	while (volumeRoster.GetNextVolume(&volume) == B_OK) {
		BQuery query;
		query.SetVolume(&volume);

		char buffer[256];
		snprintf(buffer, sizeof(buffer), "%s=*", fCategoryAttribute.String());
		query.SetPredicate(buffer);
		query.Fetch();

		BEntry entry;
		while (query.GetNextEntry(&entry) == B_OK) {
			BFile file(&entry, B_READ_ONLY);
			attr_info info;

			if (file.InitCheck() == B_OK
				&& file.GetAttrInfo(fCategoryAttribute, &info) == B_OK
				&& info.size > 1) {
				if (info.size > sizeof(buffer))
					info.size = sizeof(buffer);

				if (file.ReadAttr(fCategoryAttribute.String(), B_STRING_TYPE,
						0, buffer, info.size) < 0) {
					continue;
				}

				const char *text = buffer;
				while (true) {
					char* offset = strstr(text, ",");
					if (offset != NULL)
						offset[0] = '\0';

					if (!fGroups->FindItem(text)) {
						int32 index = 0;
						while ((item = fGroups->ItemAt(index)) != NULL) {
							if (strcmp(text, item->Label()) < 0)
								break;
							index++;
						}
						BMessage* message = new BMessage(M_GROUP_MENU);
						message->AddString("group", text);
						fGroups->AddItem(new BMenuItem(text, message), index);
						count++;
					}
					if (offset) {
						text = offset + 1;
						while (*text == ' ')
							text++;
					}
					else
						break;
				}
			}
		}
	}

	if (count == 0) {
		fGroups->AddItem(item = new BMenuItem(
			B_TRANSLATE_WITH_CONTEXT("none", "Groups list"),
			new BMessage(M_GROUP_MENU)));
		item->SetEnabled(false);
	}

	fGroups->SetTargetForItems(this);
}*/


void
PersonView::CreateFile(const entry_ref* ref)
{
	// TODO At the moment People save the files into the People format,
	// this should be changed to something that can support every
	// translator
	fContact->Append(new BRawContact(B_PERSON_FORMAT, new BFile(ref,
		B_READ_WRITE | B_CREATE_FILE)));

	Save();
}


bool
PersonView::IsSaved() const
{
	if (fPictureView && fPictureView->HasChanged())
		return false;

	if (fAddrView != NULL && fAddrView->HasChanged())
		return false;

	for (int32 i = fControls.CountItems() - 1; i >= 0; i--) {
		if (fControls.ItemAt(i)->HasChanged())
			return false;
	}

	return true;
}


void
PersonView::Save()
{
	fSaving = true;

	int32 count = fControls.CountItems();
	for (int32 i = 0; i < count; i++) {
		ContactFieldTextControl* control = fControls.ItemAt(i);
		control->UpdateField();
	}

	if (fAddrView != NULL)
		fAddrView->UpdateAddressField();

	if (fPictureView && fPictureView->HasChanged()) {
		fPictureView->Update();
		BBitmap* bitmap = fPictureView->Bitmap();

		if (fPhotoField == NULL) {
			fPhotoField = new BPhotoContactField(bitmap);
			fContact->AddField(fPhotoField);
		} else {
			fPhotoField->SetPhoto(bitmap);
		}
	}

	fFile->GetModificationTime(&fLastModificationTime);

	fContact->Commit();
	// TODO alert here if error

	fSaving = false;
}


void
PersonView::UpdateData(BFile* file)
{
	fFile = file;
	fContact->Append(new BRawContact(B_PERSON_FORMAT, file));
}


void
PersonView::UpdatePicture(BBitmap* bitmap)
{
	if (fPictureView == NULL) {
		fPictureView = new PictureView(70, 90, bitmap);

		GridLayout()->AddView(fPictureView, 0, 0, 1, 5);
		GridLayout()->ItemAt(0, 0)->SetExplicitAlignment(
			BAlignment(B_ALIGN_CENTER, B_ALIGN_TOP));

		return;
	}

	if (fSaving)
		return;
/*
	time_t modificationTime = 0;
	BEntry entry(ref);
	entry.GetModificationTime(&modificationTime);

	if (entry.InitCheck() == B_OK
		&& modificationTime <= fLastModificationTime) {
		return;
	}*/

	fPictureView->Update(bitmap);
}


bool
PersonView::IsTextSelected() const
{
	for (int32 i = fControls.CountItems() - 1; i >= 0; i--) {
		BTextView* text = fControls.ItemAt(i)->TextView();

		int32 start, end;
		text->GetSelection(&start, &end);
		if (start != end)
			return true;
	}
	return false;
}


void
PersonView::Reload()
{

}


void
PersonView::_LoadFieldsFromContact()
{
	if (fContact->CountFields() == 0)
		fContact->CreateDefaultFields();

	for (int i = 0; i < fContact->CountFields(); i++) {
		BContactField* field = fContact->FieldAt(i);
		if (field != NULL)
			AddField(field);
	}
}
