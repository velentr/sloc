/*
 * sloc.c
 *      implements functions for counting the lines of source code in a given
 *      directory (including all subdirectories). code is easily modified to
 *      add new languages.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "sloc.h"
#include "languages.h"

int main(int argc, char **argv)
{
    int             i;
    char *          filename = NULL;
    sloc_t          counts[NUM_LANGS];

    for (i = 0; i < NUM_LANGS; i++)
    {
        counts[i].tot = 0;
        counts[i].code = 0;
        counts[i].com = 0;
        counts[i].blank = 0;
        counts[i].files = 0;
    }

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0)
        {
            disp_version();
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            disp_usage(argv[0]);
        }
        else
        {
            filename = argv[i];
            count_lines(filename, counts);
        }
    }

    if (filename == NULL)
    {
        filename = getcwd(NULL, 0);
        count_lines(filename, counts);
        free(filename);
    }

    print_sloc(counts);

    return EXIT_SUCCESS;
}

void disp_version(void)
{
    printf("sloc-"VERSION", (c) 2013 Brian Kubisiak, "
           "see LICENSE for details\n");
    exit(EXIT_SUCCESS);
}

void disp_usage(char *prog)
{
    printf("usage: %s [-v] [-h] [file] [...]\n", prog);
    exit(EXIT_SUCCESS);
}

void count_lines(char *filename, sloc_t *counts)
{
    struct stat sb;
    int         lang;

    if (stat(filename, &sb) != -1)
    {
        if (S_ISDIR(sb.st_mode) != 0)
        {
            count_folder(filename, counts);
        }
        else if ( S_ISREG(sb.st_mode) != 0 &&
                  (lang = get_file_lang(filename)) != -1)
        {
            count_file(filename, counts + lang, lang);
        }
    }
    else
    {
        perror(filename);
    }
}

int strends_with(char *haystack, char *needle)
{
    char *  end = haystack + strlen(haystack) - strlen(needle);

    if (strcmp(end, needle) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int get_file_lang(char *filename)
{
    int i;
    int j;

    for (i = 0; i < NUM_LANGS; i++)
    {
        for (j = 0; langs[i].ext[j] != NULL; j++)
        {
            if (strends_with(filename, langs[i].ext[j]) != 0)
            {
                return i;
            }
        }
    }

    return -1;
}

void count_file(char *filename, sloc_t *counter, int lang)
{
    FILE *  fp;
    char    s[BUFSIZ];
    char    codeline = 0;
    char    comline = 0;
    char    comment = 0;
    char    eol = 0;
    char *  pos;

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return;
    }

    counter->files++;

    /* loop through doc */
    while (fgets(s, BUFSIZ, fp) != NULL)
    {
        /* loop through line */
        for (pos = s; pos < s + BUFSIZ; pos++)
        {
            /* comment switch */
            switch (comment)
            {
            case 0:
                /* if not in comment, check for beginning of comment */
                if (langs[lang].startblk != NULL &&
                    strstr(pos, langs[lang].startblk) == pos)
                {
                    /* if beginning of block is found, enter comment mode */
                    comment = 1;
                    comline = 1;
                    /* continue to one less than the end of the comment */
                    pos += (strlen(langs[lang].startblk) - 1);
                    /* continue loop through line */
                    continue;
                }
                else if (langs[lang].eol != NULL &&
                         strstr(pos, langs[lang].eol) == pos)
                {
                    comline = 1;
                    eol = 1;
                }
                /* break comment switch */
                break;
            default:
                /* if in comment, check for end of block */
                if (langs[lang].endblk != NULL &&
                    strstr(pos, langs[lang].endblk) == pos)
                {
                    /* if the end of the block is found, exit comment mode */
                    comment = 0;
                    comline = 1;
                    /* continue to one less than the end of the comment */
                    pos += (strlen(langs[lang].endblk) - 1);
                    /* continue loop through line */
                    continue;
                }
                /* break comment switch */
                break;
            }
            if (eol == 1)
            {
                /* break loop through line */
                break;
            }

            /* add the character to the proper line count */
            /* pos switch */
            switch (*pos)
            {
            case '\t': /* fallthrough */
            case ' ':
                /* continue loop through line */
                continue;
            case '\r':      /* fallthrough */
            case '\n':
                eol = 1;    /* fallthrough */
            case '\0':
                /* break pos switch */
                break;
            default:
                /* comment switch */
                switch (comment)
                {
                case 0:
                    codeline = 1;
                    /* break comment switch */
                    break;
                default:
                    comline = 1;
                    /* break comment switch */
                    break;
                }
                /* continue loop through line */
                continue;
            }
            /* break loop through line */
            break;
        }
        if (eol == 1)
        {
            eol = 0;
            counter->tot++;
            counter->code += codeline;
            counter->com += comline;
            counter->blank += (codeline == 0 && comline == 0) ? 1 : 0;
            codeline = 0;
            comline = 0;
        }
    }

    fclose(fp);
}

void count_folder(char *dirname, sloc_t *counts)
{
    char            path[BUFSIZ];   /* path to the next file */
    int             idx;            /* index of the NULL byte in 'path' */
    DIR *           dp;
    struct dirent * next;

    strncpy(path, dirname, BUFSIZ - 1);
    path[BUFSIZ - 2] = '\0';
    idx = strlen(path);
    path[idx] = '/';
    idx++;

    if ((dp = opendir(dirname)) == NULL)
    {
        perror(dirname);
        return;
    }

    while ((next = readdir(dp)) != NULL)
    {
        if (*(next->d_name) == '.')
        {
            continue;
        }
        path[idx] = '\0';
        strncat(path, next->d_name, BUFSIZ - idx - 1);
        count_lines(path, counts);
    }

    closedir(dp);
}

void print_sloc(sloc_t *counts)
{
    int     i;
    sloc_t  tots;
    char    s[NUM_MEMBERS][BUFSIZ];
    int     maxn[NUM_MEMBERS] =
    {
        sizeof(STR_LANG) - 1,
        sizeof(STR_TOT) - 1,
        sizeof(STR_CODE) - 1,
        sizeof(STR_COM) - 1,
        sizeof(STR_BLANK) - 1,
        sizeof(STR_FILE) - 1,
    };

    sloc_list_t *   lst = NULL;
    sloc_list_t *   labels = NULL;
    sloc_list_t *   total = NULL;

    tots.tot = 0;
    tots.code = 0;
    tots.com = 0;
    tots.blank = 0;
    tots.files = 0;

    /* put the data in a list that is suited for printing */
    for (i = 0; i < NUM_LANGS; i++)
    {
        if (counts[i].files != 0)
        {
            tots.tot += counts[i].tot;
            tots.code += counts[i].code;
            tots.com += counts[i].com;
            tots.blank += counts[i].blank;
            tots.files += counts[i].files;

            /* check lengths of the entries */
            strcpy(s[IDX_LANG], langs[i].name);
            maxn[IDX_LANG] = MAX(maxn[IDX_LANG], strlen(langs[i].name));
            snprintf(s[IDX_TOT], BUFSIZ, "%d", counts[i].tot);
            maxn[IDX_TOT] = MAX(maxn[IDX_TOT], strlen(s[IDX_TOT]));
            snprintf(s[IDX_CODE], BUFSIZ, "%d", counts[i].code);
            maxn[IDX_CODE] = MAX(maxn[IDX_CODE], strlen(s[IDX_CODE]));
            snprintf(s[IDX_COM], BUFSIZ, "%d", counts[i].com);
            maxn[IDX_COM] = MAX(maxn[IDX_COM], strlen(s[IDX_COM]));
            snprintf(s[IDX_BLANK], BUFSIZ, "%d", counts[i].blank);
            maxn[IDX_BLANK] = MAX(maxn[IDX_BLANK], strlen(s[IDX_BLANK]));
            snprintf(s[IDX_FILE], BUFSIZ, "%d", counts[i].files);
            maxn[IDX_FILE] = MAX(maxn[IDX_FILE], strlen(s[IDX_FILE]));

            add_sloc_item(&lst, counts[i].code, s);
        }
    }

    strcpy(s[IDX_LANG], STR_TOT);
    maxn[IDX_LANG] = MAX(maxn[IDX_LANG], strlen(STR_TOT));
    snprintf(s[IDX_TOT], BUFSIZ, "%d", tots.tot);
    maxn[IDX_TOT] = MAX(maxn[IDX_TOT], strlen(s[IDX_TOT]));
    snprintf(s[IDX_CODE], BUFSIZ, "%d", tots.code);
    maxn[IDX_CODE] = MAX(maxn[IDX_CODE], strlen(s[IDX_CODE]));
    snprintf(s[IDX_COM], BUFSIZ, "%d", tots.com);
    maxn[IDX_COM] = MAX(maxn[IDX_COM], strlen(s[IDX_COM]));
    snprintf(s[IDX_BLANK], BUFSIZ, "%d", tots.blank);
    maxn[IDX_BLANK] = MAX(maxn[IDX_BLANK], strlen(s[IDX_BLANK]));
    snprintf(s[IDX_FILE], BUFSIZ, "%d", tots.files);
    maxn[IDX_FILE] = MAX(maxn[IDX_FILE], strlen(s[IDX_FILE]));
    add_sloc_item(&total, tots.tot, s);

    strcpy(s[IDX_LANG], STR_LANG);
    strcpy(s[IDX_TOT], STR_TOT);
    strcpy(s[IDX_CODE], STR_CODE);
    strcpy(s[IDX_COM], STR_COM);
    strcpy(s[IDX_BLANK], STR_BLANK);
    strcpy(s[IDX_FILE], STR_FILE);
    add_sloc_item(&labels, 0, s);

    print_sloc_list(labels, maxn);
    print_sloc_list(total, maxn);
    print_sloc_list(lst, maxn);

    free_sloc_list(lst);
    free_sloc_list(labels);
    free_sloc_list(total);
}

void add_sloc_item(sloc_list_t **lst, int idx, char s[][BUFSIZ])
{
    sloc_list_t **  cur = lst;
    sloc_list_t *   new;

    new = (sloc_list_t *)malloc(sizeof(sloc_list_t));
    if (new == NULL)
    {
        perror("malloc");
        return;
    }
    new->idx = idx;
    new->name = strdup(s[IDX_LANG]);
    new->tot = strdup(s[IDX_TOT]);
    new->code = strdup(s[IDX_CODE]);
    new->com = strdup(s[IDX_COM]);
    new->blank = strdup(s[IDX_BLANK]);
    new->files = strdup(s[IDX_FILE]);

    for (;;)
    {
        if (*cur == NULL || (*cur)->idx < new->idx)
        {
            new->next = *cur;
            *cur = new;
            return;
        }
        cur = &((*cur)->next);
    }
}

void free_sloc_list(sloc_list_t *lst)
{
    sloc_list_t *cur;
    sloc_list_t *next;

    cur = lst;
    while (cur != NULL)
    {
        next = cur->next;
        free(cur->name);
        free(cur->tot);
        free(cur->code);
        free(cur->com);
        free(cur->blank);
        free(cur->files);
        free(cur);
        cur = next;
    }
}

void print_sloc_list(sloc_list_t *list, int *lens)
{
    sloc_list_t *   cur = list;

    while (cur != NULL)
    {
        print_member(cur->name, lens[IDX_LANG]);
        print_member(cur->files, lens[IDX_FILE]);
        print_member(cur->code, lens[IDX_CODE]);
        print_member(cur->com, lens[IDX_COM]);
        print_member(cur->blank, lens[IDX_BLANK]);
        print_member(cur->tot, lens[IDX_TOT]);

        putchar('\n');

        cur = cur->next;
    }
}

void print_member(char *s, int n)
{
    int i;

    for (i = 0; i < n - strlen(s); i++)
    {
        putchar(' ');
    }
    printf("%s"SPACES, s);
}
