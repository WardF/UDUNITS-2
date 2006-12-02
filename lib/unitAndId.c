/*
 * Searchable unit-and-identifier tree.
 *
 * $Id: unitAndId.c,v 1.2 2006/12/02 22:33:47 steve Exp $
 */

/*LINTLIBRARY*/

#ifndef	_XOPEN_SOURCE
#   define _XOPEN_SOURCE 500
#endif

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "unitAndId.h"
#include "units.h"

extern enum utStatus	utStatus;


/*
 * Arguments:
 *	unit	The unit.  May be freed upon return.
 *	id	The identifier (name or symbol).  May be freed upon return.
 * Returns:
 *	NULL	Failure.  "utStatus" will be
 *		    UT_BADARG	"unit" or "id" is NULL.
 *		    UT_OS	Operating-system failure.  See "errno".
 *	else	Pointer to the new unit-and-identifier.
 */
UnitAndId*
uaiNew(
    const utUnit* const	unit,
    const char* const	id)
{
    UnitAndId*	entry = NULL;		/* failure */

    if (id == NULL || unit == NULL) {
	utStatus = UT_BADARG;
    }
    else {
	entry = malloc(sizeof(UnitAndId));

	if (entry == NULL) {
	    utStatus = UT_OS;
	}
	else {
	    entry->id = strdup(id);

	    if (entry->id == NULL) {
		utStatus = UT_OS;
	    }
	    else {
		entry->unit = utClone(unit);

		if (entry->unit == NULL) {
		    assert(utStatus != UT_SUCCESS);
		    free(entry->id);
		}
	    }

	    if (utStatus != UT_SUCCESS) {
		free(entry);
		entry = NULL;
	    }
	}
    }

    return entry;
}


/*
 * Frees memory of a unit-and-identifier.
 *
 * Arguments:
 *	entry	Pointer to the unit-and-identifier or NULL.
 */
void
uaiFree(
    UnitAndId* const	entry)
{
    if (entry != NULL) {
	free(entry->id);
	utFree(entry->unit);
	free(entry);
    }
}
