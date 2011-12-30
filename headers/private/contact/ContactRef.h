/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _CONTACT_REF_H
#define _CONTACT_REF_H

#include <Flattenable.h>
#include <ObjectList.h>

// at the moment, i don't think that some physical
// information about the contact is useful.
// Just because it's not coherent with the design,
// since a BContact is supposed to be an high level object.
// Another story is how to load a BContact using a BContactRef.

class BContactRef /*: public virtual BFlattenable*/ {
public:
				BContactRef(int32 id, bool autoFill = false);
				~BContactRef();

	const char*	name;
	const char*	nickname;
	const char* email;
	int32 		contactID;
	int32		groupID;
};

typedef BObjectList<BContactRef> BContactRefList;

#endif
