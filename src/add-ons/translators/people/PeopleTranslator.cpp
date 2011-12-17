#include "PeopleTranslator.h"

#include <shared/AutoDeleter.h>
#include <BitmapStream.h>
#include <ContactDefs.h>
#include <ContactField.h>
#include <fs_attr.h>
#include <MimeType.h>
#include <Node.h>
#include <NodeInfo.h>
#include <File.h>
#include <ObjectList.h>
#include <Volume.h>
#include <VolumeRoster.h>

#include <new>
#include <stdio.h>
#include <syslog.h>

#include "PeopleView.h"

#undef B_TRANSLATE_CONTEXT
#define B_TRANSLATE_CONTEXT "People"

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


struct Attribute {
	const char*	attribute;
	int32		width;
	const char*	name;
};

// TODO: Add flags in attribute info message to find these.
//static const char* kNameAttribute = "META:name";
//static const char* kCategoryAttribute = "META:group";

struct Attribute sDefaultAttributes[] = {
	{ B_PEOPLE_NAME, 120, B_TRANSLATE("Contact name") },
	{ B_PEOPLE_NICKNAME, 120, B_TRANSLATE("Nickname") },
	{ B_PEOPLE_COMPANY, 120, B_TRANSLATE("Company") },
	{ B_PEOPLE_ADDRESS, 120, B_TRANSLATE("Address") },
	{ B_PEOPLE_CITY, 90, B_TRANSLATE("City") },
	{ B_PEOPLE_STATE, 50, B_TRANSLATE("State") },
	{ B_PEOPLE_ZIP, 50, B_TRANSLATE("Zip") },
	{ B_PEOPLE_COUNTRY, 120, B_TRANSLATE("Country") },
	{ B_PEOPLE_HPHONE, 90, B_TRANSLATE("Home phone") },
	{ B_PEOPLE_WPHONE, 90, B_TRANSLATE("Work phone") },
	{ B_PEOPLE_FAX, 90, B_TRANSLATE("Fax") },
	{ B_PEOPLE_EMAIL, 120, B_TRANSLATE("E-mail") },
	{ B_PEOPLE_URL, 120, B_TRANSLATE("URL") },
	{ B_PEOPLE_GROUP, 120, B_TRANSLATE("Group") },
	{ NULL, 0, NULL }
};


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

//		case B_CONTACT_GROUP:
					
//		break;

		}		
		_WriteAttribute(str.String(), field->Value()); 
	}

	virtual void 	Visit(BAddressContactField* field)
	{
		_WriteAttribute(B_PEOPLE_ADDRESS, field->Street());
		_WriteAttribute(B_PEOPLE_CITY, field->City());
		_WriteAttribute(B_PEOPLE_STATE, field->Region());
		_WriteAttribute(B_PEOPLE_ZIP, field->PostalCode());
		_WriteAttribute(B_PEOPLE_COUNTRY, field->Country());
	}

	virtual void 	Visit(BPhotoContactField* field)
	{
		BBitmap* picture = field->Photo();
		if (picture) {
			fDest->Seek(SEEK_SET, 0);
			BBitmapStream stream(picture);
			// Detach *our* bitmap from stream to avoid its deletion
			// at stream object destruction
			stream.DetachBitmap(&picture);

			BTranslatorRoster* roster = BTranslatorRoster::Default();
			roster->Translate(&stream, NULL, NULL, fDest,
				field->PictureType(), B_TRANSLATOR_BITMAP,
				NULL);
		}
	}

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
	if (!outType)
		outType = B_CONTACT_FORMAT;

	if (outType != B_CONTACT_FORMAT && outType != B_PEOPLE_FORMAT)
		return B_NO_TRANSLATOR;

	// TODO add no translation
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

		_AddPicture(file, &msg);

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
		//MemoryDeleter deleter(buffer);

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


status_t
PeopleTranslator::_AddPicture(BFile* file, BMessage* msg)
{
	off_t fileSize;
	status_t status = file->GetSize(&fileSize);
		if (status != B_OK)
			return B_ERROR;

		if (fileSize < 1)
			return B_ERROR;

	translator_info info;
	memset(&info, 0, sizeof(translator_info));
	BMessage ioExtension;

	BTranslatorRoster* roster = BTranslatorRoster::Default();

	if (roster == NULL)
		return B_NO_MEMORY;

	status = roster->Identify(file, &ioExtension, &info, 0, NULL,
		B_TRANSLATOR_BITMAP);

	BBitmapStream stream;

	if (status == B_OK) {
		status = roster->Translate(file, &info, &ioExtension, &stream,
			B_TRANSLATOR_BITMAP);
	}
	if (status != B_OK)
		return B_ERROR;

	BBitmap* picture = NULL;
	if (stream.DetachBitmap(&picture) != B_OK
		|| picture == NULL)
		return B_ERROR;

	BPhotoContactField* field = new BPhotoContactField(picture);

	// Remember image format so we could store using the same
	//field->SetMIMEType(info.MIME);
	field->SetPictureType(info.type);

	_AddField(field, msg);
}


// Read attributes from person mime type. If it does not exist,
// or if it contains no attribute definitions, install a "clean"
// person mime type from the hard-coded default attributes.
/*
status_t
PeopleTranslator::_InitAttr()
{
	bool valid = false;
	BMimeType mime(PEOPLE_MIMETYPE);
	if (mime.IsInstalled()) {
		BMessage info;
		if (mime.GetAttrInfo(&info) == B_NO_ERROR) {
			int32 index = 0;
			while (true) {
				int32 type;
				if (info.FindInt32("attr:type", index, &type) != B_OK)
					break;
				bool editable;
				if (info.FindBool("attr:editable", index, &editable) != B_OK)
					break;

				// TODO: Support other types besides string attributes.
				if (type != B_STRING_TYPE || !editable)
					break;

				Attribute* attribute = new Attribute();
				ObjectDeleter<Attribute> deleter(attribute);
				if (info.FindString("attr:public_name", index,
						&attribute->name) != B_OK) {
					break;
				}
				if (info.FindString("attr:name", index,
						&attribute->attribute) != B_OK) {
					break;
				}

				if (!fAttributes.AddItem(attribute))
					break;

				deleter.Detach();
				index++;
			}
		}
		if (fAttributes.CountItems() == 0) {
			valid = false;
			mime.Delete();
		} else
			valid = true;
	}
	if (!valid) {
		mime.Install();
		mime.SetShortDescription(B_TRANSLATE_WITH_CONTEXT("Person",
			"Short mimetype description"));
		mime.SetLongDescription(B_TRANSLATE_WITH_CONTEXT(
			"Contact information for a person.",
			"Long mimetype description"));
		mime.SetIcon(kPersonIcon, sizeof(kPersonIcon));
		mime.SetPreferredApp(APP_SIG);

		// add default person fields to meta-mime type
		BMessage fields;
		for (int32 i = 0; sDefaultAttributes[i].attribute; i++) {
			fields.AddString("attr:public_name", sDefaultAttributes[i].name);
			fields.AddString("attr:name", sDefaultAttributes[i].attribute);
			fields.AddInt32("attr:type", B_STRING_TYPE);
			fields.AddBool("attr:viewable", true);
			fields.AddBool("attr:editable", true);
			fields.AddInt32("attr:width", sDefaultAttributes[i].width);
			fields.AddInt32("attr:alignment", B_ALIGN_LEFT);
			fields.AddBool("attr:extra", false);

			// Add the default attribute to the attribute list, too.
			Attribute* attribute = new Attribute();
			attribute->name = sDefaultAttributes[i].name;
			attribute->attribute = sDefaultAttributes[i].attribute;
			if (!fAttributes.AddItem(attribute))
				delete attribute;
		}

		mime.SetAttrInfo(&fields);
	}

	// create indices on all volumes for the found attributes.

	int32 count = fAttributes.CountItems();
	BVolumeRoster volumeRoster;
	BVolume volume;
	while (volumeRoster.GetNextVolume(&volume) == B_OK) {
		for (int32 i = 0; i < count; i++) {
			Attribute* attribute = fAttributes.ItemAt(i);
			fs_create_index(volume.Device(), attribute->attribute,
				B_STRING_TYPE, 0);
		}
	}
}
*/
