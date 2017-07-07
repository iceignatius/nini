/**
 * @file
 * @brief     Nested INI node.
 * @author    王文佑
 * @date      2017/07/03
 * @copyright ZLib Licence
 */
#ifndef _NINI_NODE_H_
#define _NINI_NODE_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Node types.
 */
typedef enum nini_type_t
{
    NINI_ROOT,      ///< Node is the root section (virtual).
    NINI_SECTION,   ///< Node is a section.
    NINI_STRING,    ///< Node is a key with string value.
    NINI_DECIMAL,   ///< Node is a key with integer value in decimal format.
    NINI_HEXA,      ///< Node is a key with integer value in hexadecimal format.
    NINI_FLOAT,     ///< Node is a key with floating point value.
    NINI_BOOL,      ///< Node is a key with boolean value.
    NINI_NULL,      ///< Node is a key without value.
} nini_type_t;

/**
 * @class nini_node_t
 * @brief Data node.
 */
typedef struct nini_node_t
{
    // WARNING: All values are private!

    struct nini_node_t *parent;
    struct nini_node_t *prev;
    struct nini_node_t *next;

    char *name;

    nini_type_t type;
    union
    {
        struct
        {
            struct nini_node_t *first;
            struct nini_node_t *last;
        } childs;

        char  *string;
        long   integer;
        double floating;
        bool   boolean;

    };

} nini_node_t;

nini_node_t* nini_node_create_section(const char *name);
nini_node_t* nini_node_create_string (const char *name, const char *value);
nini_node_t* nini_node_create_decimal(const char *name, long value);
nini_node_t* nini_node_create_hexa   (const char *name, long value);
nini_node_t* nini_node_create_float  (const char *name, double value);
nini_node_t* nini_node_create_bool   (const char *name, bool value);
nini_node_t* nini_node_create_null   (const char *name);
void nini_node_release(nini_node_t *self);

const char*  nini_node_get_name(const nini_node_t *self);
nini_type_t nini_node_get_type(const nini_node_t *self);

bool nini_node_have_value(const nini_node_t *self);
bool nini_node_have_child(const nini_node_t *self);

nini_node_t* nini_node_get_next_sibling(nini_node_t *self);
nini_node_t* nini_node_get_prev_sibling(nini_node_t *self);

const nini_node_t* nini_node_get_next_sibling_c(const nini_node_t *self);
const nini_node_t* nini_node_get_prev_sibling_c(const nini_node_t *self);

nini_node_t* nini_node_get_first_child(nini_node_t *self);
nini_node_t* nini_node_get_last_child (nini_node_t *self);
nini_node_t* nini_node_find_child     (nini_node_t *self, const char *name);

const nini_node_t* nini_node_get_first_child_c(const nini_node_t *self);
const nini_node_t* nini_node_get_last_child_c (const nini_node_t *self);
const nini_node_t* nini_node_find_child_c     (const nini_node_t *self, const char *name);

const char* nini_node_get_string (const nini_node_t *self);
long        nini_node_get_integer(const nini_node_t *self);
double      nini_node_get_float  (const nini_node_t *self);
bool        nini_node_get_bool   (const nini_node_t *self);

bool nini_node_link_child(nini_node_t *self, nini_node_t *node);
void nini_node_unlink    (nini_node_t *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
