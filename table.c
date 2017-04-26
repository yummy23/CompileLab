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

int addToImperSlot(SymbolEntry e){
/* need search the recent slot. ret: redec:lineno; succ:0 */
    SymbolEntry p = Top->e;
    while(p!=NULL){
        if (0==strcmp(e->name,p->name)){
            return p->row;
        }
    }
    e->stack_next = Top->e;
    Top->e = e;
    return 0;
}

/* there is a {new block} */
void ImperStack_push(){
    /* add from head */
    ImperStack s = malloc(sizeof(struct ImperStack_));
    s->next=Top;
    Top=s;
}
void ImperStack_pop(){
    
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
int addToTable(SymbolEntry e){//fail : -1 succ: 1
    /* add from head */
    int i = halfsearch(e->name);
    if (-1==i){
        printf("-_-`` W T F ! ! !\n");
        exit(0);
    }
    if (NULL!=table[i].e->table_next) //redefine
        return -1;
    e->table_next = table[i].e->table_next;    
    table[i].e->table_next = e;
    return 1;
}

/* just need to change the value */
void refreshTable(SymbolEntry e, char *name){
    int i = halfsearch(e->name);
    if (-1==i){
        printf("-_-`` W T F ! ! !\n");
        exit(0);
    }
    

}


