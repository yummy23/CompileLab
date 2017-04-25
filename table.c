#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"

#include "table.h"
#include "tree.h"

#define __DEBUG 1

#ifdef __DEBUG

#endif
LogName logname = NULL;
ImperStack Top = NULL;
Table table;
int TableSize = 0;

void addName(char *name){
    /* add from head */
    LogName ln = malloc(sizeof(struct LogName_));
    ln->name = name;
    if (NULL == logname){
        logname = ln;
        ln->next = NULL;
        TableSize++;
        return;
    }
    int val;
    val = strcmp(name,logname->name);
    if (val<0){
        ln->next = logname;
        logname = ln;
        TableSize++;
        return;
    }
    else if (0==val){
        return;
    }

    LogName p=logname;
    while(NULL!=p->next){
        val = strcmp(name,p->next->name);
        if (val<0){
            ln->next = p->next;
            p->next = ln;
            TableSize++;
            return;
        }
        else if (0 == val){
            return;
        }
        else{
            p = p->next;
            return;
        }
    }
    if (NULL == p->next){
        p->next = ln;
        TableSize++;
    }
}

void Table_init(){  
    SearchTree();

    Top = malloc(sizeof(struct ImperStack_));
    Top->e=NULL;
    Top->next=NULL;
    table = malloc(TableSize *sizeof(struct Table_));
    int i;
    LogName p = logname;
    for (i=0;i<TableSize;i++){
        table[i].e = malloc(sizeof(struct SymbolEntry_));
        table[i].e->stack_next = NULL;
        table[i].e->table_next = NULL;
        table[i].name = p->name;
#ifdef __DEBUG
        printf("%d  %s\n",i,table[i].name);
#endif
        p=p->next;
    }
}

void addToStack(SymbolEntry e){
    e->stack_next = Top->e;
    Top->e = e;
}

/* there is a {new block} */
void addToStack_new(SymbolEntry e){
    /* add from head */
    ImperStack s = malloc(sizeof(struct ImperStack_));
    s->e = e;
    s->next=Top;
    Top=s;
}

int halfsearch(char *name){
    int mid = TableSize/2,l=0,r=TableSize-1,ok=0;
    while (ok<2){
        int val = strcmp(name,table[mid].name);
        if (0 == val){
            return mid;
        }
        else if (0<val){
            mid = (l+mid)/2;
        }
        else{
            mid = (r+mid)/2;
        }
        ok+=(l==mid||r==mid);
    }
    return -1;
}

/* first add */
void addToTable(SymbolEntry e, char *name){
    /* add from head */
    int i = halfsearch(name);
    if (-1==i){
        printf("-_-`` W T F ! ! !\n");
        exit(0);
    }
    e->table_next = table[i].e->table_next;    
    table[i].e->table_next = e;
}

/* readd */
void re_addToTable(SymbolEntry e, char *name){


}


