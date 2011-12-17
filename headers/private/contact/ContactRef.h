/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _CONTACT_REF_H
#define _CONTACT_REF_H

#include <Flattenable.h>
#include <ObjectList.h>


class BContactRef /*: public virtual BFlattenable*/ {
public:
				BContactRef();
				BContactRef(uint32 id);
				~BContactRef();

	const char*	name;
	const char*	nickname;
	const char* address;
	const char* email;
	uint32 		contactID;
	uint32		groupID;
};

typedef BObjectList<BContactRef> BContactRefList;

#endif
