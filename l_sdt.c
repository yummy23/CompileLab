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
    e->kind = FUNC;

    e->stack_next = NULL;
    e->table_next = NULL;

    if (0==strcmp(chi->name,"ID")){
        int ret = addToImperSlot(e);
        if (ret>0){//redef
            printf("Error type 4 at line %d: Redefined function'%s', which have defined at line %d\n",e->row,e->name,ret);
            return NULL;
        }
        addToTable(e);        
    }
    ImperStack_push();

    e->u.ft = malloc(sizeof(struct Functype_));
    e->u.ft->name = chi->value;
    e->u.ft->row = chi->row;
    e->u.ft -> retype = type;
    chi = chi->brother;
    chi = chi->brother;

    if (0==strcmp(chi->name,"VarList")){
        e->u.ft->param = VarList(chi);
    }
    else{
        e->u.ft->param = NULL;
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
    //add to table
    SymbolEntry e = malloc(sizeof(struct SymbolEntry_));
    e->type = f->type;
    e->row = child->brother->row;
    e->name = f->name;
    e->kind = VAR;
    e->stack_next = NULL;
    e->table_next = NULL;
    int ret = addToImperSlot(e);
    if (ret>0){//redef
        printf("Error type 4 at line %d: Redefined function'%s', which have defined at line %d\n",e->row,e->name,ret);
    }
    else{
        addToTable(e);        
    }
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
    while(child!=NULL){
        if(strcmp(child->name,"Compst")==0){
            CompSt(child,retype,FROMSTRUCT); 
        }
        else if(strcmp(child->name,"RETURN")==0){
            child=child->brother;
            Type expr = Exp(child);
            if(retype==NULL||expr==NULL)return;
            if(!typeEqual(retype,expr)){
            
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
            Exp(child);
        else if(strcmp(child->name,"Stmt")==0)
            Stmt(child,retype);
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
    e->kind = VAR;
    /* then if use type, must be f->type (maybe array)*/
    chi = chi->brother;
    if (chi == NULL) {}
    else {
        if (f->type->kind == ARRAY)
            printf("Error type 5 at line %d: The type mismatched\n",chi->row);
        else{
            chi = chi->brother;
            Type expr = Exp(chi);
            if(expr!=NULL&&type!=NULL&&!typeEqual(type,expr)){
#ifdef __DEBUG
                printf("t1=%d t2=%d\n",expr->u.basic,type->u.basic);
#endif
                printf("Error type 5 at line %d: The type mismatched\n",chi->row);
            }
        }
    }
    int ret = addToImperSlot(e);
    if(0==ret){
        addToTable(e);
    }
    else{//
        printf("Error type 3 at line %d: Redefined variable'%s', which have defined at line %d\n",e->row,e->name,ret);
    }
    
}

//////// Expressions ///////////
Type Exp(Node *n){
#ifdef __DEBUG
        printName(n->name);
#endif
	Node *child=n->child;
	Type type;
	if(strcmp(child->name,"Exp")==0)
	{	
		Node *child2=child->brother;
		if(strcmp(child2->name,"ASSIGNOP")==0)			
		{
			//left value
			Node *leftChild=child->child;
			Type leftType=NULL;
			//because of associative property and priority,it is right
			if(strcmp(leftChild->name,"ID")==0&&leftChild->brother==NULL)
				leftType=Exp(child);
			else if(strcmp(leftChild->name,"Exp")==0&&leftChild->brother!=NULL&&strcmp(leftChild->brother->name,"LB")==0)	//array
				leftType=Exp(child);
			else if(strcmp(leftChild->name,"Exp")==0&&leftChild->brother!=NULL&&strcmp(leftChild->brother->name,"DOT")==0)	//struct
			{
				leftType=Exp(child);

			}
			else
				printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable\n",child->row);
			child2=child2->brother;
			Type rightType=Exp(child2);
			if(leftType==NULL||rightType==NULL)return NULL;
			if(typeEqual(leftType,rightType))return leftType;
			else
			{
				printf("Error type 5 at line %d: Type mismatched for assigment.\n",child->row);
				return NULL;
			}
		}
		else if(strcmp(child2->name,"PLUS")==0||strcmp(child2->name,"MINUS")==0||strcmp(child2->name,"STAR")==0||strcmp(child2->name,"DIV")==0||strcmp(child2->name,"RELOP")==0)		//+ - * /
		{
			Type t=Exp(child);
			child2=child2->brother;
			Type t2=Exp(child2);
			if(t==NULL||t2==NULL)return NULL;
			else if((t->kind==BASIC)&&(t2->kind==BASIC)&&t->u.basic==t2->u.basic)return t;
			else
			{
				printf("Error type 7 at line %d: Type mismatched for operands.\n",child->row);
				return NULL;
			}
		}
		else if(strcmp(child2->name,"LB")==0)	//array
		{
			Type t1=Exp(child);
			//child's child must be a ID,
			if(t1==NULL)return NULL;
			if(t1->kind!=1)
			{
				printf("Error type 10 at line %d: '",child->row);
				return NULL;
			}
			child2=child2->brother;
			Type t2=Exp(child2);
			//printf("array back\n");
			if(t2==NULL)return NULL;
			if(!((t2->kind==BASIC)&&t2->u.basic==INTTYPE))
			{
				printf("Error type 12 at line %d: Operands type mistaken\n",child2->row);
				return NULL;
			}
			return t1->u.array.elem;
		}
	/*	else if(strcmp(child2->name,"DOT")==0)	//struct
		{
			Type t1=Exp(child);
			if(t1==NULL)return NULL;
			if(t1->kind!=2)
			{
				printf("Error type 13 at line %d: Illegal use of '.'\n",child->row);
				return NULL;
			}
			FieldList fl=t1->u.structure->structure;
			child2=child2->brother;
			while(fl!=NULL)
			{
				if(strcmp(fl->name,child2->value)==0)
					return fl->type;
				fl=fl->tail;
			}
			printf("Error type 14 at line %d: Un-existed field '%s‘\n",child2->row,child2->value);
			return NULL;
		}
*/
	}
	else if(strcmp(child->name,"LP")==0)	//() 
	{
		child=child->brother;
		return Exp(child);
	}
	else if(strcmp(child->name,"MINUS")==0)	//-
	{
		child=child->brother;
		Type t=Exp(child);
		if(t==NULL)return NULL;
		if(t->kind!=0)
		{
			printf("Error type 7 at line %d: Operands type mismatched\n",child->row);
			return NULL;
		}
		return t;
	}
	else if(strcmp(child->name,"NOT")==0)	//not
	{
		child=child->brother;
		Type t=Exp(child);
		if(t==NULL)return NULL;
		if(t->kind==0&&t->u.basic==INTTYPE)return t;
		printf("Error type 7 at line %d: Operands type mismatched\n",child->row);
		return NULL;
	}
	else if(strcmp(child->name,"ID")==0&&child->brother!=NULL)	//func
	{
        FieldList f1 = NULL;
        int ret = searchTable(child->value);
        if (ret<0){
			printf("Error type 2 at line %d: Undefined function '%s'\n",child->row,child->value);
            return NULL;
        }
        
        Functype f = getFunctype_table(ret);
        if (f==NULL){
			printf("Error type 11 at line %d: '%s' must be a function\n",child->row,child->value);
			return NULL;
        }

		FieldList param=f->param;
		child=child->brother;
		child=child->brother;
		if(strcmp(child->name,"RP")==0)
		{
			if(param!=NULL)
			{
				printf("Error type 9 at line : The method '%s(",f->name);
				printparam(param);
				printf(")'is not applicable for the arguments '()'\n");
			}
		}
		else
		{
			if(!Args(child,param)){
				printf("Error type 9 at line : The method '%s(",f->name);
				printparam(param);
				printf(")'is not applicable for the arguments '(");
				printargs(child);
				printf(")'\n");
			}
		}
		return f->retype;
	}
	else if(strcmp(child->name,"ID")==0)
	{
        FieldList f = NULL;
        int ret = searchTable(child->value);
        if(ret>=0) {
            Type tp = getType_table(ret); 
            f = malloc(sizeof(struct FieldList_));
            f->name=child->value;
            f->type = tp;
        }
		if(f==NULL)
		{
			printf("Error type 1 at line %d: Undefined variable '%s'\n",child->row,child->value);	
			return NULL;
		}
		return f->type;
	}
	else if(strcmp(child->name,"INT")==0)
	{
		Type t=malloc(sizeof(struct Type_));
		t->kind=BASIC;
		t->u.basic=INTTYPE;
		return t;
	}
	else if(strcmp(child->name,"FLOAT")==0)
	{
		Type t=malloc(sizeof(struct Type_));
		t->kind=BASIC;
		t->u.basic=FLOATTYPE;
		return t;
	}

}


int typeEqual(Type t1,Type t2){
    if(t1->kind!=t2->kind){
#ifdef __DEBUG
        printf("!kind\n");
#endif
        return 0;
    }
    else{
        if(t1->kind==0)	//basic
        {
            if(t1->u.basic!=t2->u.basic){
#ifdef __DEBUG
        printf("!basic\n");
#endif
                return 0;
            }
        }
        else if(t1->kind==2)	//struct
        {
            //if a struct do not has  a name 
            /*struct TODO
              if(t1->u.structure->name==NULL||t2->u.structure->name==NULL)
              {
              return paramEqual(t1->u.structure->structure,t2->u.structure->structure);								
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

int Args(Node* n,FieldList f)
{//printName(n->name);
	if(n==NULL&&f==NULL)return 1;
	else if(n==NULL||f==NULL)return 0;
	Node *child=n->child;
	Type t=Exp(child);
	if(t==NULL)return 1;		//don't need to report the mistake again
	if(!typeEqual(t,f->type))return 0;
	if(child->brother==NULL&&f->next==NULL)return 1;
	else if(child->brother==NULL||f->next==NULL)return 0;
	return Args(child->brother->brother,f->next);
}

void printparam(FieldList f)
{
	while(f!=NULL)
	{
		printtype(f->type);
		f=f->next;
	}
}

void printargs(Node *n)
{
	Node *child=n->child;
	Type t=Exp(child);
	if(t==NULL)return;
	printtype(t);
	child=child->brother;
	if(child==NULL)return;
	child=child->brother;
    printf(",");
	printargs(child);
}

void printtype(Type t){
	if((t->kind==0||t->kind==3)&&t->u.basic==INTTYPE)
		printf(" int ");
	else if((t->kind==0||t->kind==3)&&t->u.basic==FLOATTYPE)
		printf(" float ");
	else if(t->kind==2)
		printf("struct %s ",t->u.structure->name);
	else if(t->kind==1){
		printtype(t->u.array.elem);
		printf("[]");
	}
}

