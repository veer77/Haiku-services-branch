//****************************************************************************************
//
//	File:		NormalPulseView.cpp
//
//	Written by:	Daniel Switkin
//
//	Copyright 1999, Be Incorporated
//
//****************************************************************************************


#include "NormalPulseView.h"
#include "Common.h"
#include "Pictures"

#include <Bitmap.h>
#include <Dragger.h>
#include <Window.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <cpu_type.h>


float
max_font_size(BFont font, const char* text, float maxSize, float maxWidth)
{
	const float steps = 0.5f;

	for (float size = maxSize; size > 4; size -= steps) {
		font.SetSize(size);
		if (font.StringWidth(text) <= maxWidth)
			return size;
	}

	return 4;
}

	
//	#pragma mark -


NormalPulseView::NormalPulseView(BRect rect)
	: PulseView(rect, "NormalPulseView"),
	fHasBrandLogo(false)
{
	rgb_color color = { 168, 168, 168, 0xff };
	SetViewColor(color);
	SetLowColor(color);

	mode1->SetLabel("Mini Mode");
	mode1->SetMessage(new BMessage(PV_MINI_MODE));
	mode2->SetLabel("Deskbar Mode");
	mode2->SetMessage(new BMessage(PV_DESKBAR_MODE));
	
	DetermineVendorAndProcessor();

	// Allocate progress bars and button pointers
	system_info systemInfo;
	get_system_info(&systemInfo);
	fCpuCount = systemInfo.cpu_count;
	fProgressBars = new ProgressBar *[fCpuCount];
	fCpuButtons = new CPUButton *[fCpuCount];

	// Set up the CPU activity bars and buttons
	for (int x = 0; x < fCpuCount; x++) {
		BRect r(PROGRESS_MLEFT, PROGRESS_MTOP + ITEM_OFFSET * x,
			PROGRESS_MLEFT + ProgressBar::PROGRESS_WIDTH,
			PROGRESS_MTOP + ITEM_OFFSET * x + ProgressBar::PROGRESS_HEIGHT);
		fProgressBars[x] = new ProgressBar(r, "CPU Progress Bar");
		AddChild(fProgressBars[x]);

		r.Set(CPUBUTTON_MLEFT, CPUBUTTON_MTOP + ITEM_OFFSET * x,
			CPUBUTTON_MLEFT + CPUBUTTON_WIDTH,
			CPUBUTTON_MTOP + ITEM_OFFSET * x + CPUBUTTON_HEIGHT);
		char temp[4];
		sprintf(temp, "%d", x + 1);
		fCpuButtons[x] = new CPUButton(r, "CPUButton", temp, NULL);
		AddChild(fCpuButtons[x]);

		//	If there is only 1 cpu it will be hidden below
		//	thus, no need to add the dragger as it will still
		//	be visible when replicants are turned on
		if (fCpuCount > 1) {
			BRect dragger_rect;
			dragger_rect = r;
			dragger_rect.top = dragger_rect.bottom;
			dragger_rect.left = dragger_rect.right;
			dragger_rect.bottom += 7;
			dragger_rect.right += 7;
			dragger_rect.OffsetBy(-1, -1);
			BDragger *dragger = new BDragger(dragger_rect, fCpuButtons[x], 0);
			AddChild(dragger);
		}
	}

	if (fCpuCount == 1) {
		fProgressBars[0]->MoveBy(-3, 12);
		fCpuButtons[0]->Hide();
	}
}


NormalPulseView::~NormalPulseView()
{
	delete fCpuLogo;
	delete[] fCpuButtons;
	delete[] fProgressBars;
}


void
NormalPulseView::CalculateFontSize()
{
	BFont font;
	GetFont(&font);

	fProcessorFontSize = max_font_size(font, fProcessor, 11.0f, 46.0f);

	if (!fHasBrandLogo)
		fVendorFontSize = max_font_size(font, fVendor, 13.0f, 46.0f);
}


int
NormalPulseView::CalculateCPUSpeed()
{
	system_info sys_info;
	get_system_info(&sys_info);

	int target = sys_info.cpu_clock_speed / 1000000;
	int frac = target % 100;
	int delta = -frac;
	int at = 0;
	int freqs[] = { 100, 50, 25, 75, 33, 67, 20, 40, 60, 80, 10, 30, 70, 90 };

	for (uint x = 0; x < sizeof(freqs) / sizeof(freqs[0]); x++) {
		int ndelta = freqs[x] - frac;
		if (abs(ndelta) < abs(delta)) {
			at = freqs[x];
			delta = ndelta;
		}
	}
	return target + delta;
}


void
NormalPulseView::DetermineVendorAndProcessor()
{
	system_info sys_info;
	get_system_info(&sys_info);

	// Initialize logos

	fCpuLogo = new BBitmap(BRect(0, 0, 63, 62), B_COLOR_8_BIT);

#if __POWERPC__
	fCpuLogo->SetBits(Anim1, fCpuLogo->BitsLength(), 0, B_COLOR_8_BIT);
#endif
#if __INTEL__
	if ((sys_info.cpu_type & B_CPU_x86_VENDOR_MASK) == B_CPU_INTEL_x86) {
		fCpuLogo->SetBits(IntelLogo, fCpuLogo->BitsLength(), 0, B_COLOR_8_BIT);
		fHasBrandLogo = true;
	} else
		fCpuLogo->SetBits(BlankLogo, fCpuLogo->BitsLength(), 0, B_COLOR_8_BIT);
#endif

	get_cpu_type(fVendor, sizeof(fVendor), fProcessor, sizeof(fProcessor));
}


void
NormalPulseView::Draw(BRect rect)
{
	PushState();

	// Black frame
	SetHighColor(0, 0, 0);
	BRect frame = Bounds();
	frame.right--;
	frame.bottom--;
	StrokeRect(frame);

	// Bevelled edges
	SetHighColor(255, 255, 255);
	StrokeLine(BPoint(1, 1), BPoint(frame.right - 1, 1));
	StrokeLine(BPoint(1, 1), BPoint(1, frame.bottom - 1));
	SetHighColor(80, 80, 80);
	StrokeLine(BPoint(frame.right, 1), BPoint(frame.right, frame.bottom));
	StrokeLine(BPoint(2, frame.bottom), BPoint(frame.right - 1, frame.bottom));

	// Dividing line
	SetHighColor(96, 96, 96);
	StrokeLine(BPoint(1, frame.bottom + 1), BPoint(frame.right, frame.bottom + 1));
	SetHighColor(255, 255, 255);
	StrokeLine(BPoint(1, frame.bottom + 2), BPoint(frame.right, frame.bottom + 2));
	
	// Processor picture
	DrawBitmap(fCpuLogo, BPoint(10, 10));

#if __INTEL__
	// Do nothing in the case of non-Intel CPUs - they already have a logo
	if (!fHasBrandLogo) {
		SetDrawingMode(B_OP_OVER);
		SetHighColor(240, 240, 240);
		SetFontSize(fVendorFontSize);

		float width = StringWidth(fVendor);
		MovePenTo(10 + (32 - width / 2), 30);
		DrawString(fVendor);
	}
#endif

	// Draw processor type and speed
	char buf[500];
	sprintf(buf, "%d MHz", CalculateCPUSpeed());
	SetDrawingMode(B_OP_OVER);
	SetHighColor(240, 240, 240);
	SetFontSize(fProcessorFontSize);

	float width = StringWidth(fProcessor);
	MovePenTo(10 + (32 - width / 2), 48);
	DrawString(fProcessor);

	width = StringWidth(buf);
	MovePenTo(10 + (32 - width / 2), 60);
	DrawString(buf);
	
	PopState();
}


void
NormalPulseView::Pulse()
{
	// Don't recalculate and redraw if this view is hidden
	if (!IsHidden()) {
		Update();
		if (Window()->Lock()) {
			// Set the value of each CPU bar
			for (int x = 0; x < fCpuCount; x++) {
				fProgressBars[x]->Set(max_c(0, cpu_times[x] * 100));
			}

			Sync();
			Window()->Unlock();
		}
	}
}


void
NormalPulseView::AttachedToWindow()
{
	SetFont(be_bold_font);
	CalculateFontSize();

	fPreviousTime = system_time();
	
	BMessenger messenger(Window());
	mode1->SetTarget(messenger);
	mode2->SetTarget(messenger);
	preferences->SetTarget(messenger);
	about->SetTarget(messenger);
	
	system_info sys_info;
	get_system_info(&sys_info);
	if (sys_info.cpu_count >= 2) {
		for (int x = 0; x < sys_info.cpu_count; x++) {
			cpu_menu_items[x]->SetTarget(messenger);
		}
	}
}


void
NormalPulseView::UpdateColors(BMessage *message)
{
	int32 color = message->FindInt32("color");
	bool fade = message->FindBool("fade");
	system_info sys_info;
	get_system_info(&sys_info);
	
	for (int x = 0; x < sys_info.cpu_count; x++) {
		fProgressBars[x]->UpdateColors(color, fade);
		fCpuButtons[x]->UpdateColors(color);
	}
}

