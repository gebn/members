#include <stdio.h>

#include "help.h"
#include "main.h"

/**
 * Output basic program usage.
 *
 * @param stream The output stream to print to.
 */
inline void print_usage(FILE *stream) {
    fprintf(stream, "Usage: %s [OPTION]... GROUP\n", PROGRAM);
}

/**
 * Output detailed help. Used when `-h` or `--help` passed.
 */
inline void print_help() {
    print_usage(stdout);
    puts("Show members of a group.\n"
         "\n"
         "members(1) is the complement of groups(1): where groups(1) shows the groups a\n"
         "user belongs to, members(1) shows users belonging to a group.\n"
         "\n"
         "  -p, --primary              show only members for which GROUP is their\n"
         "                               primary group\n"
         "  -s, --secondary            show only members who have a secondary\n"
         "                               membership in GROUP\n"
         "  -t, --two-lines            show primary members on the first line of output\n"
         "                               and secondary members on the second line; two\n"
         "                               lines are always printed\n"
         "  -h, --help                 display this help and exit\n"
         "  -v, --version              output version information and exit\n"
         "\n"
         "Exit status:\n"
         " 0  OK\n"
         " 1  input error (e.g. no GROUP, unrecognised flag)\n"
         " 2  GROUP does not exist\n"
         " 3  GROUP exists but could not be opened (see error message)\n"
         " 4  GROUP did not have any members\n");
}
