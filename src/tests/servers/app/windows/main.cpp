// main.cpp

#include <stdio.h>

#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <String.h>
#include <TextControl.h>
#include <TextView.h>
#include <View.h>
#include <Window.h>

class HelloView : public BView {
 public:
					HelloView(BRect frame, const char* name,
							  uint32 resizeFlags, uint32 flags)
						: BView(frame, name, resizeFlags, flags),
						  fTracking(false),
						  fTrackingStart(-1.0, -1.0),
						  fLastMousePos(-1.0, -1.0)
					{
// TODO: Find bug: this influences StringWidth(), but the font is not used
//						SetFontSize(9);
						rgb_color bg = ui_color(B_PANEL_BACKGROUND_COLOR);
						SetViewColor(bg);
						SetLowColor(bg);

						BFont font;
						GetFont(&font);
						BString string("ß amM öo\t");
						int32 charCount = string.CountChars();
						float* escapements = new float[charCount];
						font.GetEscapements(string.String(), charCount, NULL, escapements);
						for (int32 i = 0; i < charCount; i++)
							printf("escapement: %.3f\n", escapements[i]);
						delete[] escapements;
					}

	virtual	void	Draw(BRect updateRect)
					{
						rgb_color shadow = tint_color(LowColor(), B_DARKEN_2_TINT);
						rgb_color light = tint_color(LowColor(), B_LIGHTEN_MAX_TINT);

						BRect r(Bounds());

						BeginLineArray(4);
						AddLine(BPoint(r.left, r.top),
								BPoint(r.right, r.top), shadow);
						AddLine(BPoint(r.right, r.top + 1),
								BPoint(r.right, r.bottom), light);
						AddLine(BPoint(r.right - 1, r.bottom),
								BPoint(r.left, r.bottom), light);
						AddLine(BPoint(r.left, r.bottom - 1),
								BPoint(r.left, r.top + 1), shadow);
						EndLineArray();

						SetHighColor(255, 0, 0, 128);

						const char* message = "Click and drag";
						float width = StringWidth(message);
						BPoint p(r.left + r.Width() / 2.0 - width / 2.0,
								 r.top + r.Height() / 2.0);

						DrawString(message, p);

						message = "to draw a line!";
						width = StringWidth(message);
						p.x = r.left + r.Width() / 2.0 - width / 2.0;
						p.y += 20;

						DrawString(message, p);

//						SetDrawingMode(B_OP_OVER);
SetDrawingMode(B_OP_ALPHA);
SetPenSize(10.0);
						SetHighColor(0, 80, 255, 100);
						StrokeLine(fTrackingStart, fLastMousePos);
// TODO: this should not be necessary with proper state stack
// (a new state is pushed before Draw() is called)
SetDrawingMode(B_OP_COPY);
					}

	virtual	void	MouseDown(BPoint where)
					{
						fTracking = true;
						fTrackingStart = fLastMousePos = where;
						Invalidate();
					}

	virtual	void	MouseUp(BPoint where)
					{
						fTracking = false;
					}
	virtual	void	MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage)
					{
						if (fTracking) {
							fLastMousePos = where;
							Invalidate();
						}
					}
 private:
			bool	fTracking;
			BPoint	fTrackingStart;
			BPoint	fLastMousePos;
};


// show_window
void
show_window(BRect frame, const char* name)
{
//	BRect f(0, 0, frame.Width(), frame.Height());
	BRect f(frame);
	BWindow* window = new BWindow(f, name,
								  B_TITLED_WINDOW,
								  B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE);

	BRect b(window->Bounds());

	b.bottom = b.top + 8;
	BMenuBar* menuBar = new BMenuBar(b, "menu bar");
	window->AddChild(menuBar);

	BMenu* menu = new BMenu("Menus");
	menuBar->AddItem(menu);

	BMenuItem* menuItem = new BMenuItem("Quit", NULL, 'Q');
	menu->AddItem(menuItem);

	menuBar->AddItem(new BMenu("don't"));
	menuBar->AddItem(new BMenu("work!"));
	menuBar->AddItem(new BMenu("(yet)"));

	b = window->Bounds();
	b.top = menuBar->Bounds().bottom + 1;
	BBox* bg = new BBox(b, "box", B_FOLLOW_ALL, B_WILL_DRAW, B_PLAIN_BORDER);

	window->AddChild(bg);

	b = bg->Bounds();
	b.InsetBy(5.0, 5.0);
	BView* view = new HelloView(b, "hello view", B_FOLLOW_ALL,
								B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE);

	bg->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	bg->AddChild(view);

	// button
	b.Set(0.0, 0.0, 80.0, 13.0);
	b.OffsetTo(5.0, 5.0);
	BButton* button = new BButton(b, "button", "Button", NULL);
	view->AddChild(button);
//button->MoveTo(5.0, 5.0);

	// text control
	b.left = button->Frame().left;
	b.top = button->Frame().bottom + 5.0;
	b.bottom = b.top + 35.0;
	b.right += 50.0;
	BTextControl* textControl = new BTextControl(b, "text control", "Text", "<enter text here>", NULL);
	textControl->SetDivider(textControl->StringWidth(textControl->Label()) + 10.0);
	view->AddChild(textControl);

/*	BRect textRect(b);
	textRect.OffsetTo(0.0, 0.0);
	BTextView* textView = new BTextView(b, "text view", textRect,
										B_FOLLOW_ALL, B_WILL_DRAW);
	view->AddChild(textView);
	textView->SetText("This is a BTextView.");
	textView->MakeFocus(true);*/

	// check box
//	b.OffsetTo(5.0, view->Bounds().bottom - (b.Height() + 5.0));
	BCheckBox* checkBox = new BCheckBox(b, "check box", "CheckBox", NULL, B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	view->AddChild(checkBox);
checkBox->MoveTo(5.0, view->Bounds().bottom - (b.Height() + 10.0));

//	window->MoveTo(frame.left, frame.top);
	window->Show();
}

// main
int
main(int argc, char** argv)
{
	BApplication* app = new BApplication("application/x.vnd-Haiku.windows_test");

	BRect frame(50.0, 50.0, 200.0, 250.0);
	show_window(frame, "Window #1");

//	frame.Set(80.0, 100.0, 350.0, 300.0);
//	show_window(frame, "Window #2");

	app->Run();

	delete app;
	return 0;
}
