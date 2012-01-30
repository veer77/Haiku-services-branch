/*
 * Copyright 2005-2010, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Robert Polic
 *		Stephan Aßmus <superstippi@gmx.de>
 *
 * Copyright 1999, Be Incorporated.   All Rights Reserved.
 * This file may be used under the terms of the Be Sample Code License.
 */
#ifndef PEOPLE_APP_H
#define PEOPLE_APP_H


#include <Application.h>
#include <ObjectList.h>
#include <String.h>

// TODO move it to ContactDefs.h and update
// the code to use it
#define	B_PERSON_MIMETYPE	"application/x-person"
#define	B_VCARD_MIMETYPE	"text/x-vCard"
#define APP_SIG				"application/x-vnd.Be-PEPL"


class BFile;

enum {
	M_NEW					= 'newp',
	M_SAVE_AS				= 'svas',
	M_WINDOW_QUITS			= 'wndq',
	M_CONFIGURE_ATTRIBUTES	= 'catr'
};

class PersonWindow;

class TPeopleApp : public BApplication {
public:
								TPeopleApp();
		virtual 				~TPeopleApp();

		virtual void			ArgvReceived(int32, char**);
		virtual void			MessageReceived(BMessage*);
		virtual void			RefsReceived(BMessage*);
		virtual void			ReadyToRun();

private:
				PersonWindow*	_FindWindow(const entry_ref&) const;
				PersonWindow*	_NewWindow(const entry_ref* ref = NULL,
					BFile* file = NULL);
				void			_SavePreferences(BMessage* message) const;

private:
				BFile*			fPrefs;
				uint32			fWindowCount;
				BRect			fPosition;
};

#endif // PEOPLE_APP_H

