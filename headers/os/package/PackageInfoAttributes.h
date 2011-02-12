/*
 * Copyright 2011, Haiku, Inc.
 * Distributed under the terms of the MIT License.
 */
#ifndef _PACKAGE__PACKAGE_INFO_ATTRIBUTES_H_
#define _PACKAGE__PACKAGE_INFO_ATTRIBUTES_H_


namespace BPackageKit {


enum BPackageInfoAttributeID {
	B_PACKAGE_INFO_NAME = 0,
	B_PACKAGE_INFO_SUMMARY,		// single line
	B_PACKAGE_INFO_DESCRIPTION,	// multiple lines possible
	B_PACKAGE_INFO_VENDOR,		// e.g. "Haiku Project"
	B_PACKAGE_INFO_PACKAGER,	// e-mail address preferred
	B_PACKAGE_INFO_ARCHITECTURE,
	B_PACKAGE_INFO_VERSION,		// <major>[.<minor>[.<micro>]]
	B_PACKAGE_INFO_COPYRIGHTS,	// list
	B_PACKAGE_INFO_LICENSES,	// list
	B_PACKAGE_INFO_PROVIDES,	// list of resolvables this package provides,
								// each optionally giving a version
	B_PACKAGE_INFO_REQUIRES,	// list of resolvables required by this package,
								// each optionally specifying a version relation
								// (e.g. libssl.so >= 0.9.8)
	B_PACKAGE_INFO_SUPPLEMENTS,	// list of resolvables that are supplemented
								// by this package, i.e. this package marks
								// itself as an extension to other packages
	B_PACKAGE_INFO_CONFLICTS,	// list of resolvables that inhibit installation
								// of this package
	B_PACKAGE_INFO_FRESHENS,	// list of resolvables that this package
								// contains a patch for
	B_PACKAGE_INFO_REPLACES,	// list of resolvables that this package
								// will replace (upon update)
	B_PACKAGE_INFO_FLAGS,
	B_PACKAGE_INFO_CHECKSUM,	// sha256-checksum
	//
	B_PACKAGE_INFO_ENUM_COUNT,
};


}	// namespace BPackageKit


#endif	// _PACKAGE__PACKAGE_INFO_ATTRIBUTES_H_
