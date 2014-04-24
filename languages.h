/*
 *  languages.h
 *      header file fore defining the different languages that can be used.
 *      designed to be easily modifiable so languages can be added easily.
 */

#ifndef _LANGUAGES_H_
#define _LANGUAGES_H_

#define MAX_EXTS 4

typedef struct _lang_t
{
    /* the name of the programming language */
    char *name;
    /* an array of file extensions for the language */
    char *ext[MAX_EXTS];
    /* string indicating the start of a block comment */
    char *startblk;
    /* string indicating the end of a block comment */
    char *endblk;
    /* string indicating an end-of-line comment */
    char *eol;
} lang_t;

/*
 *  should be easy to add a new language. The format is:
 *
 *      {lang_name, {ext1, ext2,}, begin_blk_com, end_blk_com, begin_ln_com,},
 *
 *  each of these should be strings. there can only be MAX_EXTS - 1 extensions
 *  in that array, but you should be able to increase MAX_EXTS arbitrarily,
 *  and all the arrays will be filled out with NULL pointers (i.e. no evil
 *  side effects will be introduced).
 */
lang_t langs[] =
{
    {"Ada", {".adb", ".ads",}, NULL, NULL, "--",},
    {"Asm (Intel)", {".asm", ".inc",}, NULL, NULL, ";",},
    {"Asm (AT&T)", {".s",}, "/*", "*/", "#",},
    {"C", {".c", ".h",}, "/*", "*/", "//",},
    {"C++", {".cc", ".cpp", ".hh", ".hpp"}, "/*", "*/", "//",},
    {"Clojure", {".clj", ".edn",}, NULL, NULL, ";",},
    {"COBOL", {".cbl",}, NULL, NULL, "*>",},
    {"D", {".d",}, "/*", "*/", "//",},
    {"Eiffel", {".e",}, NULL, NULL, "--",},
    {"Emacs Lisp", {".el",}, NULL, NULL, ";;",},
    {"Erlang", {".erl", ".hrl",}, NULL, NULL, "%",},
    {"Forth", {".4th", ".fs",}, "( ", ")", "\\ ",},
    {"Fortran", {".f77", ".f95",}, NULL, NULL, "!",},
    {"Go", {".go",}, "/*", "*/", "//",},
    {"Haskell", {".hs", ".lhs",}, "{-", "-}", "--",},
    {"HTML", {".htm", ".html",}, "<!--", "-->", NULL,},
    {"Io", {".io",}, "/*", "*/", "//",},
    {"Java", {".java",}, "/*", "*/", "//",},
    {"JavaScript", {".js",}, "/*", "*/", "//",},
    {"Julia", {".jl",}, NULL, NULL, "#",},
    {"LaTeX", {".tex", ".sty",}, NULL, NULL, "%",},
    {"Lisp", {".lsp", ".lisp",}, NULL, NULL, ";",},
    {"Lout", {".lout",}, NULL, NULL, "#",},
    {"Lua", {".lua",}, "--[[", "]]", "--",},
    {"Make", {"Makefile",}, NULL, NULL, "#",},
    {"Markdown", {".md",}, NULL, NULL, NULL,},
    {"Objective-C", {".m",}, "/*", "*/", "//",},
    {"OCaml", {".ml"}, "(*", "*)", NULL,},
    {"Octave", {".oct",}, "%{", "%}", "%",},
    {"Pascal", {".pas",}, "{", "}", NULL,},
    {"Perl", {".pl", ".pm",}, NULL, NULL, "#",},
    {"PHP", {".php",}, "/*", "*/", "//",},
    {"Prolog", {".pro",}, "/*", "*/", "%",},
    {"Python", {".py",}, "\"\"\"", "\"\"\"", "#",},
    {"R", {".r", ".R",}, NULL, NULL, "#",},
    {"Ruby", {".rb", ".rbw",}, NULL, NULL, "#",},
    {"Scala", {".scala",}, "/*", "*/", "//",},
    {"Scheme", {".scm",}, NULL, NULL, ";",},
    {"Shell", {".sh", ".bash",}, NULL, NULL, "#",},
    {"Smalltalk", {".sm", ".st",}, "\"", "\"", NULL,},
    {"SQL", {".sql",}, "/*", "*/", "--",},
    {"Tcl", {".tcl",}, NULL, NULL, "#",},
    {"Vala", {".vala"}, "/*", "*/", "//",},
    {"Verilog", {".v", ".vh",}, "/*", "*/", "//",},
    {"Vimscript", {".vim",}, NULL, NULL, "\""},
    {"VHDL", {".vhdl", ".vht", ".vhd"}, NULL, NULL, "--",},
};

#endif /* _LANGUAGES_H_ */
