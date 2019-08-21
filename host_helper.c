/* Debug functions for host operations */
#include <stdio.h>
#include "host.h"
#include "stdlib.h"

char *getNodeTypeString(enum nodeType type)
{
    switch(type) {
        case 0:
            return "tomahawk";
            break;
        default:
            return "Wrong NodeType";
            break;
    }
}

char *getParamTypeString(enum paramType paramType)
{
    switch(paramType) {
        case 0:
            return "Int";
            break;
        case 1:
            return "float";
            break;
        case 2:
            return "enum";
            break;
    }
}

void printEnumDataSet(struct enumValueSet *value)
{
    int i;
    printf("\n Enum set: \n");
    for (i = 0; i < value->tot_cnt; i++)
    {
        printf("\n %s - %d", value->values[i].enumName, value->values[i].enumVal);
    }
}

void printHostParamValue(union hostParamValue *value, enum paramType type)
{
    switch(type) {
        case 0:
            printf("\n Data is %d", value->intValue);
            break;
        case 1:
            printf("\n Data is %f", value->floatValue);
            break;
        case 2:
            printf("\n Data is %s", value->strValue);
            break;
        case 3:
            printEnumDataSet(&value->enumVal);
            break;
        default:
            printf("\n Wrong param data type ");
            break;
    }
}

void printHostParam(struct hostParam *param)
{
    printf("\n Param name : %s", param->name);
    printf("\n Param type : %s", getParamTypeString(param->type));
    printf("\n Default value of param");
    printHostParamValue(&param->defaultVal, param->type);
}

/* Printing the contents of hostconfig structure */
void printHostConfig(void)
{
    int i = 0;
    printf("\n ********** Host Configuration ********** \n");
    printf("NodeType : %s \n", getNodeTypeString(hostConf.type));
    for(i = 0; i < hostConf.tot_params; i++) {
        printHostParam(&hostConf.params[i]);
    }

}

struct hostConfig hostConf;

/*****************************************************************************/
struct dnf_stack *dnf_stack_top = NULL;
int dnf_stack_push(enum tokenVal value)
{
    struct dnf_stack *new_val;
    new_val = (struct dnf_stack *)malloc(sizeof (struct dnf_stack));
    new_val->next = dnf_stack_top;
    new_val->value = value;
    dnf_stack_top = new_val;
}

enum tokenVal dnf_stack_pop(void)
{
    if (dnf_stack_top != NULL || dnf_stack_top->value != INVALID_TOKEN)
    {
        struct dnf_stack *element = dnf_stack_top;
        enum tokenVal value = element->value;
        dnf_stack_top = dnf_stack_top->next;
        free(element);
        return value;
    }
    return INVALID_TOKEN;
}

void print_dnf_stack(struct dnf_stack *element)
{
    if (element != NULL || element->value != INVALID_TOKEN)
    {
        printf("\n Element is %d", element->value);
        print_dnf_stack(element->next);
    }
}
