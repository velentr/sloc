%{

#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

#define SAVE_STRING (yylval.string = strdup(yytext))

#define TOKEN(t)    (yylval.token = t)

%}

%%

 /* Keywords */
"name"              return TOKEN(TNAME);
"startblk"          return TOKEN(TSTARTBLK);
"endblk"            return TOKEN(TENDBLK);
"starteol"          return TOKEN(TSTARTEOL);
"addlanguage"       return TOKEN(TADDLANG);
"extension"         return TOKEN(TADDEXT);

 /* Save string literals, escaping quotes. */
\"(\\.|[^\n"\\])*\" SAVE_STRING; return TSTRING;

 /* Delimeters for marking a group of assignments. */
"{"                 return TOKEN(TLDELIM);
"}"                 return TOKEN(TRDELIM);

 /* Assignment to an atom. */
"="                 return TOKEN(TASSIGNMENT);

 /* End-of-statement delimeter. */
";"                 return TOKEN(TENDSTMT);

 /* Eat up whitespace. */
[ \t\n\r\v\f]       ;

 /* Everything else should be considered garbage. */
.                   fprintf(stderr, "Bad token: '%s'\n", yytext); yyterminate();

%%

/* No idea why I need this... */
int yywrap(void) { return 1; }
