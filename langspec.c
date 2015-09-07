/**
 * langspec.c
 *
 * Manipulate language specifications.
 *
 * The language specifications are stored in a resizable vector, where each
 * element has a unique name. If a language specification is added with the same
 * name as a specification already in the vector, then the old data is
 * overwritten.
 *
 * Mappings between file extensions and language specifications are made in a
 * hash table. The extension string is hashed as the key to retrieve the index
 * of the language specification in the vector. If an extension is added when it
 * is already in the table, then the old value is overwritten.
 *
 * Revision History:
 *       2 Sep 2015     Brian Kubisiak      Initial revision, incomplete.
 *       4 Sep 2015     Brian Kubisiak      Finished basic functionality.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>
#include <assert.h>

#include "langspec.h"

#define DEFAULT_VECTOR_SIZE 64
#define DEFAULT_HASH_SIZE   1024

static struct langspec_t *vect;
static size_t vsize;
static size_t nelems;

static void grow_vector(void)
{
    /* Double the size of the vector. */
    vsize *= 2;
    vect = realloc(vect, vsize * sizeof(struct langspec_t));

    /* Check for failed memory allocation. */
    if (vect == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
}

static int cmp(const void *spec1, const void *spec2)
{
    /* Cast void pointers to language specifications. */
    struct langspec_t *ls1 = (struct langspec_t *)spec1;
    struct langspec_t *ls2 = (struct langspec_t *)spec2;

    /* Use strcmp to determine if the names are equal. */
    return strcmp(ls1->name, ls2->name);
}

static void print_normal(void)
{
    unsigned int i;

    /* For each element with nonzero total count, print counts for language. */
    for (i = 0; i < nelems; i++)
    {
        if (vect[i].total > 0) {
            printf("%s\t%d\t%d\t%d\t%d\t%d\n",
                    vect[i].name,
                    vect[i].files,
                    vect[i].code,
                    vect[i].comment,
                    vect[i].blank,
                    vect[i].total);
        }
    }
}

/*
 * Initialize all shared variables, allocate the required memory.
 */
void init_langspecs(void)
{
    /* Initialize the vector. */
    nelems = 0;
    vsize  = DEFAULT_VECTOR_SIZE;
    vect = (struct langspec_t *)malloc(vsize * sizeof(struct langspec_t));

    /* Check for failed memory allocation. */
    if (vect == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* Initialize hash table, checking for errors. */
    if (hcreate(DEFAULT_HASH_SIZE) == 0) {
        perror("hcreate");
        exit(EXIT_FAILURE);
    }
}

/*
 * Add a new language specification to the vector.
 */
unsigned long add_langspec(const char *name,   const char *startblk,
                           const char *endblk, const char *starteol)
{
    struct langspec_t new;
    struct langspec_t *old;
    unsigned long ret;

    assert(name != NULL);
    if (startblk == NULL)
        assert(endblk == NULL);
    else
        assert(endblk != NULL);

    /* If vector is already full, grow it to allow more elements. */
    if (nelems == vsize) {
        grow_vector();
    }

    /* Copy name for searching vector. */
    new.name = strdup(name);

    /* Check for failed memory allocation. */
    if (new.name == NULL)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    /* Initialize these to NULL so they can be freed. */
    new.startblk = NULL;
    new.endblk   = NULL;
    new.starteol = NULL;

    /* Search for the matching spec in the vector. */
    old = lsearch(&new, vect, &nelems, sizeof(struct langspec_t), cmp);

/* Macro for freeing the old field and replacing it with a copied string,
 * checking the copy for errors. */
#define copy_str(field) {                                   \
    free(old->field);                                       \
    old->field = (field == NULL) ? NULL : strdup(field);    \
    if (field != NULL && old->field == NULL) {              \
        perror("strdup");                                   \
    }                                                       \
}
    /* Whether element was found or added, copy over pointers for spec. */
    copy_str(startblk);
    copy_str(endblk);
    copy_str(starteol);

/* Don't pollute outside namespace. */
#undef copy_str

    /* Reset all the counts. */
    old->files   = 0;
    old->total   = 0;
    old->code    = 0;
    old->comment = 0;
    old->blank   = 0;

    /* Index of 'old' is found from pointer difference from start of vector. */
    ret = old - vect;

    assert(ret < nelems);

    return ret;
}

/*
 * Add a mapping between an extension and a language specification.
 */
void add_langext(unsigned long lang, const char *ext)
{
    ENTRY *old;
    char *key;

    assert(lang < nelems);
    assert(ext != NULL);

    key = strdup(ext);
    if (key == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    /* Look for entry, adding if it doesn't exist. */
    old = hsearch((ENTRY){ .key = key }, ENTER);

    /* Make sure no errors occurred. */
    if (old == NULL) {
        perror("hsearch");
        exit(EXIT_FAILURE);
    }

    /* Set mapping to language spec. */
    old->data = (void *)lang;
}

/*
 * Get the language specification corresponding to the given extension.
 */
struct langspec_t *get_langspec(const char *ext)
{
    ENTRY *spec;
    unsigned long i;

    assert(ext != NULL);

    /* Get index of spec from hash table. */
    spec = hsearch((ENTRY){ .key = (char *)ext }, FIND);

    /* If not in table, return NULL. */
    if (spec == NULL) {
        return NULL;
    }
    /* Else, return spec from vector. */
    else {
        i = (unsigned long)spec->data;
        return &(vect[i]);
    }
}

void free_langspecs(void)
{
    unsigned int i;

    /* Free all malloc'd strings in vector. */
    for (i = 0; i < nelems; i++) {
        free(vect[i].name);
        free(vect[i].startblk);
        free(vect[i].endblk);
        free(vect[i].starteol);
    }

    /* Free the vector itself. */
    free(vect);

    /* Get rid of the hash table. */
    hdestroy();
}

void print_langspecs(void)
{
    /* TODO add pretty-printing */
    print_normal();
}

void print_langs(int sorted)
{
    unsigned int i;

    /* Sort language specs by name if requested. */
    if (sorted) {
        qsort(vect, nelems, sizeof(struct langspec_t), cmp);
    }

    /* Print out names of languages, one per line. */
    for (i = 0; i < nelems; i++) {
        printf("%s\n", vect[i].name);
    }
}
