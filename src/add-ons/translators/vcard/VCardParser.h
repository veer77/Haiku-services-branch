/*
 * Copyright 2010 Casalinuovo Dario
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _VCARD_PARSER_H
#define _VCARD_PARSER_H

#include <SupportDefs.h>
#include <ContactDefs.h>
#include <ContactField.h>
#include <ObjectList.h>

#include "cardparser.h"


class VCardParser {
public:
					VCardParser(BPositionIO* from, bool check = false);
	virtual			~VCardParser();

	status_t		Parse();

	bool			HasBegin();
	bool			HasEnd();

	const char*		Version();

	void 			PropHandler(const CARD_Char *propName,
						const CARD_Char **params);
	void 			DataHandler(const CARD_Char *data, int len);

	int32			CountProperties();
	BContactField*	PropertyAt(int32 i);
	// rename Fields
	BObjectList<BContactField>* Properties();
private:
	CARD_Parser 	fParser;
	BPositionIO*	fFrom;

	bool			fOnlyCheck;
	bool 			fCheck;
	bool 			fBegin;
	bool 			fEnd;
	BString 		fLatestProp;
	// TODO add a list of strings
	// to pass the params

	BObjectList<BContactField> fList;
};

#endif // _H
