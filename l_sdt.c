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
        CompSt(chi);
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
    e->name = chi->value;

    e->stack_next = NULL;
    e->table_next = NULL;

    if (0==strcmp(chi->name,"ID")){
        int ret = addToTable(e);        
        if (-1==ret){  //redef
            printf("Error type 4 at line %d: Redefined function'%s', which have defined at line %d\n",e->row,e->name,ret);
        }
        else{
            addToImperSlot(e);
        }
    }
    else printf("- _ -`` A O ~ ~ ~\n");
    /*   chi = chi->brother;
         chi = chi->brother;
         if (0==strcmp(chi->name,"VarList")){
         e->u.fn->param = VarList(chi);
         }
         */
    return e;
}
/*
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
   */

/* xing can */
/*
   FieldList ParamDec(Node*n)
   {

   Node *child=n->child;
   FieldList f;
   Type type;
   type=Specifier(child);
   f=VarDec(child->brother,type,3);
   return f;
   }
   */

/* a {block} */
void CompSt(Node *n){
#ifdef __DEBUG
    printName(n->name);
#endif
    ImperStack_push();
    Node *chi = n->child;
    chi = chi->brother;
    DefList(chi,FROMOTHER);
    /* 
       chi=chi->brother;
       StmtList(chi);
       */
}

//////////// local definition ///////////
void DefList(Node* n,int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    if (n->child == NULL) return;

    Node *chi = n->child;
    Def(chi,from);
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
    e->type = f->type;
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
            Expression expr = Exp(chi,type,from);
#ifdef __DBUGE
            printTag("BUG!!!");
#endif
            if(expr!=NULL&&type!=NULL&&!typeEqual(type,expr->type)){
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
Expression Exp(Node *n, Type type, int from){
#ifdef __DEBUG
    printName(n->name);
#endif
    Node *chi = n->child;
    perror("BUG!!");
#ifdef __DBUGE
            printTag("333BUG!!!");
#endif
    Expression expr = malloc(sizeof(struct Expression_));
    if (0==strcmp(chi->name,"EXP")){

    }
    else if (0==strcmp(chi->name,"LP")){

    }
    else if (0==strcmp(chi->name,"MINUS")){

    }
    else if (0==strcmp(chi->name,"NOT")){

    }
    else if (0==strcmp(chi->name,"ID")){

    }
    else if (0==strcmp(chi->name,"INT")){
#ifdef __DBUGE
            printTag("2BUG!!!");
#endif
        expr->type->kind = BASIC;
        expr->type->u.basic = INTTYPE;
        expr->val.val_int = chi->value;
    }
    else if (0==strcmp(chi->name,"FLOAT")){

    }
#ifdef __DBUGE
            printTag("1 BUG!!!");
#endif

    return expr;
}


int typeEqual(Type t1,Type t2)
{
	if(t1->kind!=t2->kind)
	{
		return 0;
	}
	else
	{
		if(t1->kind==0)	//basic
		{
			if(t1->u.basic!=t2->u.basic)
				return 0;;
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
