/*
 * Copyright 2006, Haiku. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Stephan Aßmus <superstippi@gmx.de>
 *		Ingo Weinhold <bonefish@cs.tu-berlin.de>
 */

#include "IconUtils.h"

#include <new>
#include <fs_attr.h>

#include <Bitmap.h>
#include <Node.h>
#include <TypeConstants.h>

#include "Icon.h"
#include "IconRenderer.h"
#include "FlatIconImporter.h"

using std::nothrow;


// TODO: put into HaikuBuildCompatibility.h
#ifndef __HAIKU__
# define B_BITMAP_NO_SERVER_LINK 0
# define B_BAD_DATA B_ERROR
# define B_MINI_ICON_TYPE 'MICN'
# define B_LARGE_ICON_TYPE 'ICON'
#endif


// GetIcon
status_t
BIconUtils::GetIcon(BNode* node,
					const char* vectorIconAttrName,
					const char* smallIconAttrName,
					const char* largeIconAttrName,
					icon_size size,
					BBitmap* result)
{
	if (!result || result->InitCheck())
		return B_BAD_VALUE;

	status_t ret = B_ERROR;

	switch (result->ColorSpace()) {
		case B_RGBA32:
		case B_RGB32:
			// prefer vector icon
			ret = GetVectorIcon(node, vectorIconAttrName, result);
			if (ret < B_OK) {
				// try to fallback to B_CMAP8 icons
				// (converting to B_RGBA32 is handled)
				ret = GetCMAP8Icon(node,
								   smallIconAttrName,
								   largeIconAttrName,
								   size, result);
			}
			break;

		case B_CMAP8:
			// prefer old B_CMAP8 icons
			ret = GetCMAP8Icon(node,
							   smallIconAttrName,
							   largeIconAttrName,
							   size, result);
			if (ret < B_OK) {
				// try to fallback to vector icon
				BBitmap temp(result->Bounds(),
							 B_BITMAP_NO_SERVER_LINK, B_RGBA32);
				ret = temp.InitCheck();
				if (ret < B_OK)
					break;
				ret = GetVectorIcon(node, vectorIconAttrName, &temp);
				if (ret < B_OK)
					break;
				uint32 width = temp.Bounds().IntegerWidth() + 1;
				uint32 height = temp.Bounds().IntegerHeight() + 1;
				uint32 bytesPerRow = temp.BytesPerRow();
				ret = ConvertFromCMAP8((uint8*)temp.Bits(),
									   width,height, bytesPerRow, result);
			}
			break;
		default:
			break;
	}

	return ret;
}

// #pragma mark -

// GetVectorIcon
status_t
BIconUtils::GetVectorIcon(BNode* node, const char* attrName,
						  BBitmap* result)
{
	if (!node || node->InitCheck() < B_OK || !attrName)
		return B_BAD_VALUE;

#if TIME_VECTOR_ICONS
bigtime_t startTime = system_time();
#endif

	// get the attribute info and check type and size of the attr contents
	attr_info attrInfo;
	status_t ret = node->GetAttrInfo(attrName, &attrInfo);
	if (ret < B_OK)
		return ret;

	type_code attrType = B_RAW_TYPE;
		// TODO: introduce special type?

	if (attrInfo.type != attrType)
		return B_BAD_TYPE;

	// chicken out on unrealisticly large attributes
	if (attrInfo.size > 16 * 1024)
		return B_BAD_VALUE;

	uint8 buffer[attrInfo.size];
	node->ReadAttr(attrName, attrType, 0, buffer, attrInfo.size);

#if TIME_VECTOR_ICONS
bigtime_t importTime = system_time();
#endif

	ret = GetVectorIcon(buffer, attrInfo.size, result);
	if (ret < B_OK)
		return ret;

#if TIME_VECTOR_ICONS
bigtime_t finishTime = system_time();
printf("read: %lld, import: %lld\n", importTime - startTime, finishTime - importTime);
#endif

	return B_OK;
}

// GetVectorIcon
status_t
BIconUtils::GetVectorIcon(const uint8* buffer, size_t size,
						  BBitmap* result)
{
	if (!result)
		return B_BAD_VALUE;

	status_t ret = result->InitCheck();
	if (ret < B_OK)
		return ret;

	if (result->ColorSpace() != B_RGBA32 && result->ColorSpace() != B_RGB32)
		return B_BAD_VALUE;

	Icon icon;
	ret = icon.InitCheck();
	if (ret < B_OK)
		return ret;

	FlatIconImporter importer;
	ret = importer.Import(&icon, const_cast<uint8*>(buffer), size);
	if (ret < B_OK)
		return ret;

	IconRenderer renderer(result);
	renderer.SetIcon(&icon);
	renderer.SetScale((result->Bounds().Width() + 1.0) / 64.0);
	renderer.Render();

	// TODO: would be nice to get rid of this
	// (B_RGBA32_PREMULTIPLIED or better yet, new blending_mode)
	// NOTE: probably not necessary only because
	// transparent colors are "black" in all existing icons
	// lighter transparent colors should be too dark if
	// app_server uses correct blending
//	renderer.Demultiply();

	return B_OK;
}

// #pragma mark -

status_t
BIconUtils::GetCMAP8Icon(BNode* node,
						 const char* smallIconAttrName,
						 const char* largeIconAttrName,
						 icon_size size,
						 BBitmap* icon)
{
	// check parameters and initialization
	if (!icon || icon->InitCheck() != B_OK
		|| !node || node->InitCheck() != B_OK
		|| !smallIconAttrName || !largeIconAttrName)
		return B_BAD_VALUE;

	status_t ret = B_OK;
	// set some icon size related variables
	const char *attribute = NULL;
	BRect bounds;
	uint32 attrType = 0;
	size_t attrSize = 0;
	switch (size) {
		case B_MINI_ICON:
			attribute = smallIconAttrName;
			bounds.Set(0, 0, 15, 15);
			attrType = B_MINI_ICON_TYPE;
			attrSize = 16 * 16;
			break;
		case B_LARGE_ICON:
			attribute = largeIconAttrName;
			bounds.Set(0, 0, 31, 31);
			attrType = B_LARGE_ICON_TYPE;
			attrSize = 32 * 32;
			break;
		default:
			ret = B_BAD_VALUE;
			break;
	}

	// TODO: relax this requirement?
	if (icon->Bounds() != bounds)
		return B_BAD_VALUE;

	// get the attribute info and check type and size of the attr contents
	attr_info attrInfo;
	if (ret == B_OK)
		ret = node->GetAttrInfo(attribute, &attrInfo);
	if (ret == B_OK && attrInfo.type != attrType)
		ret = B_BAD_TYPE;
	if (ret == B_OK && attrInfo.size != attrSize)
		ret = B_BAD_DATA;

	// read the attribute
	if (ret == B_OK) {
		bool otherColorSpace = (icon->ColorSpace() != B_CMAP8);
		uint8* buffer = NULL;
		ssize_t read;
		if (otherColorSpace) {
			// other color space than stored in attribute
			buffer = new(nothrow) uint8[attrSize];
			if (!buffer)
				ret = B_NO_MEMORY;
			if (ret == B_OK) {
				read = node->ReadAttr(attribute, attrType, 0, buffer,
									  attrSize);
			}
		} else {
			read = node->ReadAttr(attribute, attrType, 0, icon->Bits(), 
								  attrSize);
		}
		if (ret == B_OK) {
			if (read < 0)
				ret = read;
			else if (read != attrInfo.size)
				ret = B_ERROR;
		}
		if (otherColorSpace) {
			// other color space than stored in attribute
			if (ret == B_OK) {
				ret = ConvertFromCMAP8(buffer,
									   (uint32)size, (uint32)size,
									   (uint32)size, icon);
			}
			delete[] buffer;
		}
	}
	return ret;
}

// #pragma mark -

// ConvertFromCMAP8
status_t
BIconUtils::ConvertFromCMAP8(BBitmap* source, BBitmap* result)
{
	if (!source)
		return B_BAD_VALUE;

	status_t ret = source->InitCheck();
	if (ret < B_OK)
		return ret;

	if (source->ColorSpace() != B_CMAP8)
		return B_BAD_VALUE;

	uint8* src = (uint8*)source->Bits();
	uint32 srcBPR = source->BytesPerRow();
	uint32 width = source->Bounds().IntegerWidth() + 1;
	uint32 height = source->Bounds().IntegerHeight() + 1;

	return ConvertFromCMAP8(src, width, height, srcBPR, result);
}

// ConvertFromCMAP8
status_t
BIconUtils::ConvertFromCMAP8(const uint8* src,
							 uint32 width, uint32 height, uint32 srcBPR,
							 BBitmap* result)
{
	if (!src || !result || srcBPR == 0)
		return B_BAD_VALUE;

	status_t ret = result->InitCheck();
	if (ret < B_OK)
		return ret;

	uint32 dstWidth = result->Bounds().IntegerWidth() + 1;
	uint32 dstHeight = result->Bounds().IntegerHeight() + 1;

	if (dstWidth < width || dstHeight < height) {
		// TODO: down scaling
		return B_ERROR;
	} else if (dstWidth > width || dstHeight > height) {
		// TODO: up scaling
		// (currently copies bitmap into result at left-top)
	}

#if __HAIKU__

	return result->ImportBits(src, height * srcBPR, srcBPR, 0, B_CMAP8);

#else

	if (result->ColorSpace() != B_RGBA32 && result->ColorSpace() != B_RGB32) {
		// TODO: support other color spaces
		return B_BAD_VALUE;
	}

	uint8* dst = (uint8*)result->Bits();
	uint32 dstBPR = result->BytesPerRow();

	const rgb_color* colorMap = system_colors()->color_list;

	for (uint32 y = 0; y < dstHeight; y++) {
		uint32* d = (uint32*)dst;
		const uint8* s = src;
		for (uint32 x = 0; x < dstWidth; x++) {
			const rgb_color c = colorMap[*s];
			uint8 alpha = 255;
			if (*s == B_TRANSPARENT_MAGIC_CMAP8)
				alpha = 0;
			*d = (alpha << 24) | (c.red << 16) | (c.green << 8) | (c.blue);
			s++;
			d++;
		}
		src += srcBPR;
		dst += dstBPR;
	}

	return B_OK;

#endif // __HAIKU__
}

// #pragma mark - forbidden

BIconUtils::BIconUtils() {}
BIconUtils::~BIconUtils() {}
BIconUtils::BIconUtils(const BIconUtils&) {}
BIconUtils& BIconUtils::operator=(const BIconUtils&) { return *this; }

