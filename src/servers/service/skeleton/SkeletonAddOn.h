/*
 * Copyright 2010-2011, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _SERVICE_ADDON_H
#define _SERVICE_ADDON_H

#include <image.h>

#include <String.h>

class BBitmap;

class BService;

class SkeletonAddOn : public BServiceAddOn {
public:
					SkeletonAddOn(image_id image, const char* path);

	BService*		ServiceAt(int32 index) const;
	int32			CountServices() const;

private:

};

#endif
