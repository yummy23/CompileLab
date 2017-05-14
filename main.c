#include "tree.h"
#include "l_sdt.h"

extern int errorFlag;
extern FILE* yyin;
extern int yy_flex_debug;
int main(int argc, char** argv) {
	if (argc <= 1) return -1;
	FILE* f = fopen(argv[1],"r");
	if (!f)
	{
		perror(argv[1]);
		return 1;
	}	
	int yy_flex_debug=1;
	int yylineno=1;

	yyrestart(f);
	yyparse();
	if(!errorFlag){
		printTree();
		Table_init();
       	Program(root);
    }
}

