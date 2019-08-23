/* Debug functions for host operations */
#include <stdio.h>
#include <string.h>
#include "host_helper.h"
#include "dnf_helper.h"

bool is_host_object(struct dnf_node *obj);
static struct hostConfig *dnf_hostConfigCast(const struct dnf_node *obj);

char *getParamTypeString(enum paramKeyValType paramType)
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

void printHostParamKeyValue(struct paramKeyValue *keyval,
                              enum paramKeyValType type)
{
    union paramKeyValueData *data = &keyval->value;
    printf("\n Key            : %s", keyval->key);
    switch(type) {
        case int_param:
            printf("\n Data           : %d", data->intValue);
            break;
        case float_param:
            printf("\n Data           : %f", data->floatValue);
            break;
        case str_param:
            printf("\n Data           : %s", data->strValue);
            break;
        case enum_param:
            printEnumDataSet(&data->enumVal);
            break;
        default:
            printf("\n Wrong param data type ");
            break;
    }
}

/* Print all elements in same level and childrens. */
void printDnfConfig(struct dnf_node *obj)
{
    if (obj)
    {
        /* Check if a node is host node or any other type */
        if(!is_host_object(obj))
        {
            return;
        }
        struct hostConfig *conf;
        conf = dnf_hostConfigCast(obj);
        printf("\nNode type       : %d", conf->type);
        printf("\n dnf tree level : %d", conf->dnf_data.level);
        printHostParamKeyValue(&conf->keyData, conf->type);
        printf("\n");
        /* Print the nodes at same level */
        printDnfConfig(obj->next);
        /* Print the child nodes */
        printDnfConfig(obj->child);
    }
}

/* Printing the contents of hostconfig structure */
void printHostConfig(void)
{
    printf("\n ********** Host Configuration ********** \n");
    struct dnf_node *obj = get_dnf_root_node();
    printDnfConfig(obj);
}


/******************************************************************************/
/************** Call back functions for host config ***************************/
/******************************************************************************/

static struct hostConfig *dnf_hostConfigCast(const struct dnf_node *obj)
{
    return CONTAINER_OF(obj, struct hostConfig, dnf_data);
}

void set_default_host_config(struct hostConfig *conf)
{
    conf->dnf_data.next = NULL;
    conf->dnf_data.parent = NULL;
    conf->dnf_data.child = NULL;
    conf->dnf_data.level = 0;
    conf->type = 0;
}
struct dnf_node *host_config_alloc(void)
{
    /* Allocate the host object */
    struct hostConfig *config =(struct hostConfig *)malloc(sizeof(struct hostConfig));
    if (config != NULL)
    {
        set_default_host_config(config);
    }
    return &config->dnf_data;
}

int insert_key_value(struct paramKeyValue *keyval, int type, char *key,
                    void *data)
{
    if (key == NULL || data == NULL)
    {
        printf("\n Invalid key/data, Cannot populate the data structure");
        return -1;
    }
    strncpy(keyval->key, key, sizeof(keyval->key));
    switch (type) {
        case int_param:
            keyval->value.intValue = *(int *)(data);
            break;
        case str_param:
            strncpy(keyval->value.strValue,(char *)data,
                                            sizeof(keyval->value.strValue));
            break;
        default:
            printf("Invalid data %d", type);
            break;
    }
}

void host_config_set(struct dnf_node *obj, char *key, void *data, int type)
{
    struct hostConfig *config = dnf_hostConfigCast(obj);
    config->type = type;
    insert_key_value(&config->keyData, type, key, data);
}

void host_parent_update(struct dnf_node *obj, struct dnf_node *parent)
{
    if (obj == NULL)
    {
        /* empty object , cannot update */
        return;
    }
    struct hostConfig *host_obj = dnf_hostConfigCast(obj);
    host_obj->dnf_data.parent = parent;
}

void host_child_update(struct dnf_node *obj,
                       struct dnf_node *child)
{
    if (obj == NULL)
    {
        /* Empty parent and cannot update child */
        return;
    }
    struct hostConfig *conf = dnf_hostConfigCast(obj);
    if (conf->dnf_data.child == NULL)
    {
        /* No child is present, as its first child */
        conf->dnf_data.child = child;
        return;
    }
    /* There are children present for this node,
     * add the new child at the end of the list
     */
    struct hostConfig *child_conf = dnf_hostConfigCast(conf->dnf_data.child);
    while(child_conf->dnf_data.next != NULL)
    {
        /* Avoid duplicate */
        if (child == conf->dnf_data.child)
        {
            /* No need to insert as element exists already */
            return;
        }
        child_conf = dnf_hostConfigCast(child_conf->dnf_data.next);
    }
    child_conf->dnf_data.next = child;
}

void host_config_dealloc(struct dnf_node *obj)
{
    if (obj != NULL) {
        struct hostConfig *conf = dnf_hostConfigCast(obj);
        free(conf);
    }
}

static const struct dnf_node_class host_config_class = {
    .type = "host_config",
    .alloc = host_config_alloc,
    .set_values = host_config_set,
    .update_parent = host_parent_update,
    .update_child = host_child_update,
    .dealloc =host_config_dealloc
};

const struct dnf_node_class *get_host_obj_class(void)
{
    return get_dnf_node_class("host_config");
}

bool is_host_object(struct dnf_node *obj)
{
    return (obj->obj_class->set_values == host_config_set);
}

/*
 * Initialization of host configuration and must be
 * done at the start of the program
 */
void hostConfigInit(void)
{
    /* Register a new obj class type */
    register_dnf_node_class(&host_config_class);
    const struct dnf_node_class *host_class = get_host_obj_class();
    struct dnf_node *obj = host_class->alloc();
    struct hostConfig *hostconf = dnf_hostConfigCast(obj);
    hostconf->dnf_data.obj_class = host_class;
    hostconf->dnf_data.level = 0;
    hostconf->type = int_param;
    strncpy(hostconf->keyData.key, "ROOT", sizeof("ROOT"));
    hostconf->keyData.value.intValue = 0;
    /* Push the obj to the stack to use it for the parsing */
    dnf_stack_push(&hostconf->dnf_data);

    /*****************************************************
     * NOTE :: Updating the dnf root node as hostconfig,
     * as we have no other node type present in the system.
     * It is expected to change this logic based on different
     * types of nodes when introduced
     */
    set_dnf_root_node(&hostconf->dnf_data);
    /* Increment the current object level for parameters */
    increment_dnf_node_level();
}

 /* There are three main cases need to take care
 * 1) New obj is at the same level of last obj.
 * 2) New obj is child of last created obj
 * 3) New obj is at same level of parent of last created obj
 */
int update_host_obj_in_tree(struct hostConfig *conf)
{
    int ret = 0;
    ret = update_dnf_node_in_tree(&conf->dnf_data);
    if (ret < 0)
    {
        printf("\n Failed to update new config in tree ");
    }
    return ret;
}

/*
 * Add a element to a tree hiearchy.
 */
int hostConfigAdd(char *key, void *value, int type)
{
    struct hostConfig *host_obj;
    const struct dnf_node_class *host_class = get_host_obj_class();
    struct dnf_node *obj = host_class->alloc();
    host_obj = dnf_hostConfigCast(obj);
    host_obj->dnf_data.obj_class = host_class;
    host_obj->dnf_data.level = get_dnf_node_level();
    host_obj->dnf_data.obj_class->set_values(&host_obj->dnf_data,
                                key, value, type);
    return update_host_obj_in_tree(host_obj);
}