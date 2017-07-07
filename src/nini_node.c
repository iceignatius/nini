#include "nini_node.h"

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
}
//------------------------------------------------------------------------------
const char* nini_node_get_name(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get name of node.
     *
     * @param self Object instance.
     * @return Name of this node.
     */
}
//------------------------------------------------------------------------------
nini_type_t nini_node_get_type(const nini_node_t *self)
{
    /**
     * @memberof nini_node_t
     * @brief Get type of node.
     *
     * @param self Object instance.
     * @return Type of this node.
     */
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
