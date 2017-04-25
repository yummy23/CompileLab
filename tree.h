#ifndef _TREE_H
#define _TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#include "table.h"

typedef struct Node_{
	int row;				//词素在源文件的行号
	char name[32];			//语/词法单元的名字
	char value[32];			//int/float/id 的值
	struct Node_ *brother;	//他的兄弟节点
	struct Node_ *child;		//他的孩子节点
}Node;

extern Node *root;	//语法树的根节点
Node* creatNode(char *name , char *value);
void addChild(Node *par, Node *chil);//parent,child
void printTree();
void SearchTree();

#endif
