#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>

#include "main.h"

/**
 * Retrieve a struct group by the group's name.
 *
 * @param group_name The name of the group to retrieve.
 * @param group Where to put the pointer to the retrieved group.
 * @return E_NOT_FOUND if the group does not exist,
 *         E_ERRNO if the getgrnam() call fails (check errno),
 *         otherwise E_SUCCESS.
 */
inline enum return_t open_group(const char *group_name, struct group **group)
{
    errno = 0;
    if ((*group = getgrnam(group_name))) {
        return E_SUCCESS;
    }

    if (errno == 0) {
        return E_NOT_FOUND;
    }

    return E_ERRNO;
}

/**
 * Print the UIDs of individuals whose primary group is the one specified.
 *
 * @param group The group whose primary members to print.
 * @return E_SUCCESS if one or more members printed,
 *         E_EMPTY otherwise.
 */
enum return_t print_primary_members(const struct group *group)
{
    struct passwd* account;
    bool first = true;

    while ((account = getpwent())) {
        if (account->pw_gid != group->gr_gid) {
            continue;
        }

        if (!first) {
            fputs(" ", stdout);
        }

        fputs(account->pw_name, stdout);
        first = false;
    }

    endpwent();
    return first ? E_EMPTY : E_SUCCESS;
}

/**
 * Print the UIDs of individuals who are listed as being members of the group.
 * If using files in /etc/nsswitch.conf, this is equivalent to printing out
 * the names listed against the group in /etc/group. It does not include
 * individuals whose primary group is the one specified.
 *
 * @param group The group whose secondary members to print.
 * @return E_SUCCESS if one or more members printed,
 *         E_EMPTY otherwise.
 */
enum return_t print_secondary_members(const struct group *group)
{
    bool first = true;
    char **name = group->gr_mem;

    while (*name) {
        if (!first) {
            fputs(" ", stdout);
        }

        fputs(*name++, stdout);
        first = false;
    }

    return first ? E_EMPTY : E_SUCCESS;
}