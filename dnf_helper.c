/*****************************************************************************/
#include "dnf_helper.h"

/*****************************************************************************/
/*********            DNF STACK OPERATIONS ***********************************/
/*****************************************************************************/
struct dnf_stack *dnf_stack_top = NULL;
int dnf_stack_push(struct dnf_node *obj)
{
    struct dnf_stack *new_val;
    new_val = (struct dnf_stack *)malloc(sizeof (struct dnf_stack));
    new_val->next = dnf_stack_top;
    new_val->element.obj = obj;
    dnf_stack_top = new_val;
}

/* Pass a valid memory location to copy the data, as pop will free up */
struct dnf_node *dnf_stack_pop(void)
{
    if (dnf_stack_top != NULL && dnf_stack_top->element.obj != NULL)
    {
        struct dnf_stack *stackTop = dnf_stack_top;
        struct dnf_node *stack_val = dnf_stack_top->element.obj;
        /* Update the stack top to new position */
        dnf_stack_top = dnf_stack_top->next;
        free(stackTop);
        return stack_val;
    }
    return NULL;
}

bool is_dnf_stack_empty(void)
{
    if (dnf_stack_top == NULL)
    {
        return true;
    }
    return false;
}

void free_dnf_stack(void)
{
    struct dnf_stack *stackVal = dnf_stack_top;
    while(stackVal != NULL) {
        struct dnf_stack *nextval = stackVal->next;
        free(stackVal);
        stackVal = nextval;
    }
}

/******************** DNF OBJECT TREE HIERARCHY HANDLERS ******************/
unsigned int curr_dnf_level = 0;
struct dnf_node *rootDnfNode = NULL;

void set_dnf_root_node(struct dnf_node *obj)
{
    rootDnfNode = obj;
}

struct dnf_node *get_dnf_root_node(void)
{
    return rootDnfNode;
}

unsigned int get_dnf_node_level(void)
{
    return curr_dnf_level;
}

void increment_dnf_node_level(void)
{
    curr_dnf_level++;
}

void decrement_dnf_node_level(void)
{
    curr_dnf_level--;
}

int update_dnf_node_in_tree(struct dnf_node *curr)
{
    struct dnf_node *prev = dnf_stack_pop();
    if (prev == NULL) {
        return -1;
    }
    if (prev->level == curr->level)
    {
        /* Both parameters at the same level, so just update the next */
        prev->next = curr;
        if (is_dnf_stack_empty()) {
            /* Need to push back the root element as we cannot empty
             * stack
             */
            dnf_stack_push(prev);
        }
        /* push new element only to the stack */
        dnf_stack_push(curr);
    }
    else if (prev->level < curr->level)
    {
        /* Need to update the parent child relationship */
        curr->obj_class->update_parent(curr, prev);
        prev->obj_class->update_child(prev, curr);
        dnf_stack_push(prev);
        dnf_stack_push(curr);
    }
    else
    {
        /* Need to pop out to find the parent */
        while(prev->level > curr->level)
        {
            prev = dnf_stack_pop();
            if (prev == NULL)
            {
                return -1;
            }
        }
        if (prev->level == curr->level)
        {
            prev->next = curr;
            dnf_stack_push(curr);
        }
        if (prev->level < curr->level)
        {
            curr->obj_class->update_parent(curr, prev);
            prev->obj_class->update_child(prev, curr);
        }
    }
    return 0;
}

void free_dnf_tree(struct dnf_node *dnf_element)
{
    if (dnf_element != NULL)
    {
        free_dnf_tree(dnf_element->next);
        free_dnf_tree(dnf_element->child);
        free(dnf_element);
    }
}

/************** DNF CLASS MAP *********************************************/

struct dnf_node_class_map dnf_map;

void register_dnf_node_class(const struct dnf_node_class *class_obj)
{
    if (class_obj != NULL && dnf_map.tot_cnt < MAX_CLASS_OBJ)
    {
        dnf_map.class_obj[dnf_map.tot_cnt] = class_obj;
    }
    dnf_map.tot_cnt++;
}

/* So naive approach to loop through the array to find a dnf class.
 * Need to replace it with a hash based map in some point in time,
 */
const struct dnf_node_class *get_dnf_node_class(char *type)
{
    int i;
    for(i = 0; i < dnf_map.tot_cnt; i++)
    {
        if (!strncmp(dnf_map.class_obj[i]->type, type,
            sizeof(dnf_map.class_obj[i]->type)))
        {
            return dnf_map.class_obj[i];
        }
    }
    return NULL;
}
