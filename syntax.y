%{
	#include "tree.h"
	#include "lex.yy.c"
//	#define YYDEBUG 1
//%define api.pure 
//%locations
	extern int TableSize;
	extern int errorFlag;
	int errorFlag=0;
%}
/* declared types */
%union {
	unsigned int type_int;
	float type_float;
	double type_double;
	char *type_str;
	Node *node;
}

/* declared tokens(terminal) */
%token <node> INT HEX OCT
%token <node> FLOAT
%token <node> SEMI COMMA
%token <node> ASSIGNOP RELOP LT LE GT GE EQ NE
%token <node> ADD SUB MUL DIV AND OR
%token <node> DOT NOT
%token <node> TYPE STRUCT RETURN IF ELSE WHILE
%token <node> LP RP LB RB LC RC 
%token <node> ID

%right ASSIGNOP  
%left OR AND RELOP
%left PLUS MINUS STAR DIV
%right NOT
%left DOT LB RB LP RP
%nonassoc LOWER_THAN_ELSE 
%nonassoc ELSE
%nonassoc STRUCT RETURN WHILE

/* declared non-terminals */
%type <node> Program ExtDefList ExtDef ExtDecList Specifier
%type <node> StructSpecifier OptTag Tag VarDec FunDec VarList
%type <node> ParamDec CompSt StmtList Stmt DefList Def DecList
%type <node> Dec Exp Args


%%

/*High-Level Definitions*/
Program : ExtDefList	{$$=creatNode("Program",""); addChild($$,$1); root=$$;}
		;
ExtDefList : ExtDef ExtDefList	{$$=creatNode("ExtDefList","");addChild($$,$1);addChild($$,$2);}
		   | /*empty*/	{$$=creatNode("useless","");}
|error SEMI	{errorFlag=1;}
|error RC	{/*errorFlag=1;*/}
;
ExtDef : Specifier ExtDecList SEMI	{$$=creatNode("ExtDef","");addChild($$,$1);addChild($$,$2); addChild($$,$3);}
	   | Specifier SEMI	{$$=creatNode("ExtDef","");addChild($$,$1); addChild($$,$2);}
| Specifier FunDec CompSt	{$$=creatNode("ExtDef","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
|error SEMI	{errorFlag=1;}
|error RC	{/*errorFlag=1;*/}
;
ExtDecList : VarDec	{$$=creatNode("ExtDecList","");addChild($$,$1);}
| VarDec COMMA ExtDecList	{$$=creatNode("ExtDecList","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
|error SEMI	{errorFlag=1;}
|error RC	{/*errorFlag=1;*/}
;

/*Specifiers*/
Specifier : TYPE	{$$=creatNode("Specifier","");addChild($$,$1);}
		  | StructSpecifier	{$$=creatNode("Specifier","");addChild($$,$1);}
;
StructSpecifier : STRUCT OptTag LC DefList RC	{$$=creatNode("StructSpecifier","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);addChild($$,$5);}
				| STRUCT Tag	{$$=creatNode("StructSpecifier","");addChild($$,$1);addChild($$,$2);}
;
OptTag : ID	{$$=creatNode("OptTag","");addChild($$,$1);}
	   | /*empty*/	{$$=creatNode("OptTag","");}
;
Tag : ID	{$$=creatNode("Tag","");addChild($$,$1);}
;
/*Declarators*/
VarDec : ID	{$$=creatNode("VarDec","");addChild($$,$1);}
|VarDec LB INT RB	{$$=creatNode("VarDec","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);}
;
FunDec : ID LP VarList RP	{$$=creatNode("FunDec","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);}
	   | ID LP RP	{$$=creatNode("FunDec","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
;
VarList : ParamDec COMMA VarList	{$$=creatNode("VarList","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
		| ParamDec	{$$=creatNode("VarList","");addChild($$,$1);}
|error SEMI	{errorFlag=1;}
|error RC	{/*errorFlag=1;*/}
;
ParamDec : Specifier VarDec	{$$=creatNode("ParamDec","");addChild($$,$1);addChild($$,$2);}
;
/*Statements*/
CompSt :  LC DefList StmtList RC	 {$$=creatNode("CompSt","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);}
|error SEMI	{errorFlag=1;}
;
StmtList : Stmt StmtList	{$$=creatNode("StmtList","");addChild($$,$1);addChild($$,$2);}
| /*empty*/	{$$=creatNode("useless","");}
;
Stmt : Exp SEMI	{$$=creatNode("Stmt","");addChild($$,$1);addChild($$,$2);}
| CompSt		{$$=creatNode("Stmt","");addChild($$,$1);}
| RETURN Exp SEMI		{$$=creatNode("Stmt","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| IF LP Exp RP Stmt	{$$=creatNode("Stmt","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);addChild($$,$5);}
| IF LP Exp RP Stmt ELSE Stmt	{$$=creatNode("Stmt","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);addChild($$,$5);addChild($$,$6);addChild($$,$7);}
| WHILE LP Exp RP Stmt	{$$=creatNode("Stmt","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);addChild($$,$5);}
|error RP	{errorFlag=1;}
;
/*Local Definitions*/
DefList : Def DefList	{$$=creatNode("DefList","");addChild($$,$1);addChild($$,$2);}
		| /*empty*/	{$$=creatNode("useless","");}
|error SEMI	{errorFlag=1;}
;
Def : Specifier DecList SEMI	{$$=creatNode("Def","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
	|error SEMI {errorFlag=1;}
;
DecList : Dec	{$$=creatNode("DecList","");addChild($$,$1);}
		| Dec COMMA DecList	{$$=creatNode("DecList","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
;
Dec : VarDec	{$$=creatNode("Dec","");addChild($$,$1);}
	| VarDec ASSIGNOP Exp	{$$=creatNode("Dec","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
;
/*Expressions*/
Exp : Exp ASSIGNOP Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| Exp AND Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}	
| Exp OR Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| Exp RELOP Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| Exp ADD Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| Exp SUB Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| Exp MUL Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| Exp DIV Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| LP Exp RP		{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| SUB Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);}
| NOT Exp	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);}
| ID LP Args RP	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);}
| ID LP RP	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| Exp LB Exp RB	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);addChild($$,$4);}
| Exp DOT ID	{$$=creatNode("Exp","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
| ID	{$$=creatNode("Exp","");addChild($$,$1);}
| INT	{$$=creatNode("Exp","");addChild($$,$1);}
| FLOAT	{$$=creatNode("Exp","");addChild($$,$1);}
;
Args : Exp COMMA Args	{$$=creatNode("Args","");addChild($$,$1);addChild($$,$2);addChild($$,$3);}
	 | Exp	{$$=creatNode("Args","");addChild($$,$1);}
;


%%
int yyerror(char* msg) {
	printf("Error type B at line %d: %s unexpected near '%s'\n",yylineno,msg,yylval.node->value);
}
