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
#include "nini_type.h"

#ifdef __cplusplus
extern "C" {
#endif

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

        union
        {
            char  *string;
            long   integer;
            double floating;
            bool   boolean;
        } value;

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

static inline
const char* nini_node_get_name(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get name of node.
     *
     * @param self Object instance.
     * @return Name of this node.
     */
    return self->name;
}

static inline
nini_type_t nini_node_get_type(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get type of node.
     *
     * @param self Object instance.
     * @return Type of this node.
     */
    return self->type;
}

bool nini_node_have_value(const nini_node_t *self);
bool nini_node_have_child(const nini_node_t *self);

static inline
nini_node_t* nini_node_get_prev_sibling(nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the previous sibling node.
     *
     * @param self Object instance.
     * @return The previous sibling node if it has; or
     *         NULL if there are no siblings in front.
     */
    return self->prev;
}

static inline
nini_node_t* nini_node_get_next_sibling(nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the next sibling node.
     *
     * @param self Object instance.
     * @return The next sibling node if it has; or
     *         NULL if there are no siblings after.
     */
    return self->next;
}

static inline
const nini_node_t* nini_node_get_prev_sibling_c(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the previous sibling node.
     *
     * @param self Object instance.
     * @return The previous sibling node if it has; or
     *         NULL if there are no siblings in front.
     */
    return nini_node_get_prev_sibling((nini_node_t*)self);
}

static inline
const nini_node_t* nini_node_get_next_sibling_c(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the next sibling node.
     *
     * @param self Object instance.
     * @return The next sibling node if it has; or
     *         NULL if there are no siblings after.
     */
    return nini_node_get_next_sibling((nini_node_t*)self);
}

static inline
nini_node_t* nini_node_get_first_child(nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the first child.
     *
     * @param self Object instance.
     * @return The first child if it has; or
     *         NULL if there are no children.
     */
    return nini_node_have_child(self) ? self->childs.first : NULL;
}

static inline
nini_node_t* nini_node_get_last_child(nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the last child.
     *
     * @param self Object instance.
     * @return The last child if it has; or
     *         NULL if there are no children.
     */
    return nini_node_have_child(self) ? self->childs.last : NULL;
}

nini_node_t* nini_node_find_child(nini_node_t *self, const char *name);

static inline
const nini_node_t* nini_node_get_first_child_c(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the first child.
     *
     * @param self Object instance.
     * @return The first child if it has; or
     *         NULL if there are no children.
     */
    return nini_node_get_first_child((nini_node_t*)self);
}

static inline
const nini_node_t* nini_node_get_last_child_c(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the last child.
     *
     * @param self Object instance.
     * @return The last child if it has; or
     *         NULL if there are no children.
     */
    return nini_node_get_last_child((nini_node_t*)self);
}

static inline
const nini_node_t* nini_node_find_child_c(const nini_node_t *self, const char *name)
{
    /**
     * @memberof nini_node_t
     * @brief Find child by name.
     *
     * @param self Object instance.
     * @param name Name of the child.
     * @return A child that match the name; or
               NULL if not found.
     */
    return nini_node_find_child((nini_node_t*)self, name);
}

static inline
const char* nini_node_get_string(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the string value.
     *
     * @param self Object instance.
     * @return The string value; or
               NULL if the node is not a string type of node.
     */
    return ( self->type == NINI_STRING )?( self->value.string ):( NULL );
}

static inline
long nini_node_get_integer(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the integer value.
     *
     * @param self Object instance.
     * @return The integer value; or
               ZERO if the node is not an integer type (decimal or hexadecimal) of node.
     */
    return ( self->type == NINI_DECIMAL || self->type == NINI_HEXA )?
           ( self->value.integer ):( 0 );
}

double nini_node_get_float(const nini_node_t *self);

static inline
bool nini_node_get_bool(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the boolean value.
     *
     * @param self Object instance.
     * @return The boolean value; or
     *         FALSE if the node is not a boolean type of node.
     */
    return ( self->type == NINI_BOOL )?( self->value.boolean ):( false );
}

bool nini_node_link_child(nini_node_t *self, nini_node_t *node);
void nini_node_unlink    (nini_node_t *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
