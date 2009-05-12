/*
 * Copyright 2003-2006, Axel Dörfler, axeld@pinc-software.de. All rights reserved.
 * Distributed under the terms of the MIT License.
 */

#include <user_runtime.h>
#include <image.h>
#include <image_defs.h>

#include "init_term_dyn.h"


// Haiku API and ABI versions -- we compile those into shared objects so that
// the runtime loader can discriminate the versions and enable compatibility
// work-arounds, if necessary.

uint32 B_SHARED_OBJECT_HAIKU_VERSION_VARIABLE = B_HAIKU_VERSION;
uint32 B_SHARED_OBJECT_HAIKU_ABI_VARIABLE = B_HAIKU_ABI;


/**	These functions are called from _init()/_fini() (in crti.S, crtn.S)
 *	_init/_term_before() is called before crtbegin/end code is executed,
 *	_init/_term_after() after this.
 *	crtbegin contains code to initialize all global constructors and
 *	other GCC related things (like exception frames).
 */


void
_init_before(image_id id)
{
	void (*before)(image_id);

	if (get_image_symbol(id, B_INIT_BEFORE_FUNCTION_NAME, B_SYMBOL_TYPE_TEXT, (void **)&before) == B_OK)
		before(id);
}


void
_init_after(image_id id)
{
	void (*after)(image_id);

	if (get_image_symbol(id, B_INIT_AFTER_FUNCTION_NAME, B_SYMBOL_TYPE_TEXT, (void **)&after) == B_OK)
		after(id);
}


void
_term_before(image_id id)
{
	void (*before)(image_id);

	if (get_image_symbol(id, B_TERM_BEFORE_FUNCTION_NAME, B_SYMBOL_TYPE_TEXT, (void **)&before) == B_OK)
		before(id);
}


void
_term_after(image_id id)
{
	void (*after)(image_id);

	if (get_image_symbol(id, B_TERM_AFTER_FUNCTION_NAME, B_SYMBOL_TYPE_TEXT, (void **)&after) == B_OK)
		after(id);
}

