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
#ifndef ADDRESS_VIEW_H
#define ADDRESS_VIEW_H

#include <String.h>
#include <GridView.h>

class BTextControl;
class BAddressContactField;

class AddressView : public BGridView {
public:
							AddressView(BAddressContactField* field);

	virtual					~AddressView();

			bool			HasChanged();
			void			Reload();
			void			UpdateAddressField();

			BString			Value() const;

			BAddressContactField* Field() const;
private:
			BTextControl*	_AddControl(const char* label, const char* value);

			BTextControl*	fStreet;
			BTextControl*	fPostalBox;
			BTextControl*	fNeighbor;
			BTextControl*	fCity;
			BTextControl*	fRegion;
			BTextControl*	fPostalCode;
			BTextControl*	fCountry;

			int32			fCount;

			BAddressContactField*	fField;
};

#endif // ADDRESS_VIEW_H
