/*
 * Copyright 2001-2005, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		DarkWyrm <bpmagic@columbus.rr.com>
 *		Adi Oanca <adioanca@mymail.ro>
 *		Stephan Aßmus <superstippi@gmx.de>
 */

/**	Data classes for working with BView states and draw parameters */


#include <stdio.h>

#include <Region.h>

#include "LinkReceiver.h"
#include "LinkSender.h"

#include "LayerData.h"


// constructor
DrawData::DrawData()
	: fOrigin(0.0, 0.0),
	  fScale(1.0),
	  fClippingRegion(NULL),
	  fHighColor(0, 0, 0, 255),
	  fLowColor(255, 255, 255, 255),
	  fPattern(kSolidHigh),
	  fDrawingMode(B_OP_COPY),
	  fAlphaSrcMode(B_PIXEL_ALPHA),
	  fAlphaFncMode(B_ALPHA_OVERLAY),
	  fPenLocation(0.0, 0.0),
	  fPenSize(1.0),
	  fFont(*gFontManager->GetSystemPlain()),
	  fFontAliasing(false),
	  fSubPixelPrecise(false),
	  fLineCapMode(B_BUTT_CAP),
	  fLineJoinMode(B_BEVEL_JOIN),
	  fMiterLimit(B_DEFAULT_MITER_LIMIT)
{
	fUnscaledFontSize = fFont.Size();
}

// copy constructor
DrawData::DrawData(const DrawData& from)
	: fClippingRegion(NULL)
{
	*this = from;
}

// copy constructor
DrawData::DrawData(const DrawData* from)
	: fOrigin(0.0, 0.0),
	  fScale(1.0),
	  fClippingRegion(NULL)
{
	if (from->fClippingRegion) {
		SetClippingRegion(*(from->fClippingRegion));
	}

	fHighColor			= from->fHighColor;
	fLowColor			= from->fLowColor;
	fPattern			= from->fPattern;

	fDrawingMode		= from->fDrawingMode;
	fAlphaSrcMode		= from->fAlphaSrcMode;
	fAlphaFncMode		= from->fAlphaFncMode;

	fPenLocation		= from->fPenLocation;
	fPenSize			= from->fPenSize;

	fFont				= from->fFont;
	fFontAliasing		= from->fFontAliasing;

	fSubPixelPrecise	= from->fSubPixelPrecise;

	fLineCapMode		= from->fLineCapMode;
	fLineJoinMode		= from->fLineJoinMode;
	fMiterLimit			= from->fMiterLimit;

	// Since fScale is reset to 1.0, the unscaled
	// font size is the current size of the font
	// (which is from->fUnscaledFontSize * from->fScale)
	fUnscaledFontSize	= fFont.Size();
}

// destructor
DrawData::~DrawData()
{
	delete fClippingRegion;
}

// operator=
DrawData&
DrawData::operator=(const DrawData& from)
{
	fOrigin	= from.fOrigin;
	fScale	= from.fScale;

	if (from.fClippingRegion) {
		SetClippingRegion(*(from.fClippingRegion));
	} else {
		delete fClippingRegion;
		fClippingRegion = NULL;
	}

	fHighColor			= from.fHighColor;
	fLowColor			= from.fLowColor;
	fPattern			= from.fPattern;

	fDrawingMode		= from.fDrawingMode;
	fAlphaSrcMode		= from.fAlphaSrcMode;
	fAlphaFncMode		= from.fAlphaFncMode;

	fPenLocation		= from.fPenLocation;
	fPenSize			= from.fPenSize;

	fFont				= from.fFont;
	fFontAliasing		= from.fFontAliasing;

	fSubPixelPrecise	= from.fSubPixelPrecise;

	fLineCapMode		= from.fLineCapMode;
	fLineJoinMode		= from.fLineJoinMode;
	fMiterLimit			= from.fMiterLimit;

	fUnscaledFontSize	= from.fUnscaledFontSize;

	return *this;
}

// SetOrigin
void
DrawData::SetOrigin(const BPoint& origin)
{
	fOrigin = origin;
}

// OffsetOrigin
void
DrawData::OffsetOrigin(const BPoint& offset)
{
	fOrigin += offset;
}

// SetScale
void
DrawData::SetScale(float scale)
{
	if (fScale != scale) {
		fScale = scale;
		// update font size
		// (pen size is currently calulated on the fly)
		fFont.SetSize(fUnscaledFontSize * fScale);
	}
}

// SetClippingRegion
void
DrawData::SetClippingRegion(const BRegion& region)
{
	if (region.Frame().IsValid()) {
		if (fClippingRegion)
			*fClippingRegion = region;
		else 
			fClippingRegion = new BRegion(region);
	} else {
		delete fClippingRegion;
		fClippingRegion = NULL;
	}
}

// SetHighColor
void
DrawData::SetHighColor(const RGBColor& color)
{
	fHighColor = color;
}

// SetLowColor
void
DrawData::SetLowColor(const RGBColor& color)
{
	fLowColor = color;
}

// SetPattern
void
DrawData::SetPattern(const Pattern& pattern)
{
	fPattern = pattern;
}

// SetDrawingMode
void
DrawData::SetDrawingMode(drawing_mode mode)
{
	fDrawingMode = mode;
}

// SetBlendingMode
void
DrawData::SetBlendingMode(source_alpha srcMode, alpha_function fncMode)
{
	fAlphaSrcMode = srcMode;
	fAlphaFncMode = fncMode;
}

// SetPenLocation
void
DrawData::SetPenLocation(const BPoint& location)
{
	// TODO: Needs to be in local coordinate system!
	// There is going to be some work involved in
	// other parts of app_server...
	fPenLocation = location;
}

// PenLocation
const BPoint&
DrawData::PenLocation() const
{
	// TODO: See above
	return fPenLocation;
}

// SetPenSize
void
DrawData::SetPenSize(float size)
{
	fPenSize = size;
}

// PenSize
// * returns the scaled pen size
float
DrawData::PenSize() const
{
	float penSize = fPenSize * fScale;
	// NOTE: As documented in the BeBook,
	// pen size is never smaller than 1.0.
	// This is supposed to be the smallest
	// possible device size.
	if (penSize < 1.0)
		penSize = 1.0;
	return penSize;
}


// SetFont
// * sets the font to be already scaled by fScale
void
DrawData::SetFont(const ServerFont& font, uint32 flags)
{
	if (flags == B_FONT_ALL) {
		fFont = font;
		fUnscaledFontSize = font.Size();
		fFont.SetSize(fUnscaledFontSize * fScale);
	} else {
		// family & style
		if (flags & B_FONT_FAMILY_AND_STYLE)
			fFont.SetFamilyAndStyle(font.GetFamilyAndStyle());
		// size
		if (flags & B_FONT_SIZE) {
			fUnscaledFontSize = font.Size();
			fFont.SetSize(fUnscaledFontSize * fScale);
		}
		// shear
		if (flags & B_FONT_SHEAR)
			fFont.SetShear(font.Shear());
		// rotation
		if (flags & B_FONT_ROTATION)
			fFont.SetRotation(font.Rotation());
		// spacing
		if (flags & B_FONT_SPACING)
			fFont.SetSpacing(font.Spacing());
		// encoding
		if (flags & B_FONT_ENCODING)
			fFont.SetEncoding(font.Encoding());
		// face
		if (flags & B_FONT_FACE)
			fFont.SetFace(font.Face());
		// flags
		if (flags & B_FONT_FLAGS)
			fFont.SetFlags(font.Flags());
	}
}

// SetForceFontAliasing
void
DrawData::SetForceFontAliasing(bool aliasing)
{
	fFontAliasing = aliasing;
}

// SetSubPixelPrecise
void
DrawData::SetSubPixelPrecise(bool precise)
{
	fSubPixelPrecise = precise;
}

// SetLineCapMode
void
DrawData::SetLineCapMode(cap_mode mode)
{
	fLineCapMode = mode;
}

// SetLineJoinMode
void
DrawData::SetLineJoinMode(join_mode mode)
{
	fLineJoinMode = mode;
}

// SetMiterLimit
void
DrawData::SetMiterLimit(float limit)
{
	fMiterLimit = limit;
}

//----------------------------LayerData----------------------
// #pragmamark -

// constructpr
LayerData::LayerData()
	: DrawData(),
	  prevState(NULL)
{
}

// LayerData
LayerData::LayerData(const LayerData& data)
	: DrawData()
{
	fClippingRegion = NULL;
	*this = data;
}

// LayerData
LayerData::LayerData(LayerData* data)
	: DrawData(data),
	  prevState(data)
{
}

// destructor
LayerData::~LayerData()
{
	delete prevState;
}

// operator=
LayerData&
LayerData::operator=(const LayerData& from)
{
	DrawData::operator=(from);

	prevState = from.prevState;
	
	return *this;
}

// PrintToStream
void
LayerData::PrintToStream() const
{
	printf("\t Origin: (%.1f, %.1f)\n", fOrigin.x, fOrigin.y);
	printf("\t Scale: %.2f\n", fScale);

	printf("\t Pen Location and Size: (%.1f, %.1f) - %.2f (%.2f)\n",
		   fPenLocation.x, fPenLocation.y, PenSize(), fPenSize);

	printf("\t HighColor: "); fHighColor.PrintToStream();
	printf("\t LowColor: "); fLowColor.PrintToStream();
	printf("\t Pattern: %llu\n", fPattern.GetInt64());

	printf("\t DrawMode: %lu\n", (uint32)fDrawingMode);
	printf("\t AlphaSrcMode: %ld\t AlphaFncMode: %ld\n",
		   (int32)fAlphaSrcMode, (int32)fAlphaFncMode);

	printf("\t LineCap: %d\t LineJoin: %d\t MiterLimit: %.2f\n",
		   (int16)fLineCapMode, (int16)fLineJoinMode, fMiterLimit);

	if (fClippingRegion)
		fClippingRegion->PrintToStream();

	printf("\t ===== Font Data =====\n");
	printf("\t Style: CURRENTLY NOT SET\n"); // ???
	printf("\t Size: %.1f (%.1f)\n", fFont.Size(), fUnscaledFontSize);
	printf("\t Shear: %.2f\n", fFont.Shear());
	printf("\t Rotation: %.2f\n", fFont.Rotation());
	printf("\t Spacing: %ld\n", fFont.Spacing());
	printf("\t Encoding: %ld\n", fFont.Encoding());
	printf("\t Face: %d\n", fFont.Face());
	printf("\t Flags: %lu\n", fFont.Flags());
}

// ReadFontFromLink
void
LayerData::ReadFontFromLink(BPrivate::LinkReceiver& link)
{
	uint16 mask;
	link.Read<uint16>(&mask);

	if (mask & B_FONT_FAMILY_AND_STYLE) {
		uint32 fontID;
		link.Read<int32>((int32*)&fontID);
		fFont.SetFamilyAndStyle(fontID);
	}

	if (mask & B_FONT_SIZE) {
		float size;
		link.Read<float>(&size);
		fFont.SetSize(size);
	}
	
	if (mask & B_FONT_SHEAR) {
		float shear;
		link.Read<float>(&shear);
		fFont.SetShear(shear);
	}

	if (mask & B_FONT_ROTATION) {
		float rotation;
		link.Read<float>(&rotation);
		fFont.SetRotation(rotation);
	}

	if (mask & B_FONT_SPACING) {
		uint8 spacing;
		link.Read<uint8>(&spacing);
		fFont.SetSpacing(spacing);
	}

	if (mask & B_FONT_ENCODING) {
		uint8 encoding;
		link.Read<uint8>((uint8*)&encoding);
		fFont.SetEncoding(encoding);
	}

	if (mask & B_FONT_FACE) {
		uint16 face;
		link.Read<uint16>(&face);
		fFont.SetFace(face);
	}

	if (mask & B_FONT_FLAGS) {
		uint32 flags;
		link.Read<uint32>(&flags);
		fFont.SetFlags(flags);
	}
}

// ReadFromLink
void
LayerData::ReadFromLink(BPrivate::LinkReceiver& link)
{
	rgb_color highColor;
	rgb_color lowColor;
	pattern patt;

	link.Read<BPoint>(&fPenLocation);
	link.Read<float>(&fPenSize);
	link.Read(&highColor, sizeof(rgb_color));
	link.Read(&lowColor, sizeof(rgb_color));
	link.Read(&patt, sizeof(pattern));
	link.Read<int8>((int8*)&fDrawingMode);
	link.Read<BPoint>(&fOrigin);
	link.Read<int8>((int8*)&fLineJoinMode);
	link.Read<int8>((int8*)&fLineCapMode);
	link.Read<float>(&fMiterLimit);
	link.Read<int8>((int8*)&fAlphaSrcMode);
	link.Read<int8>((int8*)&fAlphaFncMode);
	link.Read<float>(&fScale);
	link.Read<bool>(&fFontAliasing);

	fHighColor = highColor;
	fLowColor = lowColor;
	fPattern = patt;

	// read clipping
	int32 clipRectCount;
	link.Read<int32>(&clipRectCount);

	BRegion region;
	if (clipRectCount > 0) {
		BRect rect;
		for (int32 i = 0; i < clipRectCount; i++) {
			link.Read<BRect>(&rect);
			region.Include(rect);
		}
	}
	SetClippingRegion(region);
}

// WriteToLink
void
LayerData::WriteToLink(BPrivate::LinkSender& link) const
{
	rgb_color hc = fHighColor.GetColor32();
	rgb_color lc = fLowColor.GetColor32();
	
	// Attach font state
	link.Attach<uint32>(fFont.GetFamilyAndStyle());
	link.Attach<float>(fFont.Size());
	link.Attach<float>(fFont.Shear());
	link.Attach<float>(fFont.Rotation());
	link.Attach<uint8>(fFont.Spacing());
	link.Attach<uint8>(fFont.Encoding());
	link.Attach<uint16>(fFont.Face());
	link.Attach<uint32>(fFont.Flags());
	
	// Attach view state
	link.Attach<BPoint>(fPenLocation);
	link.Attach<float>(fPenSize);
	link.Attach(&hc, sizeof(rgb_color));
	link.Attach(&lc, sizeof(rgb_color));
	link.Attach<uint64>(fPattern.GetInt64());
	link.Attach<BPoint>(fOrigin);
	link.Attach<uint8>((uint8)fDrawingMode);
	link.Attach<uint8>((uint8)fLineCapMode);
	link.Attach<uint8>((uint8)fLineJoinMode);
	link.Attach<float>(fMiterLimit);
	link.Attach<uint8>((uint8)fAlphaSrcMode);
	link.Attach<uint8>((uint8)fAlphaFncMode);
	link.Attach<float>(fScale);
	link.Attach<bool>(fFontAliasing);

	int32 clippingRectCount = fClippingRegion ? fClippingRegion->CountRects() : 0;
	link.Attach<int32>(clippingRectCount);

	if (fClippingRegion) {
		for (int i = 0; i < clippingRectCount; i++)
			link.Attach<BRect>(fClippingRegion->RectAt(i));
	}
}

