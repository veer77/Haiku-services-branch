/*
 * Copyright 2011, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo
 */

#ifndef EXPORT_SAVE_PANEL_H
#define EXPORT_SAVE_PANEL_H

#include <FilePanel.h>
#include <MenuItem.h>
#include <TranslationDefs.h>
#include <String.h>

class SaveItem : public BMenuItem {
public:
								SaveItem(const char* name,
										 BMessage* message,
										 uint32 exportMode);
		
		uint32					ExportMode() const
									{ return fExportMode; }

private:
		uint32					fExportMode;
};

class PeopleSavePanel : public BFilePanel {
public:
								PeopleSavePanel(BMessenger* target = NULL);
		virtual					~PeopleSavePanel();

		virtual	void			SendMessage(const BMessenger* messenger,
											BMessage* message);
private:
				void			_MakeMenu();
};

#endif // EXPORT_SAVE_PANEL_H
