#include "PeopleTranslator.h"

#include <shared/AutoDeleter.h>
#include "ContactDefs.h"
#include "ContactField.h"
#include <fs_attr.h>
#include <MimeType.h>
#include <Node.h>
#include <NodeInfo.h>
#include <File.h>
#include <ObjectList.h>

#include <new>
#include <stdio.h>
#include <syslog.h>

#include "PeopleView.h"

const char* kTranslatorName = "People Contacts Files";
const char* kTranslatorInfo = "Translator for People files";

#define PEOPLE_MIME_TYPE "application/x-person"
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
		B_PEOPLE_FORMAT ,
		B_TRANSLATOR_CONTACT,
		IN_QUALITY,
		IN_CAPABILITY,
		PEOPLE_MIME_TYPE,
		"People Contact file"
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
		B_PEOPLE_FORMAT ,
		B_TRANSLATOR_CONTACT,
		OUT_QUALITY,
		OUT_CAPABILITY,
		PEOPLE_MIME_TYPE,
		"People Contact file"
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


struct PeopleVisitor : public BContactFieldVisitor {
public:
					PeopleVisitor(BFile* destination)
					:
					fDest(destination)
					{
					}
	virtual		 	~PeopleVisitor()
					{
					}

	virtual void 	Visit(BStringContactField* field)
	{
		BString str;
		switch (field->FieldType()) {
		case B_CONTACT_NAME:
			str << B_PEOPLE_NAME;
		break;
		case B_CONTACT_NICKNAME:
			str << B_PEOPLE_NICKNAME;
		break;
// the Person format is not supporting notes
// maybe it's the case to extend the attributes
//			case B_CONTACT_NOTE:
//		break;
		case B_CONTACT_ORGANIZATION:
			str << B_PEOPLE_COMPANY;
		break;
		case B_CONTACT_EMAIL:
			str << B_PEOPLE_EMAIL;
		break;
		case B_CONTACT_URL:
			str << B_PEOPLE_URL;
		break;
		case B_CONTACT_PHONE:
			str = _TranslatePhone(field);
		break;
/*		case B_CONTACT_PHOTO:

		break;
		case B_CONTACT_GROUP:
					
		break;
		*/
		}		
		_WriteAttribute(str.String(), field->Value()); 
	}

	virtual void 	Visit(BAddressContactField* field)
	{
		if (field->IsWellFormed()) {
			_WriteAttribute(B_PEOPLE_ADDRESS, field->Street());
			_WriteAttribute(B_PEOPLE_CITY, field->City());
			_WriteAttribute(B_PEOPLE_STATE, field->Region());
			_WriteAttribute(B_PEOPLE_ZIP, field->PostalCode());
			_WriteAttribute(B_PEOPLE_COUNTRY, field->Country());
		}
	}

/*
	virtual void 	Visit(BPhotoContactField* field)
	{
		BString str << B_PEOPLE_NAME;
		_WriteAttribute(str, field->Value()); 
	}*/

	void	WriteType()
	{
		BNodeInfo(fDest).SetType(PEOPLE_MIME_TYPE);
	}

private:

	BString	_TranslatePhone(BStringContactField* field)
	{
		BString str;
		switch (field->Usage()) {
			case CONTACT_DATA_OTHER:
			case CONTACT_DATA_HOME:
			case CONTACT_PHONE_MOBILE:
				str.SetTo(B_PEOPLE_HPHONE);
			break;
			case CONTACT_DATA_WORK:
			case CONTACT_PHONE_WORK_MOBILE:
				str.SetTo(B_PEOPLE_WPHONE);
			break;
			case CONTACT_PHONE_FAX_WORK:
			case CONTACT_PHONE_FAX_HOME:
				str.SetTo(B_PEOPLE_FAX);
			break;

			default:
				str.SetTo(B_PEOPLE_HPHONE);
		}
			return str;
	}

			void	_WriteAttribute(const char* attrName, const BString& value)
	{
		if (strlen(attrName) < 1 || value.Length() < 0)
			return;
		fDest->WriteAttr(attrName, B_STRING_TYPE, 0,
		value.String(), value.Length() + 1);
	}
	BFile* fDest;
};


// required by the BaseTranslator class
BTranslator *
make_nth_translator(int32 n, image_id you, uint32 flags, ...)
{
	if (!n)
		return new (std::nothrow) PeopleTranslator();

	return NULL;
}


PeopleTranslator::PeopleTranslator()
	:
	BaseTranslator(kTranslatorName, kTranslatorInfo, PEOPLE_TRANSLATOR_VERSION,
		sInputFormats, kNumInputFormats, sOutputFormats, kNumOutputFormats,
		"PeopleTranslatorSettings", sDefaultSettings, kNumDefaultSettings,
		B_TRANSLATOR_CONTACT, B_PEOPLE_FORMAT)
{
}


PeopleTranslator::~PeopleTranslator()
{
}


status_t
PeopleTranslator::Identify(BPositionIO* inSource,
	const translation_format* inFormat, BMessage* ioExtension,
	translator_info* outInfo, uint32 outType)
{
	if (!outType)
		outType = B_CONTACT_FORMAT;

	if (outType != B_CONTACT_FORMAT && outType != B_PEOPLE_FORMAT)
		return B_NO_TRANSLATOR;

	BMessage msg;
 	if (outType == B_PEOPLE_FORMAT && msg.Unflatten(inSource) == B_OK) {
		outInfo->type = B_CONTACT_FORMAT;
		outInfo->group = B_TRANSLATOR_CONTACT;
		outInfo->quality = IN_QUALITY;
		outInfo->capability = IN_CAPABILITY;
		snprintf(outInfo->name, sizeof(outInfo->name), kTranslatorName);
		strcpy(outInfo->MIME, CONTACT_MIME_TYPE);
		return B_OK;
 	} else if (outType == B_CONTACT_FORMAT)
 		return _IdentifyPeople(inSource, outInfo);

	return B_NO_TRANSLATOR;
}


status_t
PeopleTranslator::Translate(BPositionIO* inSource, const translator_info* info,
	BMessage* ioExtension, uint32 outType, BPositionIO* outDestination)
{
	printf("translate\n");
	if (!outType)
		outType = B_CONTACT_FORMAT;

	if (outType != B_CONTACT_FORMAT && outType != B_PEOPLE_FORMAT)
		return B_NO_TRANSLATOR;

	// add no translation
	BMessage msg;
	if (outType == B_PEOPLE_FORMAT && msg.Unflatten(inSource) == B_OK) {
		if (outDestination == NULL)
			return false;

		BFile* file = _PositionToFile(outDestination);
		if (file != NULL)
			return TranslateContact(&msg, ioExtension, file);
		else
			return B_ERROR;
 	} else if (outType == B_CONTACT_FORMAT)
		return TranslatePeople(inSource, ioExtension, outDestination);

	return B_NO_TRANSLATOR;
}


status_t
PeopleTranslator::TranslateContact(BMessage* inSource, 
		BMessage* ioExtension, BFile* outDestination)
{
	int32 count;
	type_code code = B_CONTACT_FIELD_TYPE;
	PeopleVisitor visitor(outDestination);

	visitor.WriteType();

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
	return B_OK;
}


status_t
PeopleTranslator::TranslatePeople(BPositionIO* inSource, 
		BMessage* ioExtension, BPositionIO* outDestination)
{
	BFile* file = _PositionToFile(inSource);
	if (file != NULL) {
		ObjectDeleter<BAddressContactField> deleter;
		BAddressContactField* addressField
			= new BAddressContactField("", false);
		deleter.SetTo(addressField);

		BMessage msg;
		char buf[B_ATTR_NAME_LENGTH];
		while (file->GetNextAttrName(buf) == B_OK) {
			char* value;
			attr_info info;
			if (file->GetAttrInfo(buf, &info) != B_OK)
				continue;

			value = (char*)calloc(info.size, 1);
			if (value == NULL)
				return B_NO_MEMORY;

			file->ReadAttr(buf, 0, 0, value, info.size);

			// TODO use a std::map here since it's just mapping
			// strings to enum values. Maybe use 2 maps, one for address fields
			// one for string fields.

			BContactField* field = NULL;
			if (strcmp(buf, B_PEOPLE_NAME) == 0) {
				field = new BStringContactField(B_CONTACT_NAME, value);
			} else if (strcmp(buf, B_PEOPLE_NICKNAME) == 0) {
				field = new BStringContactField(B_CONTACT_NICKNAME, value);
			} else if (strcmp(buf, B_PEOPLE_COMPANY) == 0) {
				field = new BStringContactField(B_CONTACT_ORGANIZATION, value);
			} else if (strcmp(buf, B_PEOPLE_ADDRESS) == 0) {
				addressField->SetStreet(value);
			} else if (strcmp(buf, B_PEOPLE_CITY) == 0) {
				addressField->SetCity(value);
			} else if (strcmp(buf, B_PEOPLE_STATE) == 0) {
				addressField->SetRegion(value);
			} else if (strcmp(buf, B_PEOPLE_ZIP) == 0) {
				addressField->SetPostalCode(value);
			} else if (strcmp(buf, B_PEOPLE_COUNTRY) == 0) {
				addressField->SetCountry(value);
			} else if (strcmp(buf, B_PEOPLE_HPHONE) == 0) {
				field = new BStringContactField(B_CONTACT_PHONE, value);
				field->SetUsage(CONTACT_DATA_HOME);
			} else if (strcmp(buf, B_PEOPLE_WPHONE) == 0) {
				field = new BStringContactField(B_CONTACT_PHONE, value);
				field->SetUsage(CONTACT_DATA_WORK);
			} else if (strcmp(buf, B_PEOPLE_FAX) == 0) {
				field = new BStringContactField(B_CONTACT_PHONE, value);
				field->SetUsage(CONTACT_PHONE_FAX_HOME);
			} else if (strcmp(buf, B_PEOPLE_EMAIL) == 0) {
				field = new BStringContactField(B_CONTACT_EMAIL, value);
			} else if (strcmp(buf, B_PEOPLE_URL) == 0) {
				field = new BStringContactField(B_CONTACT_URL, value);
			} else if (strcmp(buf, B_PEOPLE_GROUP) == 0) {
				//
			}

			if (field != NULL)
				_AddField(field, &msg);

			delete field;
			free(value);
		}

		if (addressField->Value().Length() > 0) {
			_AddField(addressField, &msg);
		}

		msg.PrintToStream();
		outDestination->Seek(0, SEEK_SET);
		msg.Flatten(outDestination);
		return B_OK;
	}
	return B_NO_TRANSLATOR;
}


status_t
PeopleTranslator::_IdentifyPeople(BPositionIO* inSource,
	translator_info* outInfo)
{
	BFile* file = _PositionToFile(inSource);
	if (file != NULL) {
		char type[B_MIME_TYPE_LENGTH];
		BNodeInfo info(file);
		if (info.InitCheck() == B_OK) {
			info.GetType(type);
			if (strcmp(type, PEOPLE_MIME_TYPE) == 0) {
				outInfo->type = B_PEOPLE_FORMAT;
				outInfo->group = B_TRANSLATOR_CONTACT;
				outInfo->quality = IN_QUALITY;
				outInfo->capability = IN_CAPABILITY;
				snprintf(outInfo->name,
					sizeof(outInfo->name), kTranslatorName);

				strcpy(outInfo->MIME, PEOPLE_MIME_TYPE);
				return B_OK;
			}
		}
		return B_OK;
	}
	return B_NO_TRANSLATOR;
}


BView*
PeopleTranslator::NewConfigView(TranslatorSettings *settings)
{
	return new PeopleView(BRect(0, 0, 225, 175), 
		"PeopleTranslator Settings", B_FOLLOW_ALL,
		B_WILL_DRAW, settings);
}


BFile*
PeopleTranslator::_PositionToFile(BPositionIO* destination)
{
	BFile* file = dynamic_cast<BFile*>(destination);
	if (file != NULL && file->InitCheck() == B_OK)
		return file;
	else
		return NULL;
}


status_t
PeopleTranslator::_AddField(BContactField* field, BMessage* msg)
{
	if (field != NULL) {
		ssize_t size = field->FlattenedSize();

		void* buffer = new char [size];
		if (buffer == NULL)
			return B_NO_MEMORY;
		MemoryDeleter deleter(buffer);

		status_t ret = field->Flatten(buffer, size);
		if (ret != B_OK)
			return ret;

		ret = msg->AddData(CONTACT_FIELD_IDENT, field->TypeCode(),
			buffer, size, false);
		if (ret != B_OK)
			return ret;

		return B_OK;
	}
	return B_ERROR;
}
