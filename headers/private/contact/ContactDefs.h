/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef B_CONTACT_DEFS_H
#define B_CONTACT_DEFS_H

#include <Errors.h>

enum ContactFieldType {
	B_CONTACT_NAME 			= 'BCNN', //BString
	B_CONTACT_NICKNAME 		= 'BCNK', // *
	B_CONTACT_PHOTO 		= 'BCNP', // image
	B_CONTACT_ADDRESS 		= 'BCNA', // BAddress
	B_CONTACT_PHONE			= 'BCNP', // BPhone
	B_CONTACT_EMAIL			= 'BCNE', // string
	B_CONTACT_URL			= 'BCNU', // BUrl
	B_CONTACT_GEO			= 'BCNG', // string?
	B_CONTACT_NOTE			= 'BCNO', // BString
	B_CONTACT_ORGANIZATION  = 'BCNJ', // string
	B_CONTACT_IM			= 'BCNI', // string
	B_CONTACT_UID			= 'BCND', // uint32
	B_CONTACT_GROUP			= 'BCNR', // uint32 / string
	B_CONTACT_PROTOCOLS 	= 'BCNL'  // string?
};

enum CommonFieldUsageTypes {
	CONTACT_DATA_CUSTOM = 'CDTC',
	CONTACT_DATA_OTHER	= 'CDOT',	
	CONTACT_DATA_HOME	= 'CDHM',
	CONTACT_DATA_WORK	= 'CDWK'
};

enum SpecificFieldUsageTypes {
	CONTACT_NAME_FAMILY = 'CNFM',
	CONTACT_NAME_GIVEN	= 'CNGV',
	CONTACT_NAME_MIDDLE	= 'CNMD',
	CONTACT_NAME_SUFFIX	= 'CNSF',

	CONTACT_NICKNAME_DEFAULT = 'CNDF',
	CONTACT_NICKNAME_MAIDEN	 = 'CNMD',
	CONTACT_NICKNAME_SHORT_NAME	 = 'CNSN',
	CONTACT_NICKNAME_INITIALS	 = 'CNIN',

	CONTACT_EMAIL_MOBILE = 'CEMB',

	CONTACT_PHONE_MOBILE = 'CPMB',
	CONTACT_PHONE_FAX_WORK = 'CPFW',
	CONTACT_PHONE_FAX_HOME = 'CPFH',
	CONTACT_PHONE_PAGER = 'CPPG',
	CONTACT_PHONE_CALLBACK = 'CPXK',
	CONTACT_PHONE_CAR = 'CPCR',
	CONTACT_PHONE_ORG_MAIN = 'CPOM',
	CONTACT_PHONE_ISDN = 'CPIN',
	CONTACT_PHONE_MAIN  = 'CPMN',
	CONTACT_PHONE_RADIO = 'CPRA',
	CONTACT_PHONE_TELEX = 'CPTE',
	CONTACT_PHONE_TTY_TDD = 'CPTY',
	CONTACT_PHONE_WORK_MOBILE = 'CPWM',
	CONTACT_PHONE_WORK_PAGER = 'CPWP',
	CONTACT_PHONE_ASSISTANT = 'CPAS',
	CONTACT_PHONE_MMS = 'CPMS',

	CONTACT_PHOTO_URL = 'CPUL',
	CONTACT_PHOTO_REF = 'CPRF',
	CONTACT_PHOTO_BITMAP = 'CPBT'
};

#endif // B_CONTACT_DEFS_H
