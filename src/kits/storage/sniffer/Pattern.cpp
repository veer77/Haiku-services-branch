//----------------------------------------------------------------------
//  This software is part of the OpenBeOS distribution and is covered 
//  by the OpenBeOS license.
//---------------------------------------------------------------------
/*!
	\file Pattern.cpp
	MIME sniffer pattern implementation
*/

#include <sniffer/Err.h>
#include <sniffer/Pattern.h>
#include <DataIO.h>
#include <stdio.h>	// for SEEK_* defines

using namespace Sniffer;

Pattern::Pattern(const char *string, const char *mask = NULL)
	: fCStatus(B_NO_INIT)
	, fErrorMessage(NULL)
{
	SetTo(string, mask);
}

Pattern::~Pattern() {
	delete fErrorMessage;
}

status_t
Pattern::InitCheck() const {
	return fCStatus;
}

Err*
Pattern::GetErr() const {
	if (fCStatus == B_OK)
		return NULL;
	else
		return new(nothrow) Err(*fErrorMessage);
//	return fErrorMessage ? NULL : new Err(*fErrorMessage);
//	return new Err("HEY, FIX THIS CRAP!", -1);
}

status_t
Pattern::SetTo(const char *string, const char *mask) {
	if (string) {
		fString = string;
		if (fString.length() == 0) {
			SetStatus(B_BAD_VALUE, "Sniffer pattern error: illegal empty pattern");		
		} else {
			if (mask) {
				fMask = mask;
				if (fString.length() != fMask.length()) {
					SetStatus(B_BAD_VALUE, "Sniffer pattern error: pattern and mask lengths do not match");
				} else {
					SetStatus(B_OK);
				}		
			} else {
				fMask = "";
				for (int i = 0; i < fString.length(); i++)
					fMask += (char)0xFF;
				SetStatus(B_OK);		
			}
		}
	} else {
		SetStatus(B_BAD_VALUE, "Sniffer parser error: NULL string parameter passed to Pattern::SetTo()");
	}
}

bool
Pattern::Sniff(Range range, BPositionIO *data) const {
	// If our range contains negative values relative to the end of
	// the file, convert them to positive values relative to the
	// beginning of the file.
	int32 start = range.Start();
	int32 end = range.End();
	off_t size = data->Seek(0, SEEK_END);
	if (end >= size)
		end = size-1;	
	for (int i = start; i <= end; i++) {
		if (Sniff(i, size, data))
			return true;
	}
}

// Assumes the BPositionIO object is in the correct
// position from which to sniff
bool
Pattern::Sniff(off_t start, off_t size, BPositionIO *data) const {
	off_t len = fString.length();
	char *buffer = new(nothrow) char[len+1];
	if (buffer) {
		ssize_t bytesRead = data->ReadAt(start, buffer, len);
		// /todo If there are fewer bytes left in the data stream
		// from the given position than the length of our data
		// string, should we just return false (which is what we're
		// doing now), or should we compare as many bytes as we
		// can and return true if those match?
		if (bytesRead < len)
			return false;
		else {
			bool result = true;
			for (int i = 0; i < len; i++) {
				if ((fString[i] & fMask[i]) != (buffer[i] & fMask[i])) {
					result = false;
					break;
				}
			}
			return result;
		}	
	} else
		return false;
}

void
Pattern::SetStatus(status_t status, const char *msg) {
	fCStatus = status;
	if (status == B_OK)
		SetErrorMessage(NULL);
	else {
		if (msg)
			SetErrorMessage(msg);
		else {
			SetErrorMessage("Sniffer parser error: Pattern::SetStatus() -- NULL msg with non-B_OK status.\n"
				"(This is officially the most helpful error message you will ever receive ;-)");
		}
	}
}

void
Pattern::SetErrorMessage(const char *msg) {
	delete fErrorMessage;
	fErrorMessage = (msg) ? (new(nothrow) Err(msg, -1)) : (NULL);
}
