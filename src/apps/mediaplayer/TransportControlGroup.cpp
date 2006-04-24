/*
 * Copyright 2006, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Stephan Aßmus <superstippi@gmx.de>
 */

// NOTE: Based on my code in the BeOS interface for the VLC media player
// that I did during the VLC 0.4.3 - 0.4.6 times. Code not done by me
// removed. -Stephan Aßmus

#include "TransportControlGroup.h"

#include <stdio.h>
#include <string.h>

#include <String.h>

#include "ButtonBitmaps.h"
#include "TransportButton.h"
#include "VolumeSlider.h"

enum {
	MSG_PLAY				= 'play',
	MSG_STOP				= 'stop',
	MSG_REWIND				= 'rwnd',
	MSG_FORWARD				= 'frwd',
	MSG_SKIP_BACKWARDS		= 'skpb',
	MSG_SKIP_FORWARD		= 'skpf',
	MSG_SET_VOLUME			= 'stvl',
	MSG_SET_MUTE			= 'stmt',
};

#define BORDER_INSET 6.0
#define MIN_SPACE 4.0
#define SPEAKER_SLIDER_DIST 6.0
#define VOLUME_MIN_WIDTH 70.0
#define VOLUME_SLIDER_LAYOUT_WEIGHT 2.0

// constructor
TransportControlGroup::TransportControlGroup(BRect frame)
	: BView(frame, "transport control group",
			B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP,
			B_WILL_DRAW | B_FRAME_EVENTS),
      fBottomControlHeight(0.0)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BRect frame(0.0, 0.0, 10.0, 10.0);
	
    // Buttons
    // Skip Back
    frame.right = kRewindBitmapWidth - 1;
    frame.bottom = kRewindBitmapHeight - 1;
	fBottomControlHeight = kRewindBitmapHeight - 1.0;
    fSkipBack = new TransportButton(frame, B_EMPTY_STRING,
                                    kSkipBackBitmapBits,
                                    kPressedSkipBackBitmapBits,
                                    kDisabledSkipBackBitmapBits,
                                    new BMessage(MSG_SKIP_BACKWARDS));
    AddChild(fSkipBack);

    // Skip Foward
    fSkipForward = new TransportButton(frame, B_EMPTY_STRING,
                                       kSkipForwardBitmapBits,
                                       kPressedSkipForwardBitmapBits,
                                       kDisabledSkipForwardBitmapBits,
                                       new BMessage(MSG_SKIP_FORWARD));
    AddChild(fSkipForward);

	// Forward
	fForward = new TransportButton(frame, B_EMPTY_STRING,
								   kForwardBitmapBits,
								   kPressedForwardBitmapBits,
								   kDisabledForwardBitmapBits,
								   new BMessage(MSG_FORWARD));
	AddChild(fForward);

	// Rewind
	fRewind = new TransportButton(frame, B_EMPTY_STRING,
								  kRewindBitmapBits,
								  kPressedRewindBitmapBits,
								  kDisabledRewindBitmapBits,
								  new BMessage(MSG_REWIND));
	AddChild(fRewind);

	// Play Pause
    frame.right = kPlayPauseBitmapWidth - 1;
    frame.bottom = kPlayPauseBitmapHeight - 1;
	if (fBottomControlHeight < kPlayPauseBitmapHeight - 1.0)
		fBottomControlHeight = kPlayPauseBitmapHeight - 1.0;
    fPlayPause = new PlayPauseButton(frame, B_EMPTY_STRING,
                                     kPlayButtonBitmapBits,
                                     kPressedPlayButtonBitmapBits,
                                     kDisabledPlayButtonBitmapBits,
                                     kPlayingPlayButtonBitmapBits,
                                     kPressedPlayingPlayButtonBitmapBits,
                                     kPausedPlayButtonBitmapBits,
                                     kPressedPausedPlayButtonBitmapBits,
                                     new BMessage(MSG_PLAY));

    AddChild(fPlayPause);

    // Stop
    frame.right = kStopBitmapWidth - 1;
    frame.bottom = kStopBitmapHeight - 1;
	if (fBottomControlHeight < kStopBitmapHeight - 1.0)
		fBottomControlHeight = kStopBitmapHeight - 1.0;
    fStop = new TransportButton(frame, B_EMPTY_STRING,
                                kStopButtonBitmapBits,
                                kPressedStopButtonBitmapBits,
                                kDisabledStopButtonBitmapBits,
                                new BMessage(MSG_STOP));
	AddChild(fStop);

	// Mute
    frame.right = kSpeakerIconBitmapWidth - 1;
    frame.bottom = kSpeakerIconBitmapHeight - 1;
	if (fBottomControlHeight < kSpeakerIconBitmapHeight - 1.0)
		fBottomControlHeight = kSpeakerIconBitmapHeight - 1.0;
    fMute = new TransportButton(frame, B_EMPTY_STRING,
                                kSpeakerIconBits,
                                kPressedSpeakerIconBits,
                                kSpeakerIconBits,
                                new BMessage(MSG_SET_MUTE));

	AddChild(fMute);

    // Volume Slider
	fVolumeSlider = new VolumeSlider(BRect(0.0, 0.0, VOLUME_MIN_WIDTH,
										   kVolumeSliderBitmapHeight - 1.0),
									 "volume slider", 0, 255,
									 new BMessage(MSG_SET_VOLUME));
	fVolumeSlider->SetValue(128);
	AddChild(fVolumeSlider);
}

// destructor
TransportControlGroup::~TransportControlGroup()
{
}

// AttachedToWindow
void
TransportControlGroup::AttachedToWindow()
{
	SetEnabled(EnabledButtons());

	// we are now a valid BHandler
	fVolumeSlider->SetTarget(this);
	fSkipBack->SetTarget(this);
	fSkipForward->SetTarget(this);
	fRewind->SetTarget(this);
	fForward->SetTarget(this);
	fPlayPause->SetTarget(this);
	fStop->SetTarget(this);
	fMute->SetTarget(this);

	FrameResized(Bounds().Width(), Bounds().Height());
}

// FrameResized
void
TransportControlGroup::FrameResized(float width, float height)
{
	// layout controls
	BRect r(Bounds());
	r.InsetBy(BORDER_INSET, BORDER_INSET);
	_LayoutControls(r);
}

// GetPreferredSize
void
TransportControlGroup::GetPreferredSize(float* width, float* height)
{
	BRect r(_MinFrame());

	if (width)
		*width = r.Width();
	if (height)
		*height = r.Height();
}

// MessageReceived
void
TransportControlGroup::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case MSG_PLAY:
			_TogglePlaying();
			break;
		case MSG_STOP:
			_Stop();
			break;

		case MSG_REWIND:
			_Rewind();
			break;
		case MSG_FORWARD:
			_Forward();
			break;

		case MSG_SKIP_BACKWARDS:
			_SkipBackward();
			break;
		case MSG_SKIP_FORWARD:
			_SkipForward();
			break;

		case MSG_SET_VOLUME:
			_SetVolume();
			break;
		case MSG_SET_MUTE:
			_ToggleMute();
			break;

		default:
		    BView::MessageReceived(message);
		    break;
	}
}

// default implementation for the virtuals
uint32 TransportControlGroup::EnabledButtons()		{ return 0; }
void TransportControlGroup::TogglePlaying()			{}
void TransportControlGroup::Stop()					{}
void TransportControlGroup::Rewind()				{}
void TransportControlGroup::Forward()				{}
void TransportControlGroup::SkipBackward()			{}
void TransportControlGroup::SkipForward()			{}
void TransportControlGroup::SetVolume(float value)	{}
void TransportControlGroup::ToggleMute()			{}

// SetEnabled
void
TransportControlGroup::SetEnabled(uint32 buttons)
{
	fVolumeSlider->SetEnabled(buttons & VOLUME_ENABLED);
	fMute->SetEnabled(buttons & VOLUME_ENABLED);

	fSkipBack->SetEnabled(buttons & SKIP_BACK_ENABLED);
	fSkipForward->SetEnabled(buttons & SKIP_FORWARD_ENABLED);
	fRewind->SetEnabled(buttons & SEEK_BACK_ENABLED);
	fForward->SetEnabled(buttons & SEEK_FORWARD_ENABLED);

	fPlayPause->SetEnabled(buttons & PLAYBACK_ENABLED);
	fStop->SetEnabled(buttons & PLAYBACK_ENABLED);
}

// #pragma mark -

// SetPlaybackState
void
TransportControlGroup::SetPlaybackState(uint32 state)
{
	switch (state) {
		case PLAYBACK_STATE_PLAYING:
			fPlayPause->SetPlaying();
			break;
		case PLAYBACK_STATE_PAUSED:
			fPlayPause->SetPaused();
			break;
		case PLAYBACK_STATE_STOPPED:
			fPlayPause->SetStopped();
			break;
	}
}

// SetSkippable
void
TransportControlGroup::SetSkippable(bool backward, bool forward)
{
	fSkipBack->SetEnabled(backward);
	fSkipForward->SetEnabled(forward);
}

// #pragma mark -

// SetAudioEnabled
void
TransportControlGroup::SetAudioEnabled(bool enabled)
{
	fMute->SetEnabled(enabled);
	fVolumeSlider->SetEnabled(enabled);
}

// SetMuted
void
TransportControlGroup::SetMuted(bool mute)
{
	fVolumeSlider->SetMuted(mute);
}

// Volume
uint32
TransportControlGroup::Volume() const
{
	return fVolumeSlider->Value();
}

// #pragma mark -

// _LayoutControls
void
TransportControlGroup::_LayoutControls(BRect frame) const
{
	BRect r(frame);
	// calculate absolutly minimal width
	float minWidth = fSkipBack->Bounds().Width();
	minWidth += fRewind->Bounds().Width();
	minWidth += fStop->Bounds().Width();
	minWidth += fPlayPause->Bounds().Width();
	minWidth += fForward->Bounds().Width();
	minWidth += fSkipForward->Bounds().Width();
	minWidth += fMute->Bounds().Width();
	minWidth += VOLUME_MIN_WIDTH;

	float currentWidth = frame.Width();
	float space = (currentWidth - minWidth) / 6.0;
	// apply weighting
	space = MIN_SPACE + (space - MIN_SPACE) / VOLUME_SLIDER_LAYOUT_WEIGHT;
	// layout controls with "space" inbetween
	r.left = frame.left;
	r.top = r.top + MIN_SPACE;
	r.bottom = frame.bottom;
	// skip back
	r.right = r.left + fSkipBack->Bounds().Width();
	_LayoutControl(fSkipBack, r);
	// rewind
	r.left = r.right + space;
	r.right = r.left + fRewind->Bounds().Width();
	_LayoutControl(fRewind, r);
	// stop
	r.left = r.right + space;
	r.right = r.left + fStop->Bounds().Width();
	_LayoutControl(fStop, r);
	// play/pause
	r.left = r.right + space;
	r.right = r.left + fPlayPause->Bounds().Width();
	_LayoutControl(fPlayPause, r);
	// forward
	r.left = r.right + space;
	r.right = r.left + fForward->Bounds().Width();
	_LayoutControl(fForward, r);
	// skip forward
	r.left = r.right + space;
	r.right = r.left + fSkipForward->Bounds().Width();
	_LayoutControl(fSkipForward, r);
	// speaker icon
	r.left = r.right + space + space;
	r.right = r.left + fMute->Bounds().Width();
	_LayoutControl(fMute, r);
	// volume slider
	r.left = r.right + SPEAKER_SLIDER_DIST;
		// keep speaker icon and volume slider attached
	r.right = frame.right;
	_LayoutControl(fVolumeSlider, r, true);
}

// _MinFrame
BRect           
TransportControlGroup::_MinFrame() const
{
	// add up width of controls along bottom (seek slider will likely adopt)
	float minWidth = 2 * BORDER_INSET;
	minWidth += fSkipBack->Bounds().Width() + MIN_SPACE;
	minWidth += fRewind->Bounds().Width() + MIN_SPACE;
	minWidth += fStop->Bounds().Width() + MIN_SPACE;
	minWidth += fPlayPause->Bounds().Width() + MIN_SPACE;
	minWidth += fForward->Bounds().Width() + MIN_SPACE;
	minWidth += fSkipForward->Bounds().Width() + MIN_SPACE + MIN_SPACE;
	minWidth += fMute->Bounds().Width() + SPEAKER_SLIDER_DIST;
	minWidth += VOLUME_MIN_WIDTH;

	// add up height of seek slider and heighest control on bottom
	float minHeight = 2 * BORDER_INSET;
	minHeight += fBottomControlHeight;
	return BRect(0.0, 0.0, minWidth - 1.0, minHeight - 1.0);
}

// _LayoutControl
void
TransportControlGroup::_LayoutControl(BView* view, BRect frame,
								 bool resizeWidth, bool resizeHeight) const
{
	if (!resizeHeight)
		// center vertically
		frame.top = (frame.top + frame.bottom) / 2.0 - view->Bounds().Height() / 2.0;
	if (!resizeWidth)
		// center horizontally
		frame.left = (frame.left + frame.right) / 2.0 - view->Bounds().Width() / 2.0;
	view->MoveTo(frame.LeftTop());
	float width = resizeWidth ? frame.Width() : view->Bounds().Width();
	float height = resizeHeight ? frame.Height() : view->Bounds().Height();
	if (resizeWidth || resizeHeight)
		view->ResizeTo(width, height);
}

// _TogglePlaying
void
TransportControlGroup::_TogglePlaying()
{
	TogglePlaying();
}

// _Stop
void
TransportControlGroup::_Stop()
{
	fPlayPause->SetStopped();
	Stop();
}

// _Rewind
void
TransportControlGroup::_Rewind()
{
	Rewind();
}

// _Forward
void
TransportControlGroup::_Forward()
{
	Forward();
}

// _SkipBackward
void
TransportControlGroup::_SkipBackward()
{
	SkipBackward();
}

// _SkipForward
void
TransportControlGroup::_SkipForward()
{
	SkipForward();
}

// _SetVolume
void
TransportControlGroup::_SetVolume()
{
	SetVolume((float)(fVolumeSlider->Value() / 255.0));
}

// _ToggleMute
void
TransportControlGroup::_ToggleMute()
{
	fVolumeSlider->SetMuted(!fVolumeSlider->IsMuted());
	ToggleMute();
}


