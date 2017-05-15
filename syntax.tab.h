/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    HEX = 259,
    OCT = 260,
    FLOAT = 261,
    SEMI = 262,
    COMMA = 263,
    ASSIGNOP = 264,
    RELOP = 265,
    LT = 266,
    LE = 267,
    GT = 268,
    GE = 269,
    EQ = 270,
    NE = 271,
    PLUS = 272,
    SUB = 273,
    MUL = 274,
    DIV = 275,
    AND = 276,
    OR = 277,
    DOT = 278,
    NOT = 279,
    TYPE = 280,
    STRUCT = 281,
    RETURN = 282,
    IF = 283,
    ELSE = 284,
    WHILE = 285,
    LP = 286,
    RP = 287,
    LB = 288,
    RB = 289,
    LC = 290,
    RC = 291,
    ID = 292,
    MINUS = 293,
    STAR = 294,
    LOWER_THAN_ELSE = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 12 "./syntax.y" /* yacc.c:1909  */

	unsigned int type_int;
	float type_float;
	double type_double;
	char *type_str;
	Node *node;

#line 103 "./syntax.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */
