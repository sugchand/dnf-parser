/* Host data structure to hold the data.
 */
#include "dnf_helper.h"

#ifndef HOST_HELPER_H
#define HOST_HELPER_H


enum paramKeyValType {
    int_param = 1,
    float_param,
    str_param,
    enum_param,
    paramKeyValTypeEnd
};

struct enumValue {
    char enumName[500];
    int enumVal;
};

struct enumValueSet {
    unsigned int tot_cnt;
    struct enumValue values[MAX_PARAMS];
};

union paramKeyValueData {
    char strValue[500];
    int intValue;
    float floatValue;
    struct enumValueSet enumVal;
};

struct paramKeyValue {
    char key[500];
    union paramKeyValueData value;
};

/* Hostconfig holds a key value pair with its type.
 * a Key can be a string value to specify the name of value.
 * value can be any type of data to store
 */
struct hostConfig {
    struct dnf_obj dnf_data;
    int type; /* type of obj */
    struct paramKeyValue keyData;
};

void printHostConfig(void);
void hostConfigInit(void);
int hostConfigAdd(char *key, void *value, int type);

#endif /* HOST_HELPER_H */