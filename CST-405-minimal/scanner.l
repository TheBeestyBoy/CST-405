%{
/* LEXICAL ANALYZER (SCANNER)
 * This is the first phase of compilation - breaking input into tokens
 * Flex generates a scanner from these rules that reads character by character
 */
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"  /* Token definitions from Bison */
%}

/* Flex options to suppress warnings for unused functions */
%option nounput
%option noinput

%%

"int"           { return INT; }
"double"        { return DOUBLE; }
"print"         { return PRINT; }

[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.str = strdup(yytext);
    return ID; 
}

[0-9]+\.[0-9]+  { 
    yylval.dbl = atof(yytext);
    return DOUBLE_LITERAL; 
}

[0-9]+          { 
    yylval.num = atoi(yytext);
    return NUM; 
}

"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"="             { return '='; }
";"             { return ';'; }
"("             { return '('; }
")"             { return ')'; }

"//"[^\n]*      { /* Single-line comment - ignore */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* Multi-line comment - ignore */ }

[ \t\n]         { /* Whitespace - ignore */ }

.               { printf("Lexical Error: Unknown character '%c'\n", *yytext); }

%%

/* Required by Flex - signals end of input */
int yywrap() {
    return 1;  /* 1 means no more input files */
}