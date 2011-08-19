/*
 * Copyright 2005-2011, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Robert Polic
 *
 * Copyright 1999, Be Incorporated.   All Rights Reserved.
 * This file may be used under the terms of the Be Sample Code License.
 */
#ifndef CONTACT_FIELD_TEXT_CONTROL_H
#define CONTACT_FIELD_TEXT_CONTROL_H

#include <ContactDefs.h>
#include <ContactField.h>
#include <String.h>
#include <TextControl.h>

class ContactFieldTextControl : public BTextControl {
public:
					ContactFieldTextControl(BContactField* field);
					~ContactFieldTextControl();

	bool			HasChanged();
	void			Revert();
	void			Update();

	BString			Value() const;

	BContactField*	Field() const { return fField; }
private:
	BContactField*	fField;
};

#endif // CONTACT_FIELD_TEXT_CONTROL_H
