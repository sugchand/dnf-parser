
#include <stdio.h>
#include "host_helper.h"

int yyerror(char *s)
{
    extern int yylineno;
    printf("Error on line : %d,  %s\n", yylineno, s);
    return 0;
}

int main(int argc, char *argv[])
{
    extern FILE *yyin;
/* Enable it to debug the parsing of tokens,
 * make sure bison command is provided with -t.
 * .output file provide insights on parser stages
 */
//#ifdef YYDEBUG
//      yydebug = 1;
//#endif

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            perror(argv[1]);
            return 1;
        }
    }
    else {
        printf("Reading the input from stdin\n");
    }
    hostConfigInit();
    if (!yyparse()) {
        printf("Successfully completed the token parsing\n");
    }
    printHostConfig();

    // Free up the memory allocated for the dnf nodes and stack
    free_dnf_tree(get_dnf_root_node());
    free_dnf_stack();
    fclose(yyin);
    return 0;
}