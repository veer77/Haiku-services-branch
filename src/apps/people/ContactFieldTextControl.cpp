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

#include <Font.h>
#include <Catalog.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#undef B_TRANSLATE_CONTEXT
#define B_TRANSLATE_CONTEXT "People"


ContactFieldTextControl::ContactFieldTextControl(BContactField* field)
	:
	BTextControl(NULL, "", NULL),
	fField(field)
{
	printf("field pointer %p\n", field);
	SetLabel(field->Label());
	SetText(field->Value());

	SetAlignment(B_ALIGN_RIGHT, B_ALIGN_LEFT);
}


ContactFieldTextControl::~ContactFieldTextControl()
{
}


bool
ContactFieldTextControl::HasChanged()
{
	return fField->Value() != Text();
}


void
ContactFieldTextControl::Reload()
{
	if (HasChanged())
		SetText(fField->Value());
}


void
ContactFieldTextControl::UpdateField()
{
	fField->SetValue(Text());
}


BString
ContactFieldTextControl::Value() const
{
	return fField->Value();
}
