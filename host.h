/* Host data structure to hold the data.
 */

#define MAX_PARAMS 100
/* Must update both structure at the same time*/
enum nodeType {
    tomahawk = 0
};

enum paramType {
    int_param = 0,
    float_param,
    str_param,
    enum_param
};

struct enumValue {
    char enumName[500];
    int enumVal;
};

struct enumValueSet {
    unsigned int tot_cnt;
    struct enumValue values[MAX_PARAMS];
};

union hostParamValue {
    char strValue[500];
    int intValue;
    float floatValue;
    struct enumValueSet enumVal;
};

struct hostParam {
    char name[500];
    enum paramType type;
    union hostParamValue defaultVal;
};

struct hostConfig {
    enum nodeType type;
    struct hostParam params[MAX_PARAMS];
    unsigned int tot_params;
};

extern struct hostConfig hostConf;
void printHostConfig(void);



/*****************************************************************************/
/* All keyword tokens must be defined here */
enum tokenVal {
        /* Last token in the set , do not add any token after this */
            INVALID_TOKEN
};

struct dnf_stack {
    enum tokenVal value;
    struct dnf_stack *next;
};

extern struct dnf_stack *dnf_stack_top;
