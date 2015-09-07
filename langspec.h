/**
 * langspec.h
 *
 * Manipulate language specifications.
 *
 * Contains function prototypes and datatypes for describing programming
 * languages, tracking existing specifications, and managing memory for all
 * language specifications. The datatype also contains counters for recording
 * the lines of code counted.
 *
 * Revision History:
 *       2 Sep 2015     Brian Kubisiak      Initial revision.
 *       4 Sep 2015     Brian Kubisiak      Updated documentation.
 */
#ifndef _LANGSPEC_H_
#define _LANGSPEC_H_


/*
 * langspec_t
 *
 * Structure for holding a language specification and line counts.
 *
 * A language specification holds the language's name (as a string), delimeters
 * for starting and ending block comments (strings), delimeter for starting a
 * comment that goes to the end of the line (string), and integer counts
 * recording lines-of-code. These counts should be initialized to 0.
 */
struct langspec_t {
    char *name;     /*< Language name. */
    char *startblk; /*< String indicating start of block comment. */
    char *endblk;   /*< String indicating end of block comment. */
    char *starteol; /*< String indicating an EOL comment. */
    int total;      /*< Total number of lines counted. */
    int code;       /*< Number of lines containing code. */
    int comment;    /*< Number of lines containing comments. */
    int blank;      /*< Number of lines containing only whitespace. */
    int files;      /*< Number of files counted. */
};

/*
 * init_langspecs
 *
 * Initialize memory for language specification.
 *
 * Allocates memory for the langspec vector, creates the hash table for
 * file-extension mappings, and initializes all shared variables. This function
 * must be called before any other operations can be performed on the language
 * specification vector. If any of the initialization fails, this function will
 * print an error message and exit.
 */
void init_langspecs(void);

/*
 * add_langspec
 *
 * Adds a new language specification to the langspec vector.
 *
 * Creates a new language with the given properties and adds it to the langspec
 * vector. If a language already exists with the given name, then the
 * specification is updated with the new properties. Copies of the input strings
 * are stored, so it is ok if these pointers go out of scope. Returns the index
 * of the lanspec inside the vector, which should be used as an argument for
 * 'add_langext'.
 *
 * This operation completes in O(N) time, where N is the length of the vector.
 *
 * @pre name != NULL
 *
 * @post Returned value is a valid index in the vector.
 *
 * @param name      The name of the programming language.
 * @param startblk  String indicating the start of a block comment, or NULL if
 *                  block comments are not allowed.
 * @param endblk    String indicating the end of a block comment, or NULL if
 *                  block comments are not allowed.
 * @param starteol  String indicating the start of an end-of-line comment, or
 *                  NULL if end-of-line comments are not allowed.
 *
 * @returns Returns the index of the langspec in the langspec vector. This
 *          should be used as a parameter to the 'add_langext' function.
 */
unsigned long add_langspec(const char *name,   const char *startblk,
                           const char *endblk, const char *starteol);

/*
 * add_langext
 *
 * Adds a mapping between a file extension and a language specification.
 *
 * Maps the given filename extension to the given programming language. The
 * programming language is specified using an index into the langspec vector;
 * this should be the return value from 'add_langspec'. The extension is used as
 * a key into a hash table, so multiple extensions can still map to the same
 * language. If the given extension is already mapped to a programming language,
 * then the old mapping is replaced by the new mapping. If an error occurs, an
 * error message is printed and the program will exit.
 *
 * This operation completes in O(1) time.
 *
 * @pre ext != NULL
 * @pre 'lang' is a valid index into the vector.
 *
 * @param lang  Index into the langspec vector pointing to the language to map.
 * @param ext   File extension to map to the given language.
 */
void add_langext(unsigned long lang, const char *ext);

/*
 * get_langspec
 *
 * Get the language specification corresponding to the given extension.
 *
 * Looks up which language the given extension maps to, returning the result. If
 * the given extension has no existing mappings, then NULL is returned.
 * Otherwise, a pointer to an existing language specification will be returned.
 * This object can be used for recording line count data.
 *
 * This operation completes in O(1) time.
 *
 * @pre ext != NULL
 *
 * @param ext   File extension for which to find the language mapping.
 *
 * @returns Returns the language specification corresponding to the given
 *          filename extension.
 */
struct langspec_t *get_langspec(const char *ext);

void free_langspecs(void);

void print_langspecs(void);

void print_langs(int sorted);


#endif /* end of include guard: _LANGSPEC_H_ */
