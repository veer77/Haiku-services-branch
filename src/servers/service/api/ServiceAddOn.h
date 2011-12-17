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

class BServiceAddOn {
public:
							BServiceAddOn(image_id image, const char* path);

	virtual	status_t		InitCheck() const;

	virtual BService* 		ServiceAt(int32 index) const;
	virtual int32			CountServices() const;

			const char*		Name() const;
			const char*		FriendlyName() const;
			BBitmap*		DisplayIcon() const;

			const char*		Path() const;
private:

};

#endif
