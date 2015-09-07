/**
 * count.c
 *
 * Count lines of code.
 *
 * Revision History:
 *       4 Sep 2015     Brian Kubisiak      Initial revision.
 */
#ifndef _COUNT_H_
#define _COUNT_H_


#define _XOPEN_SOURCE 700   /* Needed for nftw, getline. */
#include <ftw.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "count.h"
#include "langspec.h"

#define DEFAULT_OPEN_FD 4096

typedef int (*comment_checker)(const char *, const char *);

int checknop(const char *line, const char *delim)
{
    /* Get rid of compiler warning about unused parameters. */
    (void)line; (void)delim;

    return 0;
}

int iscomment(const char *line, const char *delim)
{
    return strncmp(line, delim, strlen(delim)) == 0 ? 1 : 0;
}

static struct langspec_t *find_langspec(const char *basename)
{
    const char *ext;

    /* Find the file extension. */
    ext = strrchr(basename, '.');

    /* If no extension, use whole filename. */
    if (ext == NULL) {
        ext = basename;
    }

    /* Get the langspec associated with this filename. */
    return get_langspec(ext);
}

static int count_line(const char *line, struct langspec_t *spec, int in_comment,
                      comment_checker checkblk, comment_checker checkeol)
{
    unsigned int i;

    int code    = 0;
    int comment = 0;

    /* Add another line to the total count. */
    spec->total++;

    for (i = 0; line[i] != '\0'; i++)
    {
        /* Ignore whitespace characters. */
        if (isspace(line[i]))
            continue;

        if (in_comment) {
            comment = 1;
            if (checkblk(line+i, spec->endblk)) {
                in_comment = 0;
                i += strlen(spec->endblk) - 1;
            }
        }
        else {
            if (checkeol(line+i, spec->starteol)) {
                comment = 1;
                break;
            }
            else {
                if (checkblk(line+i, spec->startblk)) {
                    in_comment = 1;
                    comment = 1;
                    i += strlen(spec->startblk) - 1;
                }
                else {
                    code = 1;
                }
            }
        }
    }

    spec->comment += comment;
    spec->code    += code;
    spec->blank   += (code == 0 && comment == 0) ? 1 : 0;

    return in_comment;
}

static void count_stream(FILE *fp, struct langspec_t *spec)
{
    int in_comment = 0;
    char *line = NULL;
    size_t n = 0;
    comment_checker checkblk;
    comment_checker checkeol;

    checkblk = (spec->startblk == NULL) ? checknop : iscomment;
    checkeol = (spec->starteol == NULL) ? checknop : iscomment;

    /* Count each line individually, keeping track of whether counter is in a
     * block comment. */
    while (getline(&line, &n, fp) != -1)
    {
        in_comment = count_line(line, spec, in_comment, checkblk, checkeol);
    }

    /* getline malloc'd line buffer; free it here. */
    free(line);
}

static void count_file(const char *fpath, int base)
{
    struct langspec_t *spec;
    FILE *fp;

    spec = find_langspec(fpath+base);
    if (spec == NULL) {
        return;
    }

    /* Open file for reading. */
    fp = fopen(fpath, "r");

    /* If cannot open file, silently fail. */
    /* TODO better behavior here. */
    if (fp == NULL) {
        return;
    }

    /* Once file is open, count input stream. */
    spec->files++;
    count_stream(fp, spec);

    /* Close the file again. */
    fclose(fp);
}

static int count_node(const char *fpath, const struct stat *sb, int typeflag,
                      struct FTW *ftwbuf)
{
    /* Get rid of compiler warning. */
    (void)sb;

    /* Only count lines if 'fpath' is a regular file. */
    if (typeflag == FTW_F) {
        count_file(fpath, ftwbuf->base);
    }

    /* Cannot fail. */
    return 0;
}

void count_from(const char *root)
{
    nftw(root, count_node, DEFAULT_OPEN_FD, 0);
}


#endif /* end of include guard: _COUNT_H_ */
