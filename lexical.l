%{
	#include "tree.h"
	#include "syntax.tab.h"
	extern int errorFlag;	
	extern YYSTYPE yylval;
%}

delim [ \t]
ws {delim}+
digit [0-9]
digit_ [1-9]
letter_ [A-Za-z_]
hex (0x|0X)({digit}|[a-f]|[A-F])*
oct 0[0-7]*
int ({digit_}{digit}*)|0
sfloat (({digit}+\.{digit}*)|({digit}*\.{digit}+))[Ee][+-]?{digit}+
float {sfloat}|((({digit_}{digit}*)|0)\.{digit}+)
id {letter_}({letter_}|{digit})*
comment1 (\/\/)({letter_}|{digit}|{ws})*\n

%option yylineno
%option yywrap

%%

"//"	{
	char c1=input();
	while(c1!='\n')
		c1=input();	
	unput(c1);
	}
"/*"	{
	char c1, c2 = input();
	    do {
		   if (c2 == '\n') 
			   yylineno++;
			   c1 = c2;
			   c2 = input();
		  } while ((c1 != '*' && c1) || (c2 != '/' && c2));
}

{ws}	{/*empty*/}
{hex}	{yylval.node=creatNode("INT",yytext); return INT;}
{oct}	{yylval.node=creatNode("INT",yytext); return INT;}	
{int}	{yylval.node=creatNode("INT",yytext); return INT;}
{float}	{yylval.node=creatNode("FLOAT",yytext); return FLOAT;}
";"		{yylval.node=creatNode("SEMI",yytext); return SEMI;}
","		{yylval.node=creatNode("COMMA",yytext); return COMMA;}
"="		{yylval.node=creatNode("ASSIGNOP",yytext); return ASSIGNOP;}
"<"		{yylval.node=creatNode("RELOP",yytext); return RELOP;}
"<="	{yylval.node=creatNode("RELOP",yytext); return RELOP;}
">"		{yylval.node=creatNode("RELOP",yytext); return RELOP;}
">="	{yylval.node=creatNode("RELOP",yytext); return RELOP;}
"=="	{yylval.node=creatNode("RELOP",yytext); return RELOP;}
"!="	{yylval.node=creatNode("RELOP",yytext); return RELOP;}
"+"		{yylval.node=creatNode("PLUS",yytext); return PLUS;}	
"-"		{yylval.node=creatNode("SUB",yytext); return SUB;}	
"*"		{yylval.node=creatNode("MUL",yytext); return MUL;}
"/"		{yylval.node=creatNode("DIV",yytext); return DIV;}
"&&"	{yylval.node=creatNode("AND",yytext); return AND;}
"||"	{yylval.node=creatNode("OR",yytext); return OR;}
"."		{yylval.node=creatNode("DOT",yytext); return DOT;}
"!"		{yylval.node=creatNode("NOT",yytext); return NOT;}
int|float	{yylval.node=creatNode("TYPE",yytext); return TYPE;}
"("		{yylval.node=creatNode("LP",yytext); return LP;}
")"		{yylval.node=creatNode("RP",yytext); return RP;}
"["		{yylval.node=creatNode("LB",yytext); return LB;}
"]"		{yylval.node=creatNode("RB",yytext); return RB;}
"{"		{yylval.node=creatNode("LC",yytext); return LC;}
"}"		{yylval.node=creatNode("RC",yytext); return RC;}
struct	{yylval.node=creatNode("STRUCT",yytext); return STRUCT;}
return	{yylval.node=creatNode("RETURN",yytext); return RETURN;}
if		{yylval.node=creatNode("IF",yytext); return IF;}
else	{yylval.node=creatNode("ELSE",yytext); return ELSE;}
while	{yylval.node=creatNode("WHILE",yytext); return WHILE;}
\r		{/*another code for huanhang!*/}
\n		{/*yycolumn=1;lineno++;*/}	
{id}	{yylval.node=creatNode("ID",yytext); return ID;}
.		{errorFlag=1;printf("Error type A at Line %d: Mysterious characters '%s'\n",yylineno,yytext);}
%%
