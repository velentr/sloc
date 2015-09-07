/**
 * sloc.c
 *
 * Count lines of code.
 *
 * Entry point for the 'sloc' program, which counts the number or lines of code
 * in various programming languages. Keeps stats on the total number of files
 * counted, as well as counts of total lines, blank lines, lines containing
 * code, and lines containing comments.
 *
 * Revision History:
 *       2 Sep 2015     Brian Kubisiak      Initial revision, incomplete.
 */
#include <stdlib.h>     /* exit */
#include <stdio.h>      /* printf */
#include <unistd.h>     /* getopt */
#include <assert.h>     /* assert */

#include "sloc.h"
#include "langspec.h"
#include "count.h"
#include "config.h"

#define DEFAULT_CONFIG_FILE "default.conf"

static void usage(char *arg0)
{
    printf("usage: %s [-c config] [-hv] [filenames]\n", arg0);
}

static void version(void)
{
    printf("sloc-%s, (c) 2013-15\n", VERSION_STRING);
}

int main(int argc, char * const argv[])
{
    int opt;
    int i;

    init_langspecs();

    parse_config(DEFAULT_CONFIG_FILE);

    /* Parse command-line options. */
    while ((opt = getopt(argc, argv, "c:hv")) != -1)
    {
        switch (opt)
        {
        case 'c':
            parse_config(optarg);
            break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_SUCCESS);
        case 'v':
            version();
            exit(EXIT_SUCCESS);
        case '?':
            usage(argv[0]);
            exit(EXIT_FAILURE);
        /* Should never return an option that isn't being handled above. */
        default:
            assert(0);
        }
    }

    /* Done parsing options; if no args left, count the current directory. */
    if (optind == argc) {
        count_from(".");
    }
    /* Else, count each of the given arguments. */
    else {
        for (i = optind; i < argc; i++) {
            count_from(argv[i]);
        }
    }

    print_langspecs();

    free_langspecs();

    return EXIT_SUCCESS;
}
