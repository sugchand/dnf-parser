#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef DNF_HELPER_H
#define DNF_HELPER_H
/*****************************************************************************/
#define MAX_PARAMS 100
#define MAX_PARAM_VALUES 10
#define MAX_CLASS_OBJ 1000

#define CONTAINER_OF(ptr, type, member) ({            \
 const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
  (type *)( (char *)__mptr - offsetof(type,member) );})
#define snprintf_safe(...) (snprintf(__VA_ARGS__) < 0 ? abort() : (void)0)

struct dnf_obj_class;

struct dnf_obj {
    const struct dnf_obj_class *obj_class;
    /* Level of a specific object in the hierarchy */
    unsigned int level;
    // Point to the parent if exist, NULL for the global parent
    struct dnf_obj *parent;
    // Point to the first child in the list
    struct dnf_obj *child;
    // Point to nodes at same level
    struct dnf_obj *next;
};

struct dnf_obj_class {
    /* Type of DNF obj class */
	char type[1000];
    struct dnf_obj *(*alloc)(void);
    void (*set_values)(struct dnf_obj *obj, char *key, void *data, int type);
    void (*update_parent)(struct dnf_obj *obj, struct dnf_obj *parent);
    void (*update_child)(struct dnf_obj *obj, struct dnf_obj *child);
    void (*dealloc)(struct dnf_obj *obj);
};

/*****************************************************************************/
struct dnf_stack_value {
    struct dnf_obj *obj;
};

struct dnf_stack {
    struct dnf_stack_value element;
    struct dnf_stack *next;
};
int dnf_stack_push(struct dnf_obj *obj);
struct dnf_obj *dnf_stack_pop(void);
void free_dnf_stack(void);

/*****************************************************************************/

/* Using a simple array for the map implementation now,
 * it must be replaced with hash based implementation to scale
 */
struct dnf_obj_class_map {
	const struct dnf_obj_class *class_obj[MAX_CLASS_OBJ];
	int tot_cnt;
};

void register_dnf_obj_class(const struct dnf_obj_class *class_obj);
const struct dnf_obj_class *get_dnf_obj_class(char *type);

/*****************************************************************************/
struct dnf_obj *rootDnfNode;

void set_dnf_root_node(struct dnf_obj *obj);
struct dnf_obj *get_dnf_root_node(void);
unsigned int get_dnf_obj_level(void);
void increment_dnf_obj_level(void);
void decrement_dnf_obj_level(void);
int update_dnf_obj_in_tree(struct dnf_obj *curr);
void free_dnf_tree(struct dnf_obj *dnf_element);
/*****************************************************************************/

int yylex();
int yyerror(char *s);
int yyparse();

#endif /* DNF_HELPER_H */