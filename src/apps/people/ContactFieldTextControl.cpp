/*
 * Copyright 2005-2011, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Robert Polic
 *      Dario Casalinuovo
 *
 * Copyright 1999, Be Incorporated.   All Rights Reserved.
 * This file may be used under the terms of the Be Sample Code License.
 */

#include "ContactFieldTextControl.h"

#include <Catalog.h>
#include <Font.h>
#include <MenuField.h>
#include <PopUpMenu.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#undef B_TRANSLATE_CONTEXT
#define B_TRANSLATE_CONTEXT "People"


ContactFieldTextControl::ContactFieldTextControl(BContactField* field)
	:
	BView(NULL, B_WILL_DRAW),
	fField(field)
{
	printf("ContactFieldTextControl field pointer %p\n", field);

	//const char* label = 
	//	BContactField::ExtendedLabel(field->FieldType(), field->Usage());

	//SetLabel(label);
	fTextControl = new BTextControl(NULL, "", NULL);
	fTextControl->SetText(field->Value());
	fTextControl->SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);

	BPopUpMenu* menu = new BPopUpMenu("test menu");
	BMenuField* field = new BMenuField("Menu", menu,
           B_WILL_DRAW);
    AddChild(field);
}


ContactFieldTextControl::~ContactFieldTextControl()
{
}


bool
ContactFieldTextControl::HasChanged()
{
	return fField->Value() != fTextControl->Text();
}


void
ContactFieldTextControl::Reload()
{
	if (HasChanged())
		fTextControl->SetText(fField->Value());
}


void
ContactFieldTextControl::UpdateField()
{
	fField->SetValue(fTextControl->Text());
}


BString
ContactFieldTextControl::Value() const
{
	return fField->Value();
}


BContactField*
ContactFieldTextControl::Field() const
{
	return fField;
}


BTextView*
ContactFieldTextControl::TextView() const
{
	return fTextControl->TextView();
}
