#ifndef PEOPLE_VIEW_H
#define PEOPLE_VIEW_H

#include <View.h>

#include "TranslatorSettings.h"

class PeopleView : public BView {
	public:
		PeopleView(const BRect& frame, const char* name, uint32 resizeMode,
			uint32 flags, TranslatorSettings* settings);
		virtual ~PeopleView();

	private:
		TranslatorSettings *fSettings;
};

#endif // #ifndef People_VIEW_H
