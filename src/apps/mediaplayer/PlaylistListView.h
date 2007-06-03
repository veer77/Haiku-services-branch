/*
 * Copyright 2007, Haiku. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Stephan Aßmus <superstippi@gmx.de>
 */
#ifndef PLAYLIST_LIST_VIEW_H
#define PLAYLIST_LIST_VIEW_H

#include "ListViews.h"

class Controller;
class ControllerObserver;
class Playlist;
class PlaylistItem;
class PlaylistObserver;

class PlaylistListView : public SimpleListView {
 public:
								PlaylistListView(BRect frame,
									Playlist* playlist,
									Controller* controller);
	virtual						~PlaylistListView();

	// BView interface
	virtual	void				AttachedToWindow();
	virtual	void				MessageReceived(BMessage* message);

	virtual	void				MouseDown(BPoint where);
	virtual	void				KeyDown(const char* bytes, int32 numBytes);

	// SimpleListView interface
	virtual	void				MoveItems(BList& indices, int32 toIndex);
	virtual	void				CopyItems(BList& indices, int32 toIndex);
	virtual	void				RemoveItemList(BList& indices);

	virtual	void				DrawListItem(BView* owner, int32 index,
									BRect frame) const;

 private:
			void				_FullSync();
			void				_AddItem(const entry_ref& ref, int32 index);
			void				_RemoveItem(int32 index);

			void				_SetCurrentPlaylistIndex(int32 index);
			void				_SetPlaybackState(uint32 state);

			void				_RefsReceived(BMessage* message,
									int32 dropIndex);

			Playlist*			fPlaylist;
			PlaylistObserver*	fPlaylistObserver;

			Controller*			fController;
			ControllerObserver*	fControllerObserver;

			int32				fCurrentPlaylistIndex;
			uint32				fPlaybackState;

			font_height			fFontHeight;
			PlaylistItem*		fLastClickedItem;
};

#endif // PLAYLIST_LIST_VIEW_H
