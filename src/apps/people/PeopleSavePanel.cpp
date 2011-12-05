/*
 * Copyright 2011, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Dario Casalinuovo
 */

#include "PeopleSavePanel.h"

#include <Window.h>


PeopleSavePanel::PeopleSavePanel(BMessenger* target)
	:
	BFilePanel(B_SAVE_PANEL, target)
	  //BHandler(name),
{
	BWindow* window = Window();
	if (!window || !window->Lock())
		return;

	window->SetTitle("Save Contact As");
}


PeopleSavePanel::~PeopleSavePanel()
{
}


void
PeopleSavePanel::SendMessage(const BMessenger* messenger, BMessage* message)
{
	//if (message)
		//message->AddInt32("export mode", ExportMode());

	BFilePanel::SendMessage(messenger, message);
}


void
PeopleSavePanel::_MakeMenu()
{
}
