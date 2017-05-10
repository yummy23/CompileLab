#ifndef _TABLE_H
#define _TABLE_H

#include "stdio.h"

#define INTTYPE 0
#define FLOATTYPE 1
#define FUNCTYPE 2
#define FROMSTRUCT 3
#define FROMOTHER  4
/* type */
typedef struct Type_ *Type;
typedef struct FieldList_ *FieldList;
typedef struct Functype_ *Functype;

struct Type_{
    enum {BASIC, ARRAY, STRUCTER}kind;
    union
    {
        // 基本类型,int?float?
        int basic;
        // 数组类型信息包括元素类型与数组大小构成
        struct { Type elem; int size; } array;
        // 结构体类型信息是一个链表
        FieldList structure;
    } u;
};

struct FieldList_
{
    char* name; // 域的名字
    Type type;  // 域的类型
    FieldList next; // 下一个域
};

struct Functype_
{
	char*name;
	int row;
	Type retype;
	FieldList param;
};


/* table */
typedef struct ImperStack_ *ImperStack;
typedef struct Table_ *Table;
typedef struct FuncName_ *FuncName;
typedef struct SymbolEntry_ *SymbolEntry;
typedef struct Var_ *Var;

struct ImperStack_{
    ImperStack next;
    SymbolEntry e;
};

struct Table_{//NULL head
    char *name;/* the name for each SymbolEntry */
    SymbolEntry e;
};
//TODO in yuyifenxi don't need to record value,!!KISS
struct SymbolEntry_{
    Type type;
    int row; 
    char *name;
    enum {VAR,FUNC,STRUCTURE}kind;
    union{
        char *ProgramName;
        Functype ft;
        Var var;
    }u;
    SymbolEntry stack_next;
    SymbolEntry table_next;
};

struct Var_{
    union{
	//int val_int;
	//float val_float;
    }val;
};


/* Log all name */
typedef struct LogName_ *LogName;

struct LogName_{
    char *name;
    LogName next;
};



void addName(char *);
void Table_init();

int addToImperSlot(SymbolEntry e);
void ImperStack_push();
void ImperStack_pop();

int addToTable(SymbolEntry);//fail:row succ:0
void refreshTable(SymbolEntry e,char *name);
int searchTable(char *name);//define:row not:-1
Type getType_table(int i);
Functype getFunctype_table(int i);

#endif
