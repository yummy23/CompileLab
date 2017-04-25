#include "string.h"

#include "l_sdt.h"
#include "table.h"

#define __DEBUG 1

#ifdef __DEBUG

#endif

#ifdef __DEBUG
void printName(char *s){
    printf("****  %s\n",s);
}

void printTag(char *s){
    printf("#### %s\n",s);
} 
#endif


/* init syntactic unit, all "this" program */
void Program(Node *n){
#ifdef __DEBUG
    printName(n->name);
#endif
    ExtDefList(n->child);
}

/* xxList */
void ExtDefList(Node *n){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi=n->child;
    if (NULL!=chi){
        ExtDef(chi);
        if (NULL!=chi->brother)
            ExtDefList(chi->brother);
    }
}

/* definition of a global var | structer | function */
void ExtDef(Node *n){
    Node *chi=n->child;
    Type type=Specifier(chi);

    chi=chi->brother;
    if (0==strcmp(chi->name,"SEMI")){
#ifdef __DEBUG
        printTag("SEMI");
#endif
        return;    
    }
    if (0==strcmp(chi->name,"ExtDecList")){
        ExtDecList(chi);
        chi=chi->brother;
        if (0==strcmp(chi->name,"SEMI")){
#ifdef __DEBUG
            printTag("SEMI");
#endif
            return;    
        }
    }
    else if (0==strcmp(chi->name,"FunDec")){
        FunDec(chi,type);
        chi=chi->brother;
        if (0==strcmp(chi->name,"CompSt")){
            CompSt(chi);
        }
    }
    else{
        printf("- _ -`` A O ~ ~ ~\n");
    }

}

/* xxList */
void ExtDecList(Node *n){
#ifdef __DEBUG
    printName(n->name);
#endif

}

/* var type: int,float; structer... */
Type Specifier(Node *n){
#ifdef __DEBUG
    printName(n->name);
#endif
    Type type;
    type=malloc(sizeof(struct Type_));
    type->kind=0;
    Node *child = n->child;
    if(strcmp(child->value,"int")==0)//basic
        type->u.basic=INTTYPE;
    else if(strcmp(child->value,"float")==0)
        type->u.basic=FLOATTYPE;
    else{
        type=StructSpecifier(child);
    }
    return type;
}

/* structer's type */
Type StructSpecifier(Node *n){
#ifdef __DEBUG
    printName(n->name);
#endif
    Type type=malloc(sizeof(struct Type_));
    return type;
}

/* function dec */
void FunDec(Node *n, Type type){
#ifdef __DEBUG
    printName(n->name);
#endif
    SymbolEntry
    FuncName fm = malloc(struct FuncName_);
    Node chi = n->child;
    if (0==strcmp(chi->name,"ID")){
        
    }
    else printf("- _ -`` A O ~ ~ ~\n");

}

/* a {block} */
void CompSt(Node *n){
#ifdef __DEBUG
    printName(n->name);
#endif

}

