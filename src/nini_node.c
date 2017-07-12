#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "nini_node.h"

//------------------------------------------------------------------------------
static inline
char* clone_string(const char *src)
{
#ifdef __unix__
    return strdup(src);
#else
    size_t len = strlen(src);
    char *str = malloc(len+1);
    if( !str ) return NULL;

    memcpy(str, src, len);
    str[len] = 0;

    return str;
#endif
}
//------------------------------------------------------------------------------
static
nini_node_t* nini_node_create(nini_type_t type, const char *name)
{
    nini_node_t *node = NULL;

    bool succ = false;
    do
    {
        node = calloc(1, sizeof(nini_node_t));
        if( !node ) break;

        node->type = type;

        if( !name ) break;
        node->name = clone_string(name);
        if( !node->name ) break;

        succ = true;
    } while(false);

    if( !succ )
    {
        if( node && node->name )
            free(node->name);

        if( node )
            free(node);

        node = NULL;
    }

    return node;
}
//------------------------------------------------------------------------------
nini_node_t* nini_node_create_section(const char *name)
{
    /**
     * @memberof nini_node_t
     * @brief Create a section node.
     *
     * @param name Name of the new section.
     * @return Instance of the new node if succeed; or
     *         NULL if failed!
     */
    return nini_node_create(NINI_SECTION, name);
}
//------------------------------------------------------------------------------
nini_node_t* nini_node_create_string(const char *name, const char *value)
{
    /**
     * @memberof nini_node_t
     * @brief Create a key node with string value.
     *
     * @param name  Name of the new key.
     * @param value Value of the new key.
     * @return Instance of the new node if succeed; or
     *         NULL if failed!
     */
    nini_node_t *node = NULL;

    bool succ = false;
    do
    {
        node = nini_node_create(NINI_STRING, name);
        if( !node ) break;

        node->value.string = clone_string( value ? value : "" );
        if( !node->value.string ) break;

        succ = true;
    } while(false);

    if( !succ )
    {
        nini_node_release(node);
        node = NULL;
    }

    return node;
}
//------------------------------------------------------------------------------
nini_node_t* nini_node_create_decimal(const char *name, long value)
{
    /**
     * @memberof nini_node_t
     * @brief Create a key node with integer value in decimal format.
     *
     * @param name  Name of the new key.
     * @param value Value of the new key.
     * @return Instance of the new node if succeed; or
     *         NULL if failed!
     */
    nini_node_t *node = nini_node_create(NINI_DECIMAL, name);
    if( !node ) return NULL;

    node->value.integer = value;

    return node;
}
//------------------------------------------------------------------------------
nini_node_t* nini_node_create_hexa(const char *name, long value)
{
    /**
     * @memberof nini_node_t
     * @brief Create a key node with integer value in hexadecimal format.
     *
     * @param name  Name of the new key.
     * @param value Value of the new key.
     * @return Instance of the new node if succeed; or
     *         NULL if failed!
     */
    nini_node_t *node = nini_node_create(NINI_HEXA, name);
    if( !node ) return NULL;

    node->value.integer = value;

    return node;
}
//------------------------------------------------------------------------------
nini_node_t* nini_node_create_float(const char *name, double value)
{
    /**
     * @memberof nini_node_t
     * @brief Create a key node with floating point value.
     *
     * @param name  Name of the new key.
     * @param value Value of the new key.
     * @return Instance of the new node if succeed; or
     *         NULL if failed!
     */
    nini_node_t *node = nini_node_create(NINI_FLOAT, name);
    if( !node ) return NULL;

    node->value.floating = value;

    return node;
}
//------------------------------------------------------------------------------
nini_node_t* nini_node_create_bool(const char *name, bool value)
{
    /**
     * @memberof nini_node_t
     * @brief Create a key node with boolean value.
     *
     * @param name  Name of the new key.
     * @param value Value of the new key.
     * @return Instance of the new node if succeed; or
     *         NULL if failed!
     */
    nini_node_t *node = nini_node_create(NINI_BOOL, name);
    if( !node ) return NULL;

    node->value.boolean = value;

    return node;
}
//------------------------------------------------------------------------------
nini_node_t* nini_node_create_null(const char *name)
{
    /**
     * @memberof nini_node_t
     * @brief Create a key node without value.
     *
     * @param name Name of the new key.
     * @return Instance of the new node if succeed; or
     *         NULL if failed!
     */
    return nini_node_create(NINI_NULL, name);
}
//------------------------------------------------------------------------------
void nini_node_release(nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Destruct and release a node instance.
     *
     * @param self Object instance.
     */
    if( !self ) return;

    if( self->type == NINI_STRING && self->value.string )
        free(self->value.string);

    if( self->type == NINI_ROOT || self->type == NINI_SECTION )
    {
        nini_node_t *node = self->childs.first;
        while( node )
        {
            nini_node_t *node_remove = node;
            node = node->next;

            nini_node_release(node_remove);
        }
    }

    if( self->name )
        free(self->name);

    free(self);
}
//------------------------------------------------------------------------------
bool nini_node_have_value(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Check if node have value.
     *
     * @param self Object instance.
     * @return TRUE if node have value; and FALSE if not.
     */
    switch( self->type )
    {
    case NINI_STRING:
    case NINI_DECIMAL:
    case NINI_HEXA:
    case NINI_FLOAT:
    case NINI_BOOL:
        return true;

    case NINI_ROOT:
    case NINI_SECTION:
    case NINI_NULL:
        return false;
    }

    return false;
}
//------------------------------------------------------------------------------
bool nini_node_have_child(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Check if node have child.
     *
     * @param self Object instance.
     * @return TRUE if node have child; and FALSE if not.
     */
    switch( self->type )
    {
    case NINI_ROOT:
    case NINI_SECTION:
        return self->childs.first;

    case NINI_STRING:
    case NINI_DECIMAL:
    case NINI_HEXA:
    case NINI_FLOAT:
    case NINI_BOOL:
    case NINI_NULL:
        return false;
    }

    return false;
}
//------------------------------------------------------------------------------
nini_node_t* nini_node_find_child(nini_node_t *self, const char *name)
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
    if( !nini_node_have_child(self) || !name ) return NULL;

    for(nini_node_t *node = self->childs.first;
        node;
        node = node->next)
    {
        if( 0 == strcmp(node->name, name) )
            return node;
    }

    return NULL;
}
//------------------------------------------------------------------------------
double nini_node_get_float(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get the floating point value.
     *
     * @param self Object instance.
     * @return The floating point value; or
     *         NAN if the node is not a floating type of node.
     */
    return ( self->type == NINI_FLOAT )?( self->value.floating ):( nan("") );
}
//------------------------------------------------------------------------------
bool nini_node_link_child(nini_node_t *self, nini_node_t *node)
{
    /**
     * @memberof nini_node_t
     * @brief Link a node to this node.
     *
     * @param self Object instance.
     * @param node The node to be linked to this node,
     *             and it will be one of the children of this node if succeed.
     * @return TRUE if succeed; and FALSE if not.
     *         The failure may be caused by one of the following reasons:
     *         * The node is already linked to an other one.
     *         * The node is a virtual node.
     *         * Self node is not a section type or root type of node,
     *           and cannot have a child.
     *
     * @remarks The child node will be managed by its parent,
     *          and it will be released when its parent be destructed.
     *          So, do not release the child node duplicated if it is already linked to a node.
     */
    if( self->type != NINI_ROOT && self->type != NINI_SECTION ) return false;

    if( !node ) return false;
    if( node->type == NINI_ROOT ) return false;  // Node is a virtual node.
    if( node->parent ) return false;  // Node is already linked.

    node->parent = self;

    node->prev = self->childs.last;
    if( self->childs.last ) self->childs.last->next = node;

    self->childs.last = node;
    if( !self->childs.first )
        self->childs.first = node;

    return true;
}
//------------------------------------------------------------------------------
void nini_node_unlink(nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Unlink node from it parent and siblings.
     *
     * @param self Object instance.
     */
    if( !self->parent ) return;  // Not linked to any one.

    nini_node_t *parent = self->parent;
    nini_node_t *prev   = self->prev;
    nini_node_t *next   = self->next;

    if( prev )
        prev->next = next;
    else
        parent->childs.first = next;

    if( next )
        next->prev = prev;
    else
        parent->childs.last = prev;

    self->parent = NULL;
}
//------------------------------------------------------------------------------
