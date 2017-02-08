#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
#include <getopt.h>
#include <grp.h>

#include "main.h"
#include "group.h"
#include "help.h"

/**
 * The name of this program, used for error and warning messages.
 */
const char *PROGRAM = "members";

/**
 * The version of this program.
 */
const char *VERSION = "0.0.1-alpha";

/**
 * Command-line flags.
 */
static const struct option long_flags[] = {
        { "version",   no_argument, NULL, 'v' },
        { "help",      no_argument, NULL, 'h' },
        { "primary",   no_argument, NULL, 'p' },
        { "secondary", no_argument, NULL, 's' },
        { "two-lines", no_argument, NULL, 't' },
        { 0,           0,           0,    0   }
};

/**
 * Print a warning using the conventional format.
 *
 * @param format_string The string to print, including format specifiers.
 * @param ... Arguments to insert into the `format_string`.
 */
inline void warn(const char *format_string, ...)
{
    va_list args;
    fprintf(stderr, "%s: ", PROGRAM);
    va_start(args, format_string);
    vfprintf(stderr, format_string, args);
    va_end(args);
}

/**
 * Outputs the program name and version.
 */
inline void print_version() {
    printf("%s %s\n", PROGRAM, VERSION);
}

/**
 * Print the primary members of a group with error handling.
 *
 * @param group The group whose primary members to print.
 * @return E_SUCCESS if >0 members were printed,
 *         E_EMPTY otherwise.
 */
static inline enum return_t handle_primary_members(const struct group *group)
{
    enum return_t status;
    status = print_primary_members(group);
    if (status == E_EMPTY) {
        warn("%s is not the primary group of any users\n", group->gr_name);
    }
    else {
        puts("");
    }
    return status;
}

/**
 * Print the secondary members of a group with error handling.
 *
 * @param group The group whose secondary members to print.
 * @return E_SUCCESS if >0 members were printed,
 *         E_EMPTY otherwise.
 */
static inline enum return_t handle_secondary_members(const struct group *group)
{
    enum return_t status;
    status = print_secondary_members(group);
    if (status == E_EMPTY) {
        warn("%s does not have any secondary members\n", group->gr_name);
    }
    else {
        puts("");
    }
    return status;
}

/**
 * Print the primary members of a group on one line followed by the secondary
 * members on another, with error handling. Two lines are always printed.
 *
 * @param group The group whose members to print.
 * @return E_SUCCESS if >0 members were printed,
 *         E_EMPTY otherwise.
 */
static inline enum return_t handle_two_lines_members(const struct group *group)
{
    bool has_output;
    has_output = print_primary_members(group) == E_SUCCESS;
    puts("");
    has_output = print_secondary_members(group) || has_output;
    puts("");
    return has_output ? E_SUCCESS : E_EMPTY;
}

/**
 * Print the primary members of a group followed by the secondary members, all
 * on one line.
 *
 * @param group The group whose members to print.
 * @return E_SUCCESS if >0 members were printed,
 *         E_EMPTY otherwise.
 */
static inline enum return_t handle_all_members(const struct group *group)
{
    enum return_t status;
    bool output;

    status = print_primary_members(group);
    output = status == E_SUCCESS;
    if (output) {
        fputs(" ", stdout);
    }

    status = print_secondary_members(group);
    if (output || status == E_SUCCESS) {
        status = E_SUCCESS;
        puts("");
    }

    return status;
}

int main(const int argc, char *argv[])
{
    bool primary = false, secondary = false, two_lines = false;
    int option_index;
    int option;
    const char *group_name;
    enum return_t result;
    struct group* group;

    while (true) {
        if ((option = getopt_long(argc, argv, "+vhpst", long_flags,
                                  &option_index)) == -1) {
            break;
        }

        switch (option) {
            case 'v':
                print_version();
                return E_SUCCESS;
            case 'h':
                print_help();
                return E_SUCCESS;
            case 'p':
                primary = true;
                break;
            case 's':
                secondary = true;
                break;
            case 't':
                two_lines = true;
                break;
            default:
                // getopt has already printed a message for us
                return E_INPUT;
        }
    }

    if (optind == argc) {
        // missing group name
        print_usage(stderr);
        return E_INPUT;
    }

    group_name = argv[optind];
    result = open_group(group_name, &group);
    if (result == E_NOT_FOUND) {
        warn("group %s does not exist\n", group_name);
        return result;
    }
    else if (result == E_ERRNO) {
        warn("failed to open group %s\n", strerror(errno));
        return result;
    }

    if (primary) {
        return handle_primary_members(group);
    }

    if (secondary) {
        return handle_secondary_members(group);
    }

    if (two_lines) {
        return handle_two_lines_members(group);
    }

    return handle_all_members(group);
}
