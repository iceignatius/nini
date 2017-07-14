#include <string.h>
#include "nini_helper.h"

//------------------------------------------------------------------------------
static
char* extract_last_name(char **path, char deli)
{
    char *pos = *path ? strrchr(*path, deli) : NULL;
    if( pos )
    {
        *pos = 0;
        return pos + 1;
    }
    else
    {
        char *name = *path;
        *path = NULL;
        return name;
    }
}
//------------------------------------------------------------------------------
static
char* extract_first_name(char **path, char deli)
{
    char *name = *path;

    char *pos = *path ? strchr(*path, deli) : NULL;
    if( pos )
    {
        *pos  = 0;
        *path = pos + 1;
    }
    else
    {
        *path = NULL;
    }

    return name;
}
//------------------------------------------------------------------------------
static
nini_node_t* find_node_by_path(nini_root_t *root, char *path, char deli)
{
    nini_node_t *parent = &root->super;
    if( !path || !strlen(path) ) return parent;

    char *name;
    while(( name = extract_first_name(&path, deli) ))
    {
        nini_node_t *child = nini_node_find_child(parent, name);
        if( !child ) return NULL;

        parent = child;
    }

    return parent;
}
//------------------------------------------------------------------------------
static
const nini_node_t* find_node_by_path_c(const nini_root_t *root, char *path, char deli)
{
    return find_node_by_path((nini_root_t*)root, path, deli);
}
//------------------------------------------------------------------------------
static
nini_node_t* make_and_link_section_by_name(nini_node_t *parent, const char *name)
{
    nini_node_t *child = nini_node_find_child(parent, name);
    if( child ) return child;

    child = nini_node_create_section(name);
    if( !child ) return NULL;

    if( !nini_node_link_child(parent, child) )
    {
        nini_node_release(child);
        return NULL;
    }

    return child;
}
//------------------------------------------------------------------------------
static
nini_node_t* make_node_by_path(nini_root_t *root, char *path, char deli)
{
    nini_node_t *parent = &root->super;
    if( !path || !strlen(path) ) return parent;

    char *name;
    while(( name = extract_first_name(&path, deli) ))
    {
        nini_node_t *child = make_and_link_section_by_name(parent, name);
        if( !child ) return NULL;

        parent = child;
    }

    return parent;
}
//------------------------------------------------------------------------------
static
bool replace_child(nini_node_t *parent, nini_node_t *new_child)
{
    nini_node_t *old_child = nini_node_find_child(parent, nini_node_get_name(new_child));
    if( old_child )
    {
        nini_node_unlink(old_child);
        nini_node_release(old_child);
    }

    return nini_node_link_child(parent, new_child);
}
//------------------------------------------------------------------------------
bool nini_is_existed(const nini_root_t *root, const char *path, char deli)
{
    /**
     * @brief Check if a key or section existed.
     *
     * @param root The root node of NINI nodes.
     * @param path The path of the key to be operated, see @ref key-path for more details.
     * @param deli The path delimiter.
     * @return TRUE if the node does existed; and FALSE if not.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *node_path = buf;

    return find_node_by_path_c(root, node_path, deli);
}
//------------------------------------------------------------------------------
nini_type_t nini_get_type(const nini_root_t *root, const char *path, char deli)
{
    /**
     * @brief Get the type of a key or section.
     *
     * @param root The root node of NINI nodes.
     * @param path The path of the key to be operated, see @ref key-path for more details.
     * @param deli The path delimiter.
     * @return The type of the key.
     *
     * @remarks
     * The return value is undefined if the node does not existed,
     * we only guarantee that the return will not match
     * any value defined in nini_type_t in this case.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *node_path = buf;

    const nini_node_t *node = find_node_by_path_c(root, node_path, deli);

    return node ? nini_node_get_type(node) : -1;
}
//------------------------------------------------------------------------------
const char* nini_read_string(const nini_root_t *root, const char *path, char deli, const char *failval)
{
    /**
     * @brief Read string value from a key.
     *
     * @param root    The root node of NINI nodes.
     * @param path    The path of the key to be operated, see @ref key-path for more details.
     * @param deli    The path delimiter.
     * @param failval The value that will be used if read failed.
     * @return The value if succeed; or
     *         @a failval if the key does not existed or the key type does not match.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *node_path = buf;

    const nini_node_t *node = find_node_by_path_c(root, node_path, deli);
    if( !node ) return failval;

    return nini_node_get_type(node) == NINI_STRING ? nini_node_get_string(node) : failval;
}
//------------------------------------------------------------------------------
long nini_read_integer(const nini_root_t *root, const char *path, char deli, long failval)
{
    /**
     * @brief Read integer value from a key.
     *
     * @param root    The root node of NINI nodes.
     * @param path    The path of the key to be operated, see @ref key-path for more details.
     * @param deli    The path delimiter.
     * @param failval The value that will be used if read failed.
     * @return The value if succeed; or
     *         @a failval if the key does not existed or the key type does not match.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *node_path = buf;

    const nini_node_t *node = find_node_by_path_c(root, node_path, deli);
    if( !node ) return failval;

    return nini_node_get_type(node) == NINI_DECIMAL || nini_node_get_type(node) == NINI_HEXA ?
           nini_node_get_integer(node) : failval;
}
//------------------------------------------------------------------------------
double nini_read_float(const nini_root_t *root, const char *path, char deli, double failval)
{
    /**
     * @brief Read floating point value from a key.
     *
     * @param root    The root node of NINI nodes.
     * @param path    The path of the key to be operated, see @ref key-path for more details.
     * @param deli    The path delimiter.
     * @param failval The value that will be used if read failed.
     * @return The value if succeed; or
     *         @a failval if the key does not existed or the key type does not match.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *node_path = buf;

    const nini_node_t *node = find_node_by_path_c(root, node_path, deli);
    if( !node ) return failval;

    return nini_node_get_type(node) == NINI_FLOAT ? nini_node_get_float(node) : failval;
}
//------------------------------------------------------------------------------
bool nini_read_bool(const nini_root_t *root, const char *path, char deli, bool failval)
{
    /**
     * @brief Read boolean value from a key.
     *
     * @param root    The root node of NINI nodes.
     * @param path    The path of the key to be operated, see @ref key-path for more details.
     * @param deli    The path delimiter.
     * @param failval The value that will be used if read failed.
     * @return The value if succeed; or
     *         @a failval if the key does not existed or the key type does not match.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *node_path = buf;

    const nini_node_t *node = find_node_by_path_c(root, node_path, deli);
    if( !node ) return failval;

    return nini_node_get_type(node) == NINI_BOOL ? nini_node_get_bool(node) : failval;
}
//------------------------------------------------------------------------------
bool nini_write_string(nini_root_t *root, const char *path, char deli, const char *value)
{
    /**
     * @brief Write string value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrite if it is already existed.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *parent_path = buf;

    char *name = extract_last_name(&parent_path, deli);

    nini_node_t *parent = make_node_by_path(root, parent_path, deli);
    if( !parent ) return false;

    nini_node_t *node = nini_node_create_string(name, value);
    if( !node ) return false;

    return replace_child(parent, node);
}
//------------------------------------------------------------------------------
bool nini_write_decimal(nini_root_t *root, const char *path, char deli, long value)
{
    /**
     * @brief Write decimal value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrite if it is already existed.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *parent_path = buf;

    char *name = extract_last_name(&parent_path, deli);

    nini_node_t *parent = make_node_by_path(root, parent_path, deli);
    if( !parent ) return false;

    nini_node_t *node = nini_node_create_decimal(name, value);
    if( !node ) return false;

    return replace_child(parent, node);
}
//------------------------------------------------------------------------------
bool nini_write_hexa(nini_root_t *root, const char *path, char deli, long value)
{
    /**
     * @brief Write hexadecimal value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrite if it is already existed.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *parent_path = buf;

    char *name = extract_last_name(&parent_path, deli);

    nini_node_t *parent = make_node_by_path(root, parent_path, deli);
    if( !parent ) return false;

    nini_node_t *node = nini_node_create_hexa(name, value);
    if( !node ) return false;

    return replace_child(parent, node);
}
//------------------------------------------------------------------------------
bool nini_write_float(nini_root_t *root, const char *path, char deli, double value)
{
    /**
     * @brief Write floating point value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrite if it is already existed.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *parent_path = buf;

    char *name = extract_last_name(&parent_path, deli);

    nini_node_t *parent = make_node_by_path(root, parent_path, deli);
    if( !parent ) return false;

    nini_node_t *node = nini_node_create_float(name, value);
    if( !node ) return false;

    return replace_child(parent, node);
}
//------------------------------------------------------------------------------
bool nini_write_bool(nini_root_t *root, const char *path, char deli, bool value)
{
    /**
     * @brief Write boolean value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrite if it is already existed.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *parent_path = buf;

    char *name = extract_last_name(&parent_path, deli);

    nini_node_t *parent = make_node_by_path(root, parent_path, deli);
    if( !parent ) return false;

    nini_node_t *node = nini_node_create_bool(name, value);
    if( !node ) return false;

    return replace_child(parent, node);
}
//------------------------------------------------------------------------------
bool nini_write_null(nini_root_t *root, const char *path, char deli)
{
    /**
     * @brief Create a key without value.
     *
     * @param root The root node of NINI nodes.
     * @param path The path of the key to be operated, see @ref key-path for more details.
     * @param deli The path delimiter.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrite if it is already existed.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *parent_path = buf;

    char *name = extract_last_name(&parent_path, deli);

    nini_node_t *parent = make_node_by_path(root, parent_path, deli);
    if( !parent ) return false;

    nini_node_t *node = nini_node_create_null(name);
    if( !node ) return false;

    return replace_child(parent, node);
}
//------------------------------------------------------------------------------
void nini_remove(nini_root_t *root, const char *path, char deli)
{
    /**
     * @brief Remove a node.
     *
     * @param root The root node of NINI nodes.
     * @param path The path of the key to be operated, see @ref key-path for more details.
     * @param deli The path delimiter.
     */
    char buf[strlen(path)+1];
    strncpy(buf, path, sizeof(buf));
    char *node_path = buf;

    nini_node_t *node = find_node_by_path(root, node_path, deli);
    if( !node ) return;

    nini_node_unlink(node);
    nini_node_release(node);
}
//------------------------------------------------------------------------------
