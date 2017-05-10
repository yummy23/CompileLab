#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"

#include "table.h"
#include "tree.h"

LogName logname = NULL;
ImperStack Top = NULL;
Table table;
int TableSize = 0;
//#define __DEBUG  
void addName(char *name){
#ifdef __DEBUG
    printf("ID =%s\n",name);
#endif
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
    Top->e = malloc(sizeof(struct SymbolEntry_));
    Top->e->stack_next=NULL;
    Top->next=NULL;
    table = malloc(TableSize *sizeof(struct Table_));

    int i;
    LogName p = logname;
    for (i=0;i<TableSize;i++){

        table[i].e = malloc(sizeof(struct SymbolEntry_));
        table[i].e->stack_next = NULL;
        table[i].e->table_next = NULL;
        table[i].name = p->name;
        p=p->next;
    }
}

int addToImperSlot(SymbolEntry e){
    /* need search the recent slot. ret: redec:lineno; succ:0 */
    SymbolEntry p = Top->e->stack_next;
    while(p!=NULL){
#ifdef __DEBUG
        printf("%s %s\n",e->name,p->name);
#endif
        if (0==strcmp(e->name,p->name)){
            return p->row;
        }
        p=p->stack_next;
    }
    e->stack_next = Top->e->stack_next;
    Top->e->stack_next = e;
    return 0;
}

/* there is a {new block} */
void ImperStack_push(){
    /* add from head */
    ImperStack s = malloc(sizeof(struct ImperStack_));
    s->e = malloc(sizeof(struct SymbolEntry_));
    s->e->stack_next=NULL;
    s->next=Top;
    Top=s;
}
void ImperStack_pop(){
    ImperStack s=Top;
    Top=Top->next;
    free(s->e);
    free(s);    
}

int halfsearch(char *name){//fail -1, succ: i
    int mid = TableSize/2,l=0,r=TableSize-1,ok=0;
    while (ok<2){
        int val = strcmp(name,table[mid].name);
        if (0 == val){
            return mid;
        }
        else if (0>val){
            mid = (l+mid-1)/2;
        }
        else{
            mid = (r+mid+1)/2;
        }
        ok+=(l==mid||r==mid);
    }
    return -1;
}

/* directed add */
int addToTable(SymbolEntry e){//fail : row succ: 0
    /* add from head */
    int i = halfsearch(e->name);
    if (-1==i){
        printf("-_-`` W T F ! ! !\n");
        exit(0);
    }
    e->table_next = table[i].e->table_next;    
    table[i].e->table_next = e;
    return 0;
}

/* just need to change the value */
void refreshTable(SymbolEntry e, char *name){
    int i = halfsearch(e->name);
    if (-1==i){
        printf("-_-`` W T F ! ! !\n");
        exit(0);
    }
}

int searchTable(char *name){//defined:lineno not:-1
    int i =  halfsearch(name);
    if (i<0){
        printf("-_-`` W T F ! ! !\n");
        exit(0);
    }
    if (table[i].e->table_next==NULL){
        return -1;
    }
    else 
        return i; 
}

Type getType_table(int i){
    return table[i].e->table_next->type;
}

Functype getFunctype_table(int i){
    if (table[i].e->table_next->kind==FUNC)
        return table[i].e->table_next->u.ft;
    return NULL;
}
