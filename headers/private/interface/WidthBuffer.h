/*
 * Copyright (c) 2003-2004 OpenBeOS
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 *
 * File:		WidthBuffer.cpp
 * Author:		Stefano Ceccherini (burton666@libero.it)
 * Description: WidthBuffer stores charachters widths in a hash table, to be able
 *				to retrieve them without passing through the app server.
 *				Used by BTextView and OpenTracker.
 */
#ifndef __WIDTHBUFFER_H
#define __WIDTHBUFFER_H

#include <TextView.h>

#include "TextViewSupportBuffer.h"


class BFont;


// TODO: enable this as soon as we are sure opentracker works
// with our libraries, since using a BFont here (as Dano does) is much better,
// as fonts can be classified also by spacing mode and other attributes.
#define USE_DANO_WIDTHBUFFER 0 

namespace BPrivate {

class TextGapBuffer;

struct _width_table_ {
#if USE_DANO_WIDTHBUFFER
	BFont font;				// corresponding font
#else
	int32 fontCode;			// font code
	float fontSize;			// font size
#endif
	int32 hashCount;		// number of hashed items
	int32 tableCount;		// size of table
	void *widths;			// width table	
};

class WidthBuffer : public _BTextViewSupportBuffer_<_width_table_> {
public:
	WidthBuffer();
	virtual ~WidthBuffer();

	float StringWidth(const char *inText, int32 fromOffset, int32 length,
		const BFont *inStyle);
	float StringWidth(TextGapBuffer &gapBuffer, int32 fromOffset,
		int32 length, const BFont *inStyle);

private:
	bool FindTable(const BFont *font, int32 *outIndex);
	int32 InsertTable(const BFont *font);
	
	bool GetEscapement(uint32 value, int32 index, float *escapement);
	float HashEscapements(const char *chars, int32 numChars, int32 numBytes,
		int32 tableIndex, const BFont *font);
	
	static uint32 Hash(uint32);
};

extern WidthBuffer* gWidthBuffer;

} // namespace BPrivate

using BPrivate::WidthBuffer;

#if __GNUC__ < 3
//! NetPositive binary compatibility support

class _BWidthBuffer_ : public _BTextViewSupportBuffer_<BPrivate::_width_table_> {
	_BWidthBuffer_();
	virtual ~_BWidthBuffer_();
};

extern
_BWidthBuffer_* gCompatibilityWidthBuffer;

#endif // __GNUC__ < 3

#endif // __WIDTHBUFFER_H
