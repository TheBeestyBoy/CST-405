/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NUM = 258,
     ID = 259,
     IF = 260,
     ELSE = 261,
     INT = 262,
     RETURN = 263,
     VOID = 264,
     WHILE = 265,
     PLUS = 266,
     MINUS = 267,
     TIMES = 268,
     DIVIDE = 269,
     LT = 270,
     LTE = 271,
     GT = 272,
     GTE = 273,
     EQ = 274,
     NEQ = 275,
     ASSIGN = 276,
     SEMI = 277,
     COMMA = 278,
     LPAREN = 279,
     RPAREN = 280,
     LBRACKET = 281,
     RBRACKET = 282,
     LBRACE = 283,
     RBRACE = 284
   };
#endif
/* Tokens.  */
#define NUM 258
#define ID 259
#define IF 260
#define ELSE 261
#define INT 262
#define RETURN 263
#define VOID 264
#define WHILE 265
#define PLUS 266
#define MINUS 267
#define TIMES 268
#define DIVIDE 269
#define LT 270
#define LTE 271
#define GT 272
#define GTE 273
#define EQ 274
#define NEQ 275
#define ASSIGN 276
#define SEMI 277
#define COMMA 278
#define LPAREN 279
#define RPAREN 280
#define LBRACKET 281
#define RBRACKET 282
#define LBRACE 283
#define RBRACE 284




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 26 "src/parser.y"
{
    int number;
    char *string;
    struct ASTNode *node;
}
/* Line 1529 of yacc.c.  */
#line 113 "src/parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

