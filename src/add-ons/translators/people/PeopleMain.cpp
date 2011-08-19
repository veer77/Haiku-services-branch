#include <Application.h>

#include "PeopleTranslator.h"

int main()
{
	BApplication app("application/x-vnd.Haiku-PeopleTranslator");
	status_t result;
	result = LaunchTranslatorWindow(new PeopleTranslator,
		"VCard Settings", BRect(0, 0, 225, 175));
	if (result == B_OK) {
		app.Run();
		return 0;
	} else
		return 1;
}

