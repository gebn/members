#pragma once

#include <stdlib.h>

/**
 * The type returned by all group-related functions.
 */
enum return_t {
    E_SUCCESS = EXIT_SUCCESS,     // no error
    E_INPUT = 1,       // invalid input
    E_NOT_FOUND = 2,   // group does not exist
    E_ERRNO = 3,       // an error occurred; check errno
    E_EMPTY = 4        // group is empty
};
