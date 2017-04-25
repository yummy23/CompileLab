#ifndef _TABLE_H
#define _TABLE_H

#include "stdio.h"

#define INTTYPE 0
#define FLOATTYPE 1

/* type */
typedef struct Type_ *Type;
typedef struct FieldList_ *FieldList;

struct Type_{
    enum {BASIC, ARRAY, STRUCTER}kind;
    union
    {
        // 基本类型
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

struct Table_{
    char *name;/* the name for each SymbolEntry */
    SymbolEntry e;
};

struct SymbolEntry_{
    int type; /* what the name for? */
    int isDefinited;
    union{
        char *ProgramName;
        FuncName fn;
        Var var;
    }u;
    SymbolEntry stack_next;
    SymbolEntry table_next;
};

struct FuncName_ {
    int tp;
    Type type;
    //int param_nr;
};

struct Var_{
    int tp; /* BASIC, ...*/
    Type type;
};

/* Log all name */
typedef struct LogName_ *LogName;

struct LogName_{
    char *name;
    LogName next;
};

void addName(char *);
void Table_init();
void addToStack(SymbolEntry e);
void addToStack_new(SymbolEntry);
void addToTable(SymbolEntry,char *);

#endif
