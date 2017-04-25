#include "tree.h"
extern int yylineno;
extern int TableSize;
Node *root;

Node *creatNode(char *name, char *value){
	Node *node = malloc(sizeof(Node));
	node->row = yylineno;
	strcpy(node->name , name);
	strcpy(node->value,value);
	node->child=NULL;
	node->brother=NULL;
	return node;
}

void addChild(Node *par, Node *chil){
	if (par->child==NULL){
		par->row=chil->row;
		par->child = chil;
	}
	else{
		Node *p;
		for (p=par->child;p->brother!=NULL;p=p->brother);
		p->brother=chil;
	}
}

typedef struct Stack_{
	struct Stack_ *link;
	Node *ele;
	int pos_;
}Stack;
Stack *head=NULL;
void push(Node *node,int pos){
	Stack *p=malloc(sizeof(Stack));
	p->pos_=pos;
	p->ele=node;
	p->link=head;
	head=p;
}
Node *pop(int *pos){
	Stack *node=head;
	if(head->link==NULL){
		node=head;
		head=NULL;
	}
	else{
		head=node->link;
	}
		*pos=node->pos_;
	return node->ele;
}

Node *pop_(){
	Stack *node=head;
	if(head->link==NULL){
		node=head;
		head=NULL;
	}
	else{
		head=node->link;
	}
	return node->ele;
}

int stackIsEmpty(){
	if (head==NULL)
		return 1;
	return 0;
}
//为了调试语法树
void printStack(){
	Stack *p;
	for(p=head;p!=NULL;p=p->link)
		printf("%s-",p->ele->name);
	printf("\n");
}

void printTree(){
	Node *p=root;
	int pos=0;//每行缩进的个数
	int bk=0;
	while(1){
		bk++;
		for(;p->child!=NULL;p=p->child){
			if(p->brother!=NULL){
				push(p,pos);
			}
			int i;
			for(i=0;i<pos;i++)
				printf("  ");
			printf("%s (%d)\n",p->name,p->row);
			pos++;
		}
		if(p->brother!=NULL){
			push(p,pos);
		}
		//产生了空串
		int i;
		if (strcmp(p->name,"useless")){
			for(i=0;i<pos;i++)	
				printf("  ");
		}
		if (!strcmp(p->name,"ID")||!strcmp(p->name,"TYPE")){

			printf("%s: %s\n",p->name,p->value);
		}
		else if (!strcmp(p->name, "INT")){
			int tmp;
			sscanf(p->value,"%d",&tmp);
			printf("%s: %d\n",p->name,tmp);
		}
		else if (!strcmp(p->name,"FLOAT")){
			float tmp;
			sscanf(p->value,"%f",&tmp);
			printf("%s: %f\n",p->name,tmp);
		}
		else{
//			printStack();
			if (strcmp(p->name,"useless"))
				printf("%s\n",p->name);
		}
		if(stackIsEmpty()){
			return;
		}
		else{
			p=pop(&pos);
			p=p->brother;
		}
	};
}

void SearchTree(){
	Node *p=root;
	while(1){
		for(;p->child!=NULL;p=p->child){
			if(p->brother!=NULL){
				push(p,0);
			}
		}
		if(p->brother!=NULL){
			push(p,0);
		}	
		//产生了空串			
		if (0==strcmp(p->name,"ID")){
			addName(p->value);
			//printf("%s: %s\n",p->name,p->value);
			
		}
		/*else if (!strcmp(p->name, "INT")){
			int tmp;
			sscanf(p->value,"%d",&tmp);
			printf("%s: %d\n",p->name,tmp);
		}
		else if (!strcmp(p->name,"FLOAT")){
			float tmp;
			sscanf(p->value,"%f",&tmp);
			printf("%s: %f\n",p->name,tmp);
		}
		else{
			printf("%s\n",p->name);
		}
*/
		
		if(stackIsEmpty()){
			return;
		}
		else{
			p=pop_();
			p=p->brother;
		}
	};
}
