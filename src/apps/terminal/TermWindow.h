/*
 * Copyright 2001-2007, Haiku.
 * Copyright (c) 2003-4 Kian Duffy <myob@users.sourceforge.net>
 * Parts Copyright (C) 1998,99 Kazuho Okui and Takashi Murai. 
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files or portions
 * thereof (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice
 *    in the  binary, as well as this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided with
 *    the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
#ifndef __TERMWINDOW_H
#define __TERMWINDOW_H

#include <String.h>
#include <Window.h>


class BMenu;
class BMenuBar;
class FindWindow;
class PrefWindow;
class TermView;
class SmartTabView;
class TermWindow : public BWindow {
public:
	TermWindow(BRect frame, const char* title, const char *command);
	virtual ~TermWindow();

protected:
	virtual void	MessageReceived(BMessage *message);
	virtual void	WindowActivated(bool);
	virtual void	MenusBeginning();
	virtual bool	QuitRequested();


private:
	void		_SetTermColors();
	void		_InitWindow(const char *command);
	void		_SetupMenu();
	status_t	_DoPageSetup();
	void		_DoPrint();
	void		_NewTab(const char *command);
	TermView*	_ActiveTermView();
	
	SmartTabView	*fTabView;
	TermView	*fTermView;
	BMenuBar	*fMenubar;
	BMenu		*fFilemenu,
			*fEditmenu,
			*fEncodingmenu,
			*fHelpmenu,
			*fFontMenu,
			*fWindowSizeMenu,
			*fNewFontMenu; 

	BMessage	*fPrintSettings;
	PrefWindow	*fPrefWindow;
	FindWindow	*fFindPanel;
	BRect		fSavedFrame;
	window_look	fSavedLook;
	
	//Saved search parameters
	BString	fFindString;
	BMenuItem	*fFindForwardMenuItem;
	BMenuItem 	*fFindBackwardMenuItem;
	bool		fFindSelection;
	bool		fForwardSearch;
	bool		fMatchCase;
	bool		fMatchWord;
};

#endif // __TERMWINDOW_H
