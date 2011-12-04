/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _CONTACT_REF_H
#define _CONTACT_REF_H

#include <Flattenable.h>

class BContactRef /*: public virtual BFlattenable*/ {
public:
				BContactRef();
				BContactRef(uint32 id);
				~BContactRef();
private:
	uint32 		fContactID;
};

#endif
