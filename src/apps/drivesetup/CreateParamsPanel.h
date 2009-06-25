/*
 * Copyright 2008 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT license.
 *
 * Authors:
 *		Stephan Aßmus <superstippi@gmx.de>
 *		Bryce Groff	  <bgroff@hawaii.edu>
 */
#ifndef CREATE_PARAMS_PANEL_H
#define CREATE_PARAMS_PANEL_H

#include "Support.h"

#include <Window.h>
#include <InterfaceKit.h>
#include <Partition.h>

class BMenuField;
class BTextControl;
class SizeSlider;

class CreateParamsPanel : public BWindow {
public:
								CreateParamsPanel(BWindow* window, 
									off_t offset, off_t size);
	virtual						~CreateParamsPanel();

	virtual	bool				QuitRequested();
	virtual	void				MessageReceived(BMessage* message);

			int32				Go(off_t& offset, off_t& size,
									BString& parameters);
			void				Cancel();


private:
			void 				_CreateViewControls(off_t offset, off_t size);
			
	class EscapeFilter;
			EscapeFilter*		fEscapeFilter;
			sem_id				fExitSemaphore;
			BWindow*			fWindow;
			int32				fReturnValue;
		
			BPopUpMenu*			fTypePopUpMenu;
			BMenuField*			fTypeMenuField;
			SizeSlider*			fSizeSlider;
			BButton*			fOKButton;
			BButton*			fCancelButton;
};

#endif // CREATE_PARAMS_PANEL_H
