#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "langspec.h"

extern FILE *yyin;
extern int yyparse();

static char *name = NULL;
static char *startblk = NULL;
static char *endblk = NULL;
static char *starteol = NULL;
static char **extvect = NULL;
static int vecsize = 0;

static void normalize_string(char *str)
{
    char *esc;

    /* Get rid of opening quote. */
    memmove(str, str+1, strlen(str));

    while ((esc = strstr(str, "\\\"")) != NULL)
    {
        /* Remove any backslashes before quotes. */
        memmove(esc, esc+1, strlen(esc));
    }

    /* Get rid of trailing quote. */
    str[strlen(str) - 1] = '\0';
}

void parse_config(const char *file)
{
    yyin = fopen(file, "r");

    if (yyin == NULL) {
        perror("fopen");
    }
    else {
        yyparse();
        fclose(yyin);
    }
}


void addlanguage(void)
{
    unsigned long lang;
    int i;

    if ((startblk == NULL) != (endblk == NULL)) {
        fprintf(stderr, "error: 'startblk' and 'endblk' must both be NULL or "
                        "both be defined.\n");
    }
    else if (name == NULL) {
        fprintf(stderr, "error: missing attribute 'name'.\n");
    }
    else {
        lang = add_langspec(name, startblk, endblk, starteol);
        for (i = 0; i < vecsize; i++) {
            add_langext(lang, extvect[i]);
            free(extvect[i]);
        }
    }

#define reset(s) free(s); s = NULL
    reset(name);
    reset(startblk);
    reset(endblk);
    reset(starteol);
    reset(extvect);
    vecsize = 0;
#undef reset
}

void assign_name(char *val)
{
    name = val;
    normalize_string(val);
}

void assign_startblk(char *val)
{
    startblk = val;
    normalize_string(val);
}

void assign_endblk(char *val)
{
    endblk = val;
    normalize_string(val);
}

void assign_starteol(char *val)
{
    starteol = val;
    normalize_string(val);
}

void add_extension(char *ext)
{
    normalize_string(ext);
    vecsize++;
    extvect = realloc(extvect, vecsize * sizeof(char *));
    if (extvect == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    extvect[vecsize-1] = ext;
}

