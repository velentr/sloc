%{
    #include <stdio.h>

    /* Need this header to changed the displayed color of the tiles. */
    #include "config.h"

    /* The lexer is an external function provided by scanner.l. */
    extern int yylex();

    /* Print error messages as they occur. */
    void yyerror(const char *s) { fprintf(stderr, "error: %s\n", s); }
%}

%union {
    char *string;   /* Literal string. */
    int token;      /* Delimeter token in the input. */
}

%token <string> TSTRING
%token <token>  TLDELIM TRDELIM TASSIGNMENT TENDSTMT
%token <token>  TNAME TSTARTBLK TENDBLK TSTARTEOL TADDLANG TADDEXT

/* Begin parsing the input statements. */
%start langdefs

%%

assignment : TNAME TASSIGNMENT TSTRING TENDSTMT { assign_name($3); }
           | TSTARTBLK TASSIGNMENT TSTRING TENDSTMT { assign_startblk($3); }
           | TENDBLK TASSIGNMENT TSTRING TENDSTMT { assign_endblk($3); }
           | TSTARTEOL TASSIGNMENT TSTRING TENDSTMT { assign_starteol($3); }
           | TADDEXT TASSIGNMENT TSTRING TENDSTMT { add_extension($3); }
           ;

assignments : assignment { /* Just parse the assignment on its own. */ }
            | assignments assignment { /* Parse recursively. */ }
            ;

langdef : TADDLANG TLDELIM assignments TRDELIM TENDSTMT { addlanguage(); }
     ;

langdefs : langdef { /* Don't need to do anything except parse statement. */ }
         | langdefs langdef { /* Parse multiple statements recursively. */ }
         ;

%%
