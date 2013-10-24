/*
 *  languages.h
 *      header file fore defining the different languages that can be used.
 *      designed to be easily modifiable so languages can be added easily.
 */

#ifndef _LANGUAGES_H_
#define _LANGUAGES_H_

#define MAX_EXTS 3

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

lang_t langs[] =
{
    {"Ada", {".adb", ".ads",}, NULL, NULL, "--",},
    {"Assembly", {".asm", ".s",}, NULL, NULL, ";",},
    {"C", {".c", ".h",}, "/*", "*/", "//",},
    {"C++", {".cc", ".hh",}, "/*", "*/", "//",},
    {"Clojure", {".clj", ".edn",}, NULL, NULL, ";",},
    {"D", {".d",}, "/*", "*/", "//",},
    {"Eiffel", {".e",}, NULL, NULL, "--",},
    {"Erlang", {".erl", ".hrl",}, NULL, NULL, "%",},
    {"Forth", {".4th", ".fs",}, "( ", ")", "\\ ",},
    {"Fortran", {".f77", ".f95",}, NULL, NULL, "!",},
    {"Go", {".go",}, "/*", "*/", "//",},
    {"Haskell", {".hs", ".lhs",}, "{-", "-}", "--",},
    {"HTML", {".htm", ".html",}, "<!--", "-->", NULL,},
    {"Java", {".java",}, "/*", "*/", "//",},
    {"JavaScript", {".js",}, "/*", "*/", "//",},
    {"Julia", {".jl",}, NULL, NULL, "#",},
    {"LaTeX", {".tex",}, NULL, NULL, "%",},
    {"Lisp", {".lsp", ".lisp",}, NULL, NULL, ";",},
    {"Lout", {".lout",}, NULL, NULL, "#",},
    {"Lua", {".lua",}, "--[[", "]]", "--",},
    {"Make", {"Makefile",}, NULL, NULL, "#",},
    {"Markdown", {".md",}, NULL, NULL, NULL,},
    {"MATLAB", {".mat",}, "%{", "%}", "%",},
    {"Objective-C", {".m",}, "/*", "*/", "//",},
    {"OCaml", {".ml"}, "(*", "*)", NULL,},
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
    {"VHDL", {".vhdl",}, NULL, NULL, "--",},
};

#endif /* _LANGUAGES_H_ */
