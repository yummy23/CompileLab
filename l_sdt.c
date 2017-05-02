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

////////// high-level definition //////////
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
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi=n->child;
    Type type=Specifier(chi);

    chi=chi->brother;
    if (0==strcmp(chi->name,"SEMI")){
#ifdef __DEBUG
        printTag("SEMI");
#endif
        return;
    }
    /*
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
*/
if (0==strcmp(chi->name,"FunDec")){
    FunDec(chi,type);
    chi=chi->brother;
    if (0==strcmp(chi->name,"CompSt")){
        CompSt(chi,type,FROMOTHER);
    }
}
else{
    printf("- _ -`` A O ~ ~ ~\n");
}

}

/* xxList */
/*
   void ExtDecList(Node *n){
#ifdef __DEBUG
printName(n->name);
#endif
}
*/
/////////// specifiers //////////
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
    /*  else{
        type=StructSpecifier(child);
        }
        */
    return type;
}

/* structer's type */
/*
   Type StructSpecifier(Node *n){
#ifdef __DEBUG
printName(n->name);
#endif
Type type=malloc(sizeof(struct Type_));
return type;
}
*/

//////////// Declarators //////////
FieldList VarDec(Node *n, Type type, int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    FieldList f = NULL;
    if (0==strcmp(chi->name,"ID")){   
        f = malloc(sizeof(struct FieldList_));
        f->name = chi->value;
        f->type = type;

        /*       SymbolEntry e = malloc(sizeof(struct SymbolEntry_));
        //e->u.fn = malloc(sizeof(struct FuncName_));
        e->type = type;
        e->row = chi->row;
        e->name = chi->value;

        e->stack_next = NULL;
        e->table_next = NULL;

        int ret = addToImperSlot(e);
        */      /* if (ret>0){//redef

                   printf("Error type 4 at line %d: Redefined function'%s', which have defined at line %d\n",e->row,e->name,ret);
                   }
                   else{
                   addToTable(e);        
                   }
                   */

    }
    else if (0==strcmp(chi->name,"VarDec")){
        f = VarDec(chi,type,from);
        /*TODO array*/
    }
    return f;
}

SymbolEntry FunDec(Node *n, Type type){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    SymbolEntry e = malloc(sizeof(struct SymbolEntry_));
    //e->u.fn = malloc(sizeof(struct FuncName_));
    e->type = type;
    e->row = chi->row;
    e->name = chi->value;

    e->stack_next = NULL;
    e->table_next = NULL;

    if (0==strcmp(chi->name,"ID")){
        int ret = addToImperSlot(e);
        if (ret>0){//redef
            printf("Error type 4 at line %d: Redefined function'%s', which have defined at line %d\n",e->row,e->name,ret);
        }
        else{
            addToTable(e);        
        }
    }
    ImperStack_push();
    chi = chi->brother;
    chi = chi->brother;
    if (0==strcmp(chi->name,"VarList")){
        e->u.fn = malloc(sizeof(struct FuncName_));
        e->u.fn->param = VarList(chi);
    }
    return e;
}

FieldList VarList(Node *n)
{
    Node* child=n->child;
    FieldList f;
    f=ParamDec(child);
    child=child->brother;
    if(child!=NULL)
    {
        FieldList p=f;
        child=child->brother;
        if(p==NULL)f=VarList(child);
        else{
            while(p->next!=NULL)p=p->next;
            p->next=VarList(child);
        }
    }
    return f;
}

/* xing can */
FieldList ParamDec(Node*n)
{

    Node *child=n->child;
    FieldList f;
    Type type;
    type=Specifier(child);
    f=VarDec(child->brother,type,3);
    return f;
}
////////// Statements /////////
/* a {block} */
void CompSt(Node *n, Type retype,int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    if (from == FROMSTRUCT)
        ImperStack_push();
    Node *chi = n->child;
    chi = chi->brother;
    DefList(chi,FROMOTHER);

    chi=chi->brother;
    StmtList(chi, retype);
    ImperStack_pop();
}

void StmtList(Node *n,Type retype){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    if (chi == NULL) return;

    Stmt(chi,retype);
    chi = chi->brother;
}

void Stmt(Node *n,Type retype){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *child = n->child;
    while(child!=NULL)
    {
        if(strcmp(child->name,"Compst")==0){
            CompSt(child,retype,FROMSTRUCT); 
        }
        else if(strcmp(child->name,"RETURN")==0)
        {
            child=child->brother;
            Expression expr = Exp(child,FROMOTHER);
            if(retype==NULL||expr->type==NULL)return;
            if(!typeEqual(retype,expr->type))
            {
                printf("Error type 8 at line %d: The return type mismatched\n",child->row);
            }
            return;
        }
      /*  else if(strcmp(child->name,"LP")==0)
        {
            child=child->brother;
            Type t=Exp(child);
            if(t!=NULL&&!((t->kind==0||t->kind==3)&&t->u.basic==INTTYPE))
            {
                printf("Error type ? conditional statement wrong type\n");
            }
        }
*/
        else if(strcmp(child->name,"Exp")==0)
        {
            Expression expr;
            expr = Exp(child, FROMOTHER);
        }
        else if(strcmp(child->name,"Stmt")==0)
        {
            Stmt(child,retype);
        }
        child=child->brother;
    }
}

//////////// local definition ///////////
void DefList(Node* n,int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    if (chi == NULL) return; 

    Def(chi,from);
    chi=chi->brother;
    DefList(chi,from);
}

void Def(Node *n, int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    Type type = Specifier(chi);

    chi = chi->brother;
    DecList(chi,type,from);
}

void DecList(Node *n,Type type,int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    Dec(chi,type,from);

}

void Dec(Node *n,Type type,int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    FieldList f = VarDec(chi,type,from);
    SymbolEntry e = malloc(sizeof(struct SymbolEntry_));
    e->u.var = malloc(sizeof(struct Var_));
    e->type = type;
    e->row = chi->row;
    e->name = f->name;
    /* then if use type, must be f->type (maybe array)*/
    chi = chi->brother;
    if (chi == NULL) {}
    else {
        if (f->type->kind == ARRAY)
            printf("Error type 5 at line %d: The type mismatched\n",chi->row);
        else{
            chi = chi->brother;
            Expression expr = Exp(chi,from);
            if(expr!=NULL&&type!=NULL&&!typeEqual(type,expr->type)){
#ifdef __DEBUG
                printf("t1=%d t2=%d\n",expr->type->u.basic,type->u.basic);
#endif
                printf("Error type 5 at line %d: The type mismatched\n",chi->row);
            }
            else{
                if (f->type->u.basic==INTTYPE)
                    e->u.var->val.val_int = expr->val.val_int;
                else if (f->type->u.basic==FLOATTYPE)
                    e->u.var->val.val_float = expr->val.val_float;
            }
        }
    }
    int ret = addToImperSlot(e);
    if(0==ret){
        addToTable(e);
    }
    else{/* redec in local */
        printf("Error type 3 at line %d: Redefined variable'%s', which have defined at line %d\n",e->row,e->name,ret);
    }
}

//////// Expressions ///////////
Expression Exp(Node *n, int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    Expression expr;
    //= malloc(sizeof(struct Expression_));
    //    expr->type = malloc(sizeof(struct Type_));
    if (0==strcmp(chi->name,"Exp")){
        if (strcmp(chi->brother->name,"LB")&&strcmp(chi->brother->name,"DOT")){
            Node *chi2 = chi->brother->brother;
            expr = Exp(chi, from);
            Expression expr2 = Exp(chi2, from);
#ifdef __DEBUG
            printf("^^^^^t1=%d t2=%d\n",expr->type->u.basic,expr2->type->u.basic);
#endif
            if (expr==NULL||expr2==NULL) return NULL;
            if(!typeEqual(expr->type,expr2->type)){
#ifdef __DEBUG
                printf("t1=%d t2=%d\n",expr->type->u.basic,expr2->type->u.basic);
#endif
                printf("Error type 5 at line %d: The type mismatched\n",chi->row);
                return NULL;
            }
            if(0==strcmp(chi->brother->name,"PLUS")){
                if (expr->type->u.basic == INTTYPE)
                    expr->val.val_int += expr2->val.val_int;
                else
                    expr->val.val_float += expr2->val.val_float;

            }
            else if(0==strcmp(chi->brother->name,"ASSIGNOP")){
                if (0==strcmp(chi->child->name,"INT")||0==strcmp(chi->child->name,"FLOAT")){
                    printf("Error type 6 at Line %d: the left_hand side of an assignment must be a variable.\n",chi->row);

                }
            }
            free(expr2);
        }
    }
    else if (0==strcmp(chi->name,"LP")){

    }
    else if (0==strcmp(chi->name,"MINUS")){

    }
    else if (0==strcmp(chi->name,"NOT")){

    }
    else if (0==strcmp(chi->name,"ID")){
        int ret = searchTable(chi->value);
        if (chi->brother == NULL){
            if (ret<0){
                printf("Error type 1 at line %d: Undefined variable '%s'\n",chi->row,chi->value);    
                return NULL;
            }
            expr = malloc(sizeof(struct Expression_));
            expr->type = getType_table(ret);
        }
        else if (chi->brother != NULL){
            if (ret<0){
                printf("Error type 2 at line %d: Undefined function '%s'\n",chi->row,chi->value);
                return NULL;
            }
        } 
    }
    else if (0==strcmp(chi->name,"INT")){
#ifdef __DEBUG
        printName(chi->name);
#endif
        expr = malloc(sizeof(struct Expression_));
        expr->type = malloc(sizeof(struct Type_));
        expr->type->kind = BASIC;
        expr->type->u.basic = INTTYPE;
        expr->val.val_int = atoi(chi->value);
    }
    else if (0==strcmp(chi->name,"FLOAT")){
#ifdef __DEBUG
        printName(chi->name);
#endif
        expr = malloc(sizeof(struct Expression_));
        expr->type = malloc(sizeof(struct Type_));
        expr->type->kind = BASIC;
        expr->type->u.basic = FLOATTYPE;
        expr->val.val_float = atof(chi->value);
    }
    return expr;
}


int typeEqual(Type t1,Type t2){
    if(t1->kind!=t2->kind){
        return 0;
    }
    else{
        if(t1->kind==0)	//basic
        {
            if(t1->u.basic!=t2->u.basic)
                return 0;
        }
        else if(t1->kind==2)	//struct
        {
            //if a struct do not has  a name 
            /*struct TODO
              if(t1->u.structure->name==NULL||t2->u.structure->name==NULL)
              {
              return paramEqual(t1->u.structure->inList,t2->u.structure->inList);								
              }
              if(strcmp(t1->u.structure->name,t2->u.structure->name)!=0)
              */
            return 0;
        }
        else if(t1->kind==1)        //array
        {
            return typeEqual(t1->u.array.elem,t2->u.array.elem);
        }
    }
    return 1;
}
