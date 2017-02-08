#pragma once

#include <grp.h>

#include "main.h"

enum return_t open_group(const char *name, struct group **group);

enum return_t print_primary_members(const struct group *group);

enum return_t print_secondary_members(const struct group *group);
