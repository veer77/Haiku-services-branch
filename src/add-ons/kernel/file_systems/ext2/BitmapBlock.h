/*
 * Copyright 2001-2010, Haiku Inc. All rights reserved.
 * This file may be used under the terms of the MIT License.
 *
 * Authors:
 *		Janito V. Ferreira Filho
 */
#ifndef BITMAPBLOCK_H
#define BITMAPBLOCK_H


#include "CachedBlock.h"


class BitmapBlock : public CachedBlock {
public:
							BitmapBlock(Volume* volume, uint32 numBits);
							~BitmapBlock();

			bool			SetTo(off_t block);
			bool			SetToWritable(Transaction& transaction,
								off_t block, bool empty = false);

			bool			CheckMarked(uint32 start, uint32 length);
			bool			CheckUnmarked(uint32 start, uint32 length);

			bool			Mark(uint32 start, uint32 length,
								bool force = false);
			bool			Unmark(uint32 start, uint32 length,
								bool force = false);

			void			FindNextMarked(uint32& pos);
			void			FindNextUnmarked(uint32& pos);

			void			FindPreviousMarked(uint32& pos);

			void			FindLargestUnmarkedRange(uint32& start,
								uint32& length);

			uint32			NumBits() const { return fNumBits; }

private:
			void			_FindNext(uint32& pos, bool marked);

			uint32*			fData;
			const uint32*	fReadOnlyData;

			uint32			fNumBits;
			uint32			fMaxIndex;
};

#endif	// BITMAPBLOCK_H
