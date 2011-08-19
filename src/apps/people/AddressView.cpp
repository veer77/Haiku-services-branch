
#include "AddressView.h"

#include <ControlLook.h>


AddressView::AddressView(BAddressContactField* field)
	:
	BGridView("address view"),
	fStreet(NULL),
	fPostalBox(NULL),
	fNeighbor(NULL),
	fCity(NULL),
	fRegion(NULL),
	fPostalCode(NULL),
	fCountry(NULL),
	fCount(0),
	fCheck(false),
	fField(field)
{
	SetFlags(Flags() | B_WILL_DRAW);

	float spacing = be_control_look->DefaultItemSpacing();
	BGridLayout* layout = GridLayout();
	layout->SetInsets(spacing, spacing, spacing, spacing);

	if (field != NULL) {
		_AddControl("Street", field->Street(), fStreet);
		_AddControl("PostalBox", field->PostalBox(), fPostalBox);
		_AddControl("Neighborhood", field->Neighborhood(), fNeighbor);
		_AddControl("City", field->City(), fCity);
		_AddControl("Region", field->Region(), fRegion);
		_AddControl("Postal Code", field->PostalCode(), fPostalCode);
		_AddControl("Country", field->Country(), fCountry);
		fCheck = true;
		delete fCountry;
		fCountry = new BTextControl("lol", "lol", NULL);
		BGridLayout* layout = GridLayout();


	layout->AddItem(fCountry->CreateLabelLayoutItem(), 1, fCount);
	layout->AddItem(fCountry->CreateTextViewLayoutItem(), 2, fCount);
	}
}


AddressView::~AddressView()
{

}


bool
AddressView::HasChanged()
{
	if (!fCheck)
		return false;

	printf("%p\n", fStreet);
	if (fField->Street() != fStreet->Text())
		return false;

	if (fField->PostalBox() != fPostalBox->Text())
		return false;

	if (fField->Neighborhood() != fNeighbor->Text())
		return false;

	if (fField->City() != fCity->Text())
		return false;

	if (fField->Region() != fRegion->Text())
		return false;

	if (fField->PostalCode() != fPostalCode->Text())
		return false;

	if (fField->Country() != fCountry->Text())
		return false;

	return true;
}


void
AddressView::Revert()
{
	if (!fCheck)
		return;

	fStreet->SetText(fField->Street());
	fPostalBox->SetText(fField->PostalBox());
	fNeighbor->SetText(fField->Neighborhood());
	fCity->SetText(fField->City());
	fRegion->SetText(fField->Region());
	fPostalCode->SetText(fField->PostalCode());
	fCountry->SetText(fField->Country());
}


void
AddressView::Update()
{
	if (!fCheck)
		return;

	fField->SetStreet(fStreet->Text());
	fField->SetPostalBox(fPostalBox->Text());
	fField->SetNeighborhood(fNeighbor->Text());
	fField->SetCity(fCity->Text());
	fField->SetRegion(fRegion->Text());
	fField->SetPostalCode(fPostalCode->Text());
	fField->SetCountry(fCountry->Text());
}


BString
AddressView::Value() const
{
	if (!fCheck)
		return "";

	return fField->Value();
}


void
AddressView::_AddControl(const char* label, const char* value, BTextControl* control)
{
	control = new BTextControl(label, value, NULL);
	printf("%s %s\n", label, value);
	BGridLayout* layout = GridLayout();
	fCount += 1;

	layout->AddItem(control->CreateLabelLayoutItem(), 1, fCount);
	layout->AddItem(control->CreateTextViewLayoutItem(), 2, fCount);
}
