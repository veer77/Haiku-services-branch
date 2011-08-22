#include "VCardTranslator.h"

#include <shared/AutoDeleter.h>
#include <ObjectList.h>

#include <new>
#include <stdio.h>
#include <syslog.h>

#include "VCardParserDefs.h"
#include "VCardParser.h"


const char* kTranslatorName = "VCard Files";
const char* kTranslatorInfo = "Translator for VCard";

#define VCARD_MIME_TYPE "text/x-vCard"
#define CONTACT_MIME_TYPE "application/x-hcontact"

static const translation_format sInputFormats[] = {
		{
		B_CONTACT_FORMAT,
		B_TRANSLATOR_CONTACT,
		IN_QUALITY,
		IN_CAPABILITY,
		CONTACT_MIME_TYPE,
		"Haiku binary contact"
	},
	{
		B_VCARD_FORMAT ,
		B_TRANSLATOR_CONTACT,
		IN_QUALITY,
		IN_CAPABILITY,
		VCARD_MIME_TYPE,
		"vCard Contact file"
	}
};

// The output formats that this translator supports.
static const translation_format sOutputFormats[] = {
		{
		B_CONTACT_FORMAT,
		B_TRANSLATOR_CONTACT,
		OUT_QUALITY,
		OUT_CAPABILITY,
		CONTACT_MIME_TYPE,
		"Haiku binary contact"
	},
	{
		B_VCARD_FORMAT ,
		B_TRANSLATOR_CONTACT,
		OUT_QUALITY,
		OUT_CAPABILITY,
		VCARD_MIME_TYPE,
		"vCard Contact file"
	}
};

// Default settings for the Translator
static const TranSetting sDefaultSettings[] = {
};

const uint32 kNumInputFormats = sizeof(sInputFormats) 
	/ sizeof(translation_format);
const uint32 kNumOutputFormats = sizeof(sOutputFormats)
	/ sizeof(translation_format);
const uint32 kNumDefaultSettings = sizeof(sDefaultSettings)
	/ sizeof(TranSetting);


struct VCardVisitor : public BContactFieldVisitor {
public:
					VCardVisitor(BPositionIO* destination)
					:
					fDest(destination)
					{
					}
	virtual		 	~VCardVisitor()
					{
					}

	void			WriteBegin()
	{
		fDest->Seek(0, SEEK_SET);
		const char data[] = "BEGIN:VCARD\nVERSION:2.1\n";
		fDest->Write(data, strlen(data));
	}

	void			WriteEnd()
	{
		// TODO VC 3.0 support
		const char data[] = "END:VCARD\n";
		fDest->Write(data, strlen(data));	
	}

	virtual void 	Visit(BStringContactField* field)
	{
		BString str;
		switch (field->FieldType()) {
			case B_CONTACT_NAME:
				str << VCARD_FORMATTED_NAME;
			break;
			case B_CONTACT_NICKNAME:
				str << VCARD_NICKNAME;
			break;
			case B_CONTACT_NOTE:
				str << VCARD_NOTE;
			break;
			case B_CONTACT_ORGANIZATION:
				str << VCARD_ORGANIZATION;
			break;
			case B_CONTACT_EMAIL:
				str << VCARD_EMAIL;
				switch (field->Usage()) {
					case CONTACT_DATA_HOME:
					str << ";HOME";
					case CONTACT_DATA_WORK:
					str << ";WORK";					
					case CONTACT_EMAIL_MOBILE:
					str << ";MOBILE";
				}
			break;
			case B_CONTACT_URL:
				str << VCARD_URL;
			break;
			case B_CONTACT_PHONE:
				str << VCARD_TELEPHONE;
				switch (field->Usage()) {
					case CONTACT_DATA_HOME:
					str << ";HOME";
					break;
					case CONTACT_DATA_WORK:
					str << ";WORK";
					break;
					case CONTACT_PHONE_MOBILE:
					str << ";MOBILE";
					break;
					case CONTACT_PHONE_FAX_WORK:
					str << ";WORK;FAX";
					break;
					case CONTACT_PHONE_FAX_HOME:
					str << ";WORK;FAX";
					break;
					/*case CONTACT_DATA_OTHER:
					
					break;*/
					case CONTACT_PHONE_PAGER:
					case CONTACT_PHONE_CALLBACK:
					case CONTACT_PHONE_CAR:
					case CONTACT_PHONE_ORG_MAIN:
					case CONTACT_PHONE_ISDN:
					case CONTACT_PHONE_MAIN:
					case CONTACT_PHONE_RADIO:
					case CONTACT_PHONE_TELEX:
					case CONTACT_PHONE_TTY_TDD:
					case CONTACT_PHONE_WORK_MOBILE:
					case CONTACT_PHONE_WORK_PAGER:
					case CONTACT_PHONE_ASSISTANT:
					case CONTACT_PHONE_MMS:
					break;
				}
			break;
			case B_CONTACT_ADDRESS:
				str << VCARD_ADDRESS;
			break;
			/*case B_CONTACT_PHOTO:
			
			break;*/
		}
		if (str.Length() > 0) {
			str << ":" << field->Value() << "\n";
			fDest->Write(str.String(), str.Length());
		}
	}

	virtual void 	Visit(BAddressContactField* field)
	{
		if(field->FieldType() == B_CONTACT_ADDRESS) {
			BString str(VCARD_ADDRESS);
			str << ":" << field->Value() << "\n";
			fDest->Write(str.String(), str.Length());
		}
	}

	virtual void 	Visit(BPhotoContactField* field)
	{
	
	}

	/*virtual void 		Visit(BUrlContactField* field)
	{
		if(field->FieldType() == B_CONTACT_URL) {
			BString str(VCARD_URL);
			str << ":";
			str << field->Value();
			str << "\n";
			fDest->Write(str.String(), str.Length());
		}
	}*/

private:
	BPositionIO* fDest;
};


// required by the BaseTranslator class
BTranslator *
make_nth_translator(int32 n, image_id you, uint32 flags, ...)
{
	if (!n)
		return new (std::nothrow) VCardTranslator();

	return NULL;
}


VCardTranslator::VCardTranslator()
	:
	BaseTranslator(kTranslatorName, kTranslatorInfo, VCARD_TRANSLATOR_VERSION,
		sInputFormats, kNumInputFormats, sOutputFormats, kNumOutputFormats,
		"VCardTranslatorSettings", sDefaultSettings, kNumDefaultSettings,
		B_TRANSLATOR_CONTACT, B_VCARD_FORMAT)
{
}


VCardTranslator::~VCardTranslator()
{
}


status_t
VCardTranslator::Identify(BPositionIO* inSource,
	const translation_format* inFormat, BMessage* ioExtension,
	translator_info* outInfo, uint32 outType)
{
	if (!outType)
		outType = B_CONTACT_FORMAT;

	if (outType != B_CONTACT_FORMAT && outType != B_VCARD_FORMAT)
		return B_NO_TRANSLATOR;

	BMessage msg;
 	if (outType == B_VCARD_FORMAT && msg.Unflatten(inSource) == B_OK) {
 		msg.PrintToStream();
		outInfo->type = B_CONTACT_FORMAT;
		outInfo->group = B_TRANSLATOR_CONTACT;
		outInfo->quality = IN_QUALITY;
		outInfo->capability = IN_CAPABILITY;
		snprintf(outInfo->name, sizeof(outInfo->name), kTranslatorName);
		strcpy(outInfo->MIME, CONTACT_MIME_TYPE);
		return B_OK;
 	} else if (outType == B_CONTACT_FORMAT)
 		return _IdentifyVCard(inSource, outInfo);

	return B_NO_TRANSLATOR;
}


status_t
VCardTranslator::Translate(BPositionIO* inSource, const translator_info* info,
	BMessage* ioExtension, uint32 outType, BPositionIO* outDestination)
{
	if (!outType)
		outType = B_CONTACT_FORMAT;

	if (outType != B_CONTACT_FORMAT && outType != B_VCARD_FORMAT)
		return B_NO_TRANSLATOR;

	// add no translation
	BMessage msg;
 	if (outType == B_VCARD_FORMAT && msg.Unflatten(inSource) == B_OK) {
		return TranslateContact(&msg, ioExtension, outDestination);
 	} else if (outType == B_CONTACT_FORMAT)
		return TranslateVCard(inSource, ioExtension, outDestination);

	return B_ERROR;
}


status_t
VCardTranslator::TranslateContact(BMessage* inSource, 
		BMessage* ioExtension, BPositionIO* outDestination)
{
	int32 count;
	type_code code = B_CONTACT_FIELD_TYPE;

	VCardVisitor visitor(outDestination);

	visitor.WriteBegin();

	status_t ret = inSource->GetInfo(CONTACT_FIELD_IDENT, &code, &count);
	if (ret != B_OK)
		return ret;

	for (int i = 0; i < count; i++) {
		const void* data;
		ssize_t size;

		ret = inSource->FindData(CONTACT_FIELD_IDENT, code,
			i, &data, &size);

		BContactField* field = BContactField::UnflattenChildClass(data, size);
		if (field == NULL)
			return B_ERROR;

		field->Accept(&visitor);

		delete field;
	}
	visitor.WriteEnd();
	return B_OK;
}


status_t
VCardTranslator::TranslateVCard(BPositionIO* inSource, 
		BMessage* ioExtension, BPositionIO* outDestination)
{
	VCardParser parser(inSource);

	if (parser.Parse() != B_OK)
		return B_ERROR;

	BObjectList<BContactField>* list = parser.Properties();

	BMessage msg;
	int count = list->CountItems();
	for (int32 i = 0; i < count; i++) {
		BContactField* object = list->ItemAt(i);
		ssize_t size = object->FlattenedSize();
		void* buffer = new char[size];
		if (buffer == NULL)
			return B_NO_MEMORY;
		MemoryDeleter deleter(buffer);

		status_t ret = object->Flatten(buffer, size);
		if (ret != B_OK)
			return ret;

		ret = msg.AddData(CONTACT_FIELD_IDENT,
			B_CONTACT_FIELD_TYPE, buffer, size, false);
	}

	outDestination->Seek(0, SEEK_SET);
	return msg.Flatten(outDestination);
}


status_t
VCardTranslator::_IdentifyVCard(BPositionIO* inSource,
	translator_info* outInfo)
{
	// Using this initialization
	// the parse will only verify
	// that it's a VCard file
	// without saving any property
	VCardParser parser(inSource, true);
	
	if (parser.Parse() != B_OK)
		return B_ERROR;

	if (parser.HasBegin() && parser.HasEnd()) {
		outInfo->type = B_VCARD_FORMAT;
		outInfo->group = B_TRANSLATOR_CONTACT;
		outInfo->quality = IN_QUALITY;
		outInfo->capability = IN_CAPABILITY;
		snprintf(outInfo->name, sizeof(outInfo->name), kTranslatorName);
		strcpy(outInfo->MIME, VCARD_MIME_TYPE);
		return B_OK;
	}
	return B_NO_TRANSLATOR;
}


BView*
VCardTranslator::NewConfigView(TranslatorSettings *settings)
{
	return 	new VCardView(BRect(0, 0, 225, 175), 
		"VCardTranslator Settings", B_FOLLOW_ALL,
		B_WILL_DRAW, settings);
}
