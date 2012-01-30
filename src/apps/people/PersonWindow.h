/*
 * Copyright 2010, Haiku, Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Robert Polic
 *
 * Copyright 1999, Be Incorporated.   All Rights Reserved.
 * This file may be used under the terms of the Be Sample Code License.
 */
#ifndef PERSON_WINDOW_H
#define PERSON_WINDOW_H


#include <Contact.h>
#include <String.h>
#include <Window.h>


#define	TITLE_BAR_HEIGHT	 25
#define	WIND_WIDTH			420
#define WIND_HEIGHT			340


class PersonView;
class BFilePanel;
class BMenuItem;
class BFile;


class PersonWindow : public BWindow {
public:

								PersonWindow(BRect frame,
									const char* title,
									const entry_ref* ref,
									BContact* contact);
	virtual						~PersonWindow();

	virtual	void				MenusBeginning();
	virtual	void				MessageReceived(BMessage* message);
	virtual	bool				QuitRequested();
	virtual	void				Show();

			void				SaveAs();

			bool				RefersPersonFile(const entry_ref& ref) const;

private:
			void				_GetDefaultFileName(char* name);
			void				_SetToRef(entry_ref* ref);
			void				_WatchChanges(bool doIt);

private:
			const entry_ref*	fRef;

			BFilePanel*			fPanel;
			BMenuItem*			fCopy;
			BMenuItem*			fCut;
			BMenuItem*			fPaste;
			BMenuItem*			fRevert;
			BMenuItem*			fSave;
			BMenuItem*			fUndo;
			PersonView*			fView;
			BFile*				fFile;

			BString				fNameAttribute;
};


#endif // PERSON_WINDOW_H
