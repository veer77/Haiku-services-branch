/*
 * Copyright 2011 Haiku Inc.
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef _CONTACTFIELD_H_
#define _CONTACTFIELD_H_

#include <Bitmap.h>
#include <ContactDefs.h>
#include <Flattenable.h>
#include <Message.h>
#include <String.h>
#include <ObjectList.h>

#define CONTACT_FIELD_IDENT "contactfield"


enum {
	B_CONTACT_FIELD_TYPE = 'CNFT'
};

class BAddressContactField;
class BContactFieldVisitor;
class BPhotoContactField;
class BStringContactField;

class BContactField : public virtual BFlattenable {
public:
							BContactField(type_code type);
	virtual		 			~BContactField();

	virtual void			SetValue(const BString& value) = 0;
	virtual const BString&	Value() const = 0;

	virtual status_t		CopyDataFrom(BContactField* field);

			int32			Usage() const;
			void			SetUsage(int32 usage);

	virtual void			Accept(BContactFieldVisitor* visitor) = 0;
	virtual bool			IsEqual(BContactField* field) = 0;
			type_code		FieldType() const;

			// this provide a human-friendly
			// description of the field
			const BString&	Label() const;
			void			SetLabel(const BString& label);

			// these functions will be removed probably
			bool			AddParameter(const BString& property);

			bool			AddParameterAt(const BString& property,
								int32 index);
			bool			ReplaceParameter(const BString& property,
								int32 index);

			status_t		RemoveParameter(const BString& property);
			const BString&	ParameterAt(int32 i) const;
			int32			CountParameters() const;

	virtual	bool			IsFixedSize() const;
	virtual	type_code		TypeCode() const;
	virtual	bool			AllowsTypeCode(type_code code) const;
	virtual	ssize_t			FlattenedSize() const;

			status_t 		Flatten(BPositionIO* flatData) const;
	virtual	status_t		Flatten(void* buffer, ssize_t size) const;
	virtual	status_t		Unflatten(type_code code, const void* buffer,
								ssize_t size);
			status_t		Unflatten(type_code code, BPositionIO* flatData);

	static	BContactField*	UnflattenChildClass(const void* data,
								ssize_t size);
	static	BContactField*	Duplicate(BContactField* from);
protected:
			ssize_t			_AddStringToBuffer(BPositionIO* buffer,
								const BString& str) const;
			BString			_ReadStringFromBuffer(BPositionIO* buffer,
								ssize_t len = -1);

			BString			fLabel;
			type_code 		fType;
			int32			fUsage;

			BObjectList<BString> fParamList;
};


class BContactFieldVisitor {
public:
	virtual void 			Visit(BStringContactField* field) = 0;
	virtual void 			Visit(BAddressContactField* field) = 0;
	virtual void 			Visit(BPhotoContactField* field) = 0;
};

typedef BObjectList<BContactField> BContactFieldList;

/*** Contact Fields implementations ***/

class BStringContactField : public BContactField {
public:
							BStringContactField(type_code type,
								const BString& str);

							BStringContactField(type_code type,
								const char* str = "");

	virtual					~BStringContactField();

	virtual void 			Accept(BContactFieldVisitor* v);

	virtual bool 			IsEqual(BContactField* field);
	virtual void			SetValue(const BString& value);
	virtual const BString&	Value() const;

			void			SetUsage(int32 usage);

	virtual status_t		CopyDataFrom(BContactField* field);

	virtual	ssize_t			FlattenedSize() const;
	virtual	status_t		Flatten(void* buffer, ssize_t size) const;
	virtual	status_t		Unflatten(type_code code, const void* buffer,
								ssize_t size);
protected:
			void			_InitLabel();

			struct			EqualityVisitor;
			BString			fValue;
};

// TODO move code into a BAddress object
class BAddressContactField : public BContactField {
public:
//							BAddressContactField(BAddress* address);
							BAddressContactField(BString address = "",
								bool wellFormed = true);
	virtual					~BAddressContactField();

			void			Accept(BContactFieldVisitor* v);
			bool			IsEqual(BContactField* field);

			bool			IsWellFormed() const;

	// this return a formatted address
	virtual void			SetValue(const BString& value) ;
	virtual const BString&	Value() const;

			const BString&	Street() const;
			const BString&	PostalBox() const;
			const BString&	Neighborhood() const;
			const BString&	City() const;
			const BString&	Region() const;
			const BString&	PostalCode() const;
			const BString&	Country() const;

			void			SetStreet(const BString& street);
			void			SetPostalBox(const BString& postBox);
			void			SetNeighborhood(const BString& neighbor);
			void			SetCity(const BString& city);
			void			SetRegion(const BString& region);
			void			SetPostalCode(const BString& postalCode);
			void			SetCountry(const BString& country);

	virtual status_t		CopyDataFrom(BContactField* field);

	virtual	ssize_t			FlattenedSize() const;
	virtual	status_t		Flatten(void* buffer, ssize_t size) const;
	virtual	status_t		Unflatten(type_code code, const void* buffer,
								ssize_t size);
private:
			bool			_SplitValue(const BString& str);
			void			_PopValue(BString& str, BString& value);

			BString			fStreet;
			BString			fPostalBox;
			BString			fNeighbor;
			BString			fCity;
			BString			fRegion;
			BString			fPostalCode;
			BString			fCountry;
			bool			fWellFormed;

	mutable BString			fValue;
			struct 			EqualityVisitor;
};


class BPhotoContactField : public virtual BContactField {
public:
							BPhotoContactField(BBitmap* bitmap = NULL);
	virtual					~BPhotoContactField();

	virtual	void			Accept(BContactFieldVisitor* v);
	virtual	bool			IsEqual(BContactField* field);

			BBitmap*		Photo() const;
			void			SetPhoto(BBitmap* photo);
/*
			const entry_ref& RefToPhoto();
			void			SetRefToPhoto(const entry_ref& ref);
*/

	virtual void			SetValue(const BString& value) ;
	virtual const BString&	Value() const;

	virtual status_t		CopyDataFrom(BContactField* field);

	virtual	ssize_t			FlattenedSize() const;
	virtual	status_t		Flatten(void* buffer, ssize_t size) const;
	virtual	status_t		Unflatten(type_code code, const void* buffer,
								ssize_t size);

			uint32			PictureType() const;
			void			SetPictureType(uint32 type);
protected:
			void			_InitLabel();

			struct 			EqualityVisitor;
			BBitmap*		fBitmap;
			BString			fUrl;
			int32			fPhotoType;

			entry_ref* 		fEntry;
			uint32			fPictureType;
};

/*
ATM i don't think it's needed, so i'm using
the BStringContactField for the phone number. 

class BPhoneContactField : public virtual BStringContactField {
public:
							BPhoneContactField(int phone);
							BPhoneContactField(const BString& phone);

	virtual					~BPhoneContactField();

	virtual	void			Accept(BContactFieldVisitor* v);
	virtual	bool			IsEqual(BContactField* field);

			int				Phone() const;
			void			SetPhone(int phone);

	virtual void			SetValue(const BString& value) ;
	virtual const BString&	Value() const;

	virtual	ssize_t			FlattenedSize() const;
	virtual	status_t		Flatten(void* buffer, ssize_t size) const;
	virtual	status_t		Unflatten(type_code code, const void* buffer,
								ssize_t size);
protected:
			struct 			EqualityVisitor;
			BString			fPhone;
};


this will be a special type of field
that will provide a method to define
custom contact fields presumably
using a BMessage

class BCustomContactField : public virtual BContactField {
public:
							BCustomContactField(BMessage* message,
								const char* type);
	virtual					~BCustomContactField();

			void			Accept(BContactFieldVisitor* v);
			bool			IsEqual(BContactField* field);

			BMessage*		AsTemplate();

	virtual void			SetValue(const BString& value) ;
	virtual const BString&	Value() const;

	virtual	ssize_t			FlattenedSize() const;
	virtual	status_t		Flatten(void* buffer, ssize_t size) const;
	virtual	status_t		Unflatten(type_code code, const void* buffer,
								ssize_t size);
private:
			struct 			EqualityVisitor;
};

*/
#endif	// _CONTACTFIELD_H_
