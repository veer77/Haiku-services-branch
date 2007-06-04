/*
 * Copyright 2007, Haiku. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Stephan Aßmus <superstippi@gmx.de>
 */
#ifndef COPY_PL_ITEMS_COMMAND_H
#define COPY_PL_ITEMS_COMMAND_H


#include "Command.h"

class Playlist;

class CopyPLItemsCommand : public Command {
 public:
								CopyPLItemsCommand(
									Playlist* playlist,
									const int32* indices,
									int32 count,
									int32 toIndex);
	virtual						~CopyPLItemsCommand();
	
	virtual	status_t			InitCheck();

	virtual	status_t			Perform();
	virtual status_t			Undo();

	virtual void				GetName(BString& name);

 private:
			Playlist*			fPlaylist;
			entry_ref*			fRefs;
			int32				fToIndex;
			int32				fCount;
};

#endif // COPY_PL_ITEMS_COMMAND_H
