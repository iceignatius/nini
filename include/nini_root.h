/**
 * @file
 * @brief     Nested INI root node.
 * @author    王文佑
 * @date      2017/07/03
 * @copyright ZLib Licence
 */
#ifndef _NINI_ROOT_H_
#define _NINI_ROOT_H_

#include "nini_errmsg.h"
#include "nini_node.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   User defined stream writer.
 * @details User defined function that will be called to write data into the user defined stream.
 *
 * @param stream The user defined stream object.
 * @param line   The data to be write to the stream,
 *               and that will be one line of the NINI format characters.
 * @param len    Size of the data.
 * @return TRUE if success; and FALSE if not.
 */
typedef bool(*nini_on_write_t)(void *stream, const char *line, size_t len);

/**
 * @class nini_root_t
 * @brief Root node.
 */
typedef struct nini_root_t
{
    // WARNING: All variables are private!

    nini_node_t super;

    nini_format_t format;

} nini_root_t;

void nini_root_init  (nini_root_t *self, const nini_format_t *format);
void nini_root_deinit(nini_root_t *self);

void nini_root_clear(nini_root_t *self);

static inline
nini_node_t* nini_root_get_first_child(nini_root_t *self)
{
    /**
     * @memberof nini_root_t
     * @brief Get the first child.
     *
     * @param self Object instance.
     * @return The first child if it has; or
     *         NULL if there are no children.
     */
    return nini_node_get_first_child(&self->super);
}

static inline
nini_node_t* nini_root_get_last_child(nini_root_t *self)
{
    /**
     * @memberof nini_root_t
     * @brief Get the last child.
     *
     * @param self Object instance.
     * @return The last child if it has; or
     *         NULL if there are no children.
     */
    return nini_node_get_last_child(&self->super);
}

static inline
nini_node_t* nini_root_find_child(nini_root_t *self, const char *name)
{
    /**
     * @memberof nini_root_t
     * @brief Find child by name.
     *
     * @param self Object instance.
     * @param name Name of the child.
     * @return A child that match the name; or
               NULL if not found.
     */
    return nini_node_find_child(&self->super, name);
}

static inline
const nini_node_t* nini_root_get_first_child_c(const nini_root_t *self)
{
    /**
     * @memberof nini_root_t
     * @brief Get the first child.
     *
     * @param self Object instance.
     * @return The first child if it has; or
     *         NULL if there are no children.
     */
    return nini_node_get_first_child_c(&self->super);
}

static inline
const nini_node_t* nini_root_get_last_child_c(const nini_root_t *self)
{
    /**
     * @memberof nini_root_t
     * @brief Get the last child.
     *
     * @param self Object instance.
     * @return The last child if it has; or
     *         NULL if there are no children.
     */
    return nini_node_get_last_child_c(&self->super);
}

static inline
const nini_node_t* nini_root_find_child_c(const nini_root_t *self, const char *name)
{
    /**
     * @memberof nini_root_t
     * @brief Find child by name.
     *
     * @param self Object instance.
     * @param name Name of the child.
     * @return A child that match the name; or
               NULL if not found.
     */
    return nini_node_find_child_c(&self->super, name);
}

static inline
bool nini_root_link_child(nini_root_t *self, nini_node_t *node)
{
    /**
     * @memberof nini_root_t
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
    return nini_node_link_child(&self->super, node);
}

bool nini_root_decode(nini_root_t *self, const void *data, size_t size, nini_errmsg_t *errmsg);

size_t nini_root_encode_to_stream(const nini_root_t *self,
                                  void              *stream,
                                  nini_on_write_t    on_write,
                                  nini_errmsg_t     *errmsg);

size_t nini_root_encode_to_buffer(const nini_root_t *self,
                                  void              *buf,
                                  size_t             size,
                                  nini_errmsg_t     *errmsg);

bool nini_root_load_file(nini_root_t *self, const char *filename, nini_errmsg_t *errmsg);
bool nini_root_save_file(const nini_root_t *self, const char *filename, nini_errmsg_t *errmsg);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
