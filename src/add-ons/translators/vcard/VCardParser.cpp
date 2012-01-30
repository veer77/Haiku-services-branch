/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "VCardParser.h"

#include <stdio.h>

#include "VCardParserDefs.h"


// c++ bindings to the c API
void HandleProp(void *userData,
	const CARD_Char *propName, const CARD_Char **params)
{
	VCardParser* owner = (VCardParser*)userData;
	owner->PropHandler(propName, params);
}


void
HandleData(void *userData, const CARD_Char *data, int len)
{
	VCardParser* owner = (VCardParser*)userData;
	owner->DataHandler(data, len);
}


VCardParser::VCardParser(BPositionIO* from, bool onlyCheck)
	:
	fFrom(from),
	fOnlyCheck(onlyCheck),
	fCheck(false),
	fBegin(false),
	fEnd(false)
{
	// intialize the parser
	fParser = CARD_ParserCreate(NULL);
	CARD_SetUserData(fParser, this);
	CARD_SetPropHandler(fParser, HandleProp);
	CARD_SetDataHandler(fParser, HandleData);
}


VCardParser::~VCardParser()
{
	// free the object
	CARD_ParserFree(fParser);
}


status_t
VCardParser::Parse()
{
	if (fFrom == NULL)
		return B_ERROR;

	char buf[512];
	ssize_t read;
	read = fFrom->Read(buf, sizeof(buf));

	while (read > 0) {
		int err = CARD_Parse(fParser, buf, read, false);
		if (err == 0)
			return B_ERROR;
		read = fFrom->Read(buf, sizeof(buf));
	}
	// end of the parse
	CARD_Parse(fParser, NULL, 0, true);
	return B_OK;
}


bool
VCardParser::HasBegin()
{
	return fBegin && fCheck;
}


bool
VCardParser::HasEnd()
{
	return fEnd && fCheck;
}


int32
VCardParser::CountProperties()
{
	return fList.CountItems();
}


BContactField*
VCardParser::PropertyAt(int32 i)
{
	return fList.ItemAt(i);
}

	
BObjectList<BContactField>*
VCardParser::Properties()
{
	return &fList;
}


void
VCardParser::PropHandler(const CARD_Char *propName, const CARD_Char **params)
{
	if (!fBegin && strcasecmp(propName, "BEGIN") == 0) {
		fBegin = true;
        return;
	}

    if (!fBegin)
        return;

	if (strcasecmp(propName, "END") == 0) {
		fEnd = true;
		return;
	}
	
	if (!fCheck)
		return;

	if (fOnlyCheck)
		return;
	// TODO this is actually
	// not considering 
	// the params
	// make the code better
	// in this sense
	printf("-----%s\n", propName);
	fLatestProp.SetTo(propName);
}


void
VCardParser::DataHandler(const CARD_Char *data, int len)
{
	if (fBegin && !fCheck) {
		if (len > 0) {
			BString str(data, len);
			if (str.ICompare("VCARD") == 0)
				fCheck = true;
			return;
		}
	} else if (fEnd) {
		if (len > 0) {
			BString str(data, len);
			if (str.ICompare("VCARD") == 0)
				fCheck = true;
			else
				fCheck = false;
		}
		return;
	}

	if (fOnlyCheck)
		return;

	if (len == 0) {
		return;
	}

	BString str;
	for (int i = 0; i < len; i++) {
		CARD_Char c = data[i];
		if (c == '\r')
			continue;
		else if (c == '\n')
			continue;
		else if (c >= ' ' && c <= '~')
			str.Append((char)c, 1);
	}

	// TODO For these comparisons, consider using a std::map or something,
	// this will improve performance greatly.
	// To maintain case insensitivity, just use a duplicate of str set to
	// either upper or lower case, whichever matches your std::map.

	// it's actually using case insensitive compare
	// to give more tollerance for the vcard file
	BContactField* field = NULL;
	if (fLatestProp.ICompare(VCARD_NAME) == 0) {
		field = new BStringContactField(B_CONTACT_NAME, str);
	} else if (fLatestProp.ICompare(VCARD_NICKNAME) == 0) {
		field = new BStringContactField(B_CONTACT_NICKNAME, str);
	} else if (fLatestProp.ICompare(VCARD_FORMATTED_NAME) == 0) {
		field = new BStringContactField(B_CONTACT_NAME, str);
	} else if (fLatestProp.ICompare(VCARD_URL) == 0) {
		field = new BStringContactField(B_CONTACT_URL, str);
	} else if (fLatestProp.ICompare(VCARD_EMAIL) == 0) {
		field = new BStringContactField(B_CONTACT_EMAIL, str);
	} else if (fLatestProp.ICompare(VCARD_ORGANIZATION) == 0) {
		field = new BStringContactField(B_CONTACT_ORGANIZATION, str);
	} else if (fLatestProp.ICompare(VCARD_NOTE) == 0) {
		field = new BStringContactField(B_CONTACT_NOTE, str);
	} else if (fLatestProp.ICompare(VCARD_TELEPHONE) == 0) {
		field = new BStringContactField(B_CONTACT_PHONE, str);
	} else if (fLatestProp.ICompare(VCARD_ADDRESS) == 0) {
		field = new BAddressContactField(str, true);
	}

	if (field != NULL) {
		fList.AddItem(field);
		printf("--%s\n", str.String());
	}
}
