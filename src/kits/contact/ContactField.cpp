/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include <ContactField.h>
// it is only before the code
// gets integrated into the tree
#include <shared/AutoDeleter.h>
#include <ContactDefs.h>
#include <DataIO.h>
#include <String.h>

#include <assert.h>
#include <stdio.h>

// TODO split the cpp file and mantain one header
// (add a private header if needed)

struct EqualityVisitorBase : public BContactFieldVisitor {
	EqualityVisitorBase() : result(false) {}
	bool result;

	virtual void Visit(BStringContactField* field) {}
	virtual void Visit(BAddressContactField* field) {}
};
/*
struct CopyVisitor : public BContactFieldVisitor {

	BContactField* fOwner;

	CopyVisitor(BContactField* owner)
		:
		fOwner(owner)
		{
		}

	virtual void Visit(BStringContactField* field)
	{
		field->CopyDataFrom(fOwner);
	}

	virtual void Visit(BAddressContactField* field)
	{
		field->CopyDataFrom(fOwner);
	}
};
*/

BContactField::BContactField(type_code type)
	:
	fType(type),
	fUsage(-1),
	fParamList(20, true)
{
}


BContactField::~BContactField()
{
}


type_code
BContactField::FieldType() const
{
	// This store one of the types
	// defined in ContactDefs.h
	// and is different from the
	// type used for the BFlattenable things.
	return fType;
}


int32
BContactField::Usage() const
{
	return fUsage;
}


void
BContactField::SetUsage(int32 usage)
{
	fUsage = usage;
}


const BString&
BContactField::Label() const
{
	return fLabel;
}


void
BContactField::SetLabel(const BString& label)
{
	fLabel.SetTo(label);
}


bool
BContactField::AddParameter(const BString& property)
{
	return fParamList.AddItem(new BString(property));
}


bool
BContactField::AddParameterAt(const BString& property, int32 index)
{
	return fParamList.AddItem(new BString(property), index);
}


bool
BContactField::ReplaceParameter(const BString& property, int32 index)
{
	return fParamList.ReplaceItem(index, new BString(property));
}


const BString&
BContactField::ParameterAt(int32 i) const
{
	return *fParamList.ItemAt(i);
}


int32
BContactField::CountParameters() const
{
	return fParamList.CountItems();
}


status_t
BContactField::RemoveParameter(const BString& property)
{
	// TODO use BObjectList sorting/binary search
	// to improve performances
	for (int i = 0; i < fParamList.CountItems(); i++) {
		if (property.Compare(fParamList.ItemAt(i)->String()) == 0) {
			fParamList.RemoveItemAt(i);
			return B_OK;
		}
	}
	return B_ERROR;
}


bool
BContactField::IsFixedSize() const
{ 
	return false;
}


type_code
BContactField::TypeCode() const
{
	return B_CONTACT_FIELD_TYPE;
}


bool
BContactField::AllowsTypeCode(type_code code) const
{
	if (code != B_CONTACT_FIELD_TYPE)
		return false;
	
	return true;
}


ssize_t
BContactField::FlattenedSize() const
{
	// TODO cache this value
	// for better performances
	ssize_t size = sizeof(type_code);

	int32 count = fParamList.CountItems();
	size += sizeof(count);
	size += sizeof(ssize_t) * count;

	for (int i = 0; i < count; i++)
		size += fParamList.ItemAt(i)->Length();

	return size + sizeof(fUsage);
}


status_t
BContactField::Flatten(BPositionIO* flatData) const
{
	if (flatData == NULL)
		return B_BAD_VALUE;

	// TODO ADD ENDIANESS CODE
	// this is needed because we need to know
	// whitch derived class we need to instantiate
	// in BContactField::UnflattenChildClass()
	
	// TODO check if all data was written
	flatData->Write(&fType, sizeof(type_code));

	int32 count = fParamList.CountItems();
	flatData->Write(&count, sizeof(count));

	for (int i = 0; i < count; i++)
		_AddStringToBuffer(flatData, *fParamList.ItemAt(i));

	flatData->Write(&fUsage, sizeof(fUsage));

	return B_OK;
}


status_t
BContactField::Flatten(void* buffer, ssize_t size) const
{
	if (buffer == NULL)
		return B_BAD_VALUE;

	BMemoryIO flatData(buffer, size);
	return Flatten(&flatData, size);
}


status_t
BContactField::Unflatten(type_code code,
	const void* buffer,	ssize_t size)
{
	if (buffer == NULL)
		return B_BAD_VALUE;

	BMemoryIO flatData(buffer, size);
	return Unflatten(code, &flatData);
}


status_t
BContactField::Unflatten(type_code code, BPositionIO* flatData)
{
	if (code != B_CONTACT_FIELD_TYPE)
		return B_BAD_VALUE;

	// read the type of the field	
	flatData->Read(&fType, sizeof(type_code));

	int32 count;
	flatData->Read(&count, sizeof(count));
	for (int i = 0; i < count; i++) {
		BString* str = new BString();
		*str = _ReadStringFromBuffer(flatData);
		fParamList.AddItem(str);
	}

	flatData->Read(&fUsage, sizeof(fUsage));

	return B_OK;
}


status_t
BContactField::CopyDataFrom(BContactField* field)
{
	fLabel.SetTo(field->fLabel);
	fType = field->fType;
	fUsage = field->fUsage;

	for (int i = 0; i < field->fParamList.CountItems(); i++) {
		const char* str = field->fParamList.ItemAt(i)->String();
		fParamList.AddItem(new BString(str));
	}
	return B_OK;
}


// You cannot instantiate a pure BContactField
// so this method help to unflatten the correct 
// derived class
BContactField*
BContactField::UnflattenChildClass(const void* from, ssize_t size)
{
	BMemoryIO data(from, size);

	type_code childType;
	data.Read(&childType, sizeof(childType));
	BContactField* child = NULL;
	switch (childType) {
		case B_CONTACT_NAME:
		case B_CONTACT_NICKNAME:
		case B_CONTACT_EMAIL:
		case B_CONTACT_NOTE:
		case B_CONTACT_ORGANIZATION:
		case B_CONTACT_IM:
		case B_CONTACT_URL:
		case B_CONTACT_PHONE:
			child = new BStringContactField(childType);
			break;
		case B_CONTACT_ADDRESS:
			printf("address\n");
			child = new BAddressContactField();
			break;
	}

	/*if (child != NULL 
		&& child->Unflatten(B_CONTACT_FIELD_TYPE, from, size) != B_OK)
			return NULL;*/

	status_t ret = child->Unflatten(B_CONTACT_FIELD_TYPE, from, size);
	printf("%s\n", strerror(ret));
	return child;
}


BContactField*
BContactField::Duplicate(BContactField* from)
{
	BContactField* child = NULL;
	type_code childType = from->FieldType();
	ObjectDeleter<BContactField> deleter;

	switch (childType) {
		case B_CONTACT_NAME:
		case B_CONTACT_NICKNAME:
		case B_CONTACT_EMAIL:
		case B_CONTACT_NOTE:
		case B_CONTACT_ORGANIZATION:
		case B_CONTACT_IM:
		case B_CONTACT_URL:
		case B_CONTACT_PHONE:
			child = new BStringContactField(childType);
			break;
		case B_CONTACT_ADDRESS:
			child = new BAddressContactField();
			break;
	}

	if (child != NULL && child->CopyDataFrom(from) == B_OK)
		return child;

	deleter.SetTo(child);
	return NULL;
}


ssize_t
BContactField::_AddStringToBuffer(BPositionIO* buffer, const BString& str) const
{
	ssize_t valueLength = str.Length();
	if (valueLength > 0) {
		ssize_t ret = buffer->Write(&valueLength, sizeof(valueLength));
		return ret + buffer->Write(str, valueLength);
	} else
		return buffer->Write(0, sizeof(ssize_t));

	return -1;
}


BString
BContactField::_ReadStringFromBuffer(BPositionIO* buffer, ssize_t len)
{
	BString ret;
	if (len == -1)
		buffer->Read(&len, sizeof(len));

	char* valueBuffer;
	if (len != 0) {
		ArrayDeleter<char> deleter(valueBuffer = new char[len]);
		buffer->Read(valueBuffer, len);
		ret = BString(valueBuffer, len);
	} else {
		ret = BString();
	}
	return ret; 
}

// BStringContactField

BStringContactField::BStringContactField(type_code type, const BString& str)   
	:
	BContactField(type),
	fValue(str)
{
	_InitLabel();
}


BStringContactField::BStringContactField(type_code type, const char* str) 
	:
	BContactField(type),
	fValue(str)
{
	_InitLabel();
}


BStringContactField::~BStringContactField()
{	
}


struct BStringContactField::EqualityVisitor : public EqualityVisitorBase {

	BStringContactField* fOwner;

	EqualityVisitor(BStringContactField* owner)
		:
		EqualityVisitorBase(),
		fOwner(owner) 
		{
		}

	virtual void Visit(BStringContactField* field)
	{
		if (fOwner->Usage() == field->Usage())
			result = true;

		if (field->Value().Compare(fOwner->Value()) == 0)
			result = true;
		// compare params
		// maybe BContactField can define
		// a protected function
	}

	virtual void Visit(BAddressContactField* field)
	{
	}
};


void
BStringContactField::Accept(BContactFieldVisitor* v)
{ 
	v->Visit(this);
}


bool
BStringContactField::IsEqual(BContactField* field)
{
	BStringContactField::EqualityVisitor equalityChecker(this);
	field->Accept(&equalityChecker);
	return equalityChecker.result;
}


void
BStringContactField::SetValue(const BString& value)
{
	fValue.SetTo(value);	
}


const BString&
BStringContactField::Value() const
{
	return fValue;
}


// this method should take a BStringContactField
status_t
BStringContactField::CopyDataFrom(BContactField* field)
{
	BStringContactField* from = dynamic_cast<BStringContactField*>(field);
	if (from != NULL) {
		if (BContactField::CopyDataFrom(field) == B_OK) {
			fValue.SetTo(from->fValue);
			return B_OK;
		}
	}
	return B_ERROR;
}


ssize_t
BStringContactField::FlattenedSize() const
{
	ssize_t size = BContactField::FlattenedSize();
	size += sizeof(ssize_t);
	size += fValue.Length();

	return size;
}


status_t
BStringContactField::Flatten(void* buffer, ssize_t size) const
{
	if (size < FlattenedSize())
		return B_ERROR;

	BMemoryIO flatData(buffer, size);
	status_t ret = BContactField::Flatten(&flatData);
	if (ret != B_OK)
		return ret;

	_AddStringToBuffer(&flatData, fValue);

	return B_OK;
}


status_t
BStringContactField::Unflatten(type_code code,
	const void* buffer,	ssize_t size)
{
	BMemoryIO data(buffer, size);
	status_t ret = BContactField::Unflatten(code, &data);
	if (ret != B_OK)
		return ret;

	fValue = _ReadStringFromBuffer(&data);

	return B_OK;
}


void
BStringContactField::_InitLabel()
{
	switch (fType) {
		case B_CONTACT_NAME:
			SetLabel("Name");
		break;
		case B_CONTACT_NICKNAME:
			SetLabel("Nickname");
		break;
		case B_CONTACT_EMAIL:
			SetLabel("Email");
		break;
		case B_CONTACT_NOTE:
			SetLabel("Note");
		break;
		case B_CONTACT_ORGANIZATION:
			SetLabel("Organization");
		break;
		case B_CONTACT_IM:
			SetLabel("IM");
		break;
		case B_CONTACT_URL:
			SetLabel("URL");
		break;
		case B_CONTACT_PHONE:
			SetLabel("Phone Number");
		break;
	}
}

/** BAddressContactField **/

struct BAddressContactField::EqualityVisitor : public EqualityVisitorBase {

	BAddressContactField* fOwner;

	EqualityVisitor(BAddressContactField* owner)
		:
		EqualityVisitorBase(),
		fOwner(owner) 
		{
		}

	virtual void Visit(BStringContactField* field)
	{
	}

	virtual void Visit(BAddressContactField* field)
	{
		if (fOwner->Value().Compare(fOwner->Value()) == 0)
			if (fOwner->Usage() == field->Usage())
				result = true;
	}
};

/*
BAddressContactField::BAddressContactField(const BAddress& address)
	:
	BContactField(B_CONTACT_ADDRESS)
{
}*/


BAddressContactField::BAddressContactField(BString address, bool wellFormed)
  	:
	BContactField(B_CONTACT_ADDRESS),
	fWellFormed(wellFormed)
{
	_SplitValue(address);
	SetLabel("Address");
}


BAddressContactField::~BAddressContactField()
{
}


void
BAddressContactField::Accept(BContactFieldVisitor* v)
{
	v->Visit(this);
}


bool
BAddressContactField::IsEqual(BContactField* field)
{
	BAddressContactField::EqualityVisitor equalityChecker(this);
	field->Accept(&equalityChecker);
	return equalityChecker.result;
}


void
BAddressContactField::SetValue(const BString& value)
{
	_SplitValue(value);
}


const BString&
BAddressContactField::Value() const
{
/*	// this is returning the address
	// as described in the vcard-21 spec
	BString str = BString(fPostalBox);
	str << fPostalBox << ";" << fNeighbor << ";";
	str << fStreet << ";" << fCity << ";" << fRegion;
	str << ";" << fPostalCode << ";" << fCountry << ";"; 

	return str.String();*/
	return fPostalBox;
}


status_t
BAddressContactField::CopyDataFrom(BContactField* field)
{
	if (field->FieldType() == B_CONTACT_ADDRESS) {
		BContactField::CopyDataFrom(field);
		BAddressContactField* from
			= dynamic_cast<BAddressContactField*>(field);

		if (from != NULL) {
			fStreet.SetTo(from->fStreet);
			fPostalBox.SetTo(from->fPostalBox);
			fNeighbor.SetTo(from->fNeighbor);
			fCity.SetTo(from->fCity);
			fRegion.SetTo(from->fRegion);
			fPostalCode.SetTo(from->fPostalCode);
			fCountry.SetTo(from->fCountry);
			fWellFormed = from->fWellFormed;

			return B_OK;
		}
	}
	return B_ERROR;
}


bool
BAddressContactField::IsWellFormed() const
{
	return fWellFormed;	
}


const BString&
BAddressContactField::Street() const
{
	return fStreet;
}


const BString&
BAddressContactField::PostalBox() const
{
	return fPostalBox;
}


const BString&
BAddressContactField::Neighborhood() const
{
	return fNeighbor;
}


const BString&
BAddressContactField::City() const
{
	return fCity;
}


const BString&
BAddressContactField::Region() const
{
	return fRegion;
}


const BString&
BAddressContactField::PostalCode() const
{
	return fPostalCode;
}


const BString&
BAddressContactField::Country() const
{
	return fCountry;
}


void
BAddressContactField::SetStreet(const BString& street)
{
	fStreet.SetTo(street);
}


void
BAddressContactField::SetPostalBox(const BString& postBox)
{
	fPostalBox.SetTo(postBox);
}


void
BAddressContactField::SetNeighborhood(const BString& neighbor)
{
	fNeighbor.SetTo(neighbor);
}


void
BAddressContactField::SetCity(const BString& city)
{
	fCity.SetTo(city);
}


void
BAddressContactField::SetRegion(const BString& region)
{
	fRegion.SetTo(region);
}


void
BAddressContactField::SetPostalCode(const BString& postalCode)
{
	fPostalCode.SetTo(postalCode);
}


void
BAddressContactField::SetCountry(const BString& country)
{
	fCountry.SetTo(country);
}


ssize_t
BAddressContactField::FlattenedSize() const
{
	ssize_t size = BContactField::FlattenedSize();
	size += sizeof(ssize_t);
	size += Value().Length();

	return size + sizeof(fWellFormed);
}


status_t
BAddressContactField::Flatten(void* buffer, ssize_t size) const
{
	if (size < FlattenedSize())
		return B_ERROR;

	BMemoryIO flatData(buffer, size);
	status_t ret = BContactField::Flatten(&flatData);
	if (ret != B_OK)
		return ret;

	// TODO maybe it's better to control the size
	// of the data written

	_AddStringToBuffer(&flatData, Value());

	flatData.Write(&fWellFormed, sizeof(fWellFormed));

	return B_OK;
}


status_t
BAddressContactField::Unflatten(type_code code,
	const void* buffer,	ssize_t size)
{
	BMemoryIO data(buffer, size);
	status_t ret = BContactField::Unflatten(code, &data);
	if (ret != B_OK)
		return ret;

	SetValue(_ReadStringFromBuffer(&data));
	data.Read(&fWellFormed, sizeof(fWellFormed));

	return B_OK;

}


bool
BAddressContactField::_SplitValue(const BString& str)
{
	BString value(str);
	_PopValue(value, fPostalBox);
	_PopValue(value, fNeighbor);
	_PopValue(value, fStreet);
	_PopValue(value, fCity);
	_PopValue(value, fRegion);
	_PopValue(value, fPostalCode);
	_PopValue(value, fCountry);
	return true;
}


void
BAddressContactField::_PopValue(BString& str, BString& value)
{
	int32 index = str.FindFirst(";", 0);
	if (index == B_ERROR) {
		fWellFormed = false;
		value.SetTo("");
		return;
	}
	str.MoveInto(value, 0, index);
	str.Remove(0,1);
}
