#ifndef _L_SDD_H
#define _L_SDD_H

#include <stdio.h>

#include "tree.h"
#include "table.h"

/* high-level definition */
void Program(Node *n);
void ExtDefList(Node *n);
void ExtDef(Node *n);//
void ExtDecList(Node *n);//

/* specifiers */ 
Type Specifier(Node *n);
Type StructSpecifier(Node *n);//
void OptTag(Node *n);//
void Tag(Node *n);//

/* Declarators */
void VarDec(Node *n);//
void FunDec(Node *n,Type type);//
void VarList(Node *n);//
void ParamDec(Node *n);//

/* statements */
void CompSt(Node *n);//
void Stmtlist(Node *n);//
void Stmt(Node *n);//

/* local definition */
void DefList();//
void Def();//
void DecList();//
void Dec();//

#endif
