#include "nini_root.h"

//------------------------------------------------------------------------------
void nini_root_init(nini_root_t *self, const nini_format_t *format)
{
    /**
     * @memberof nini_root_t
     * @brief Constructor.
     *
     * @param self   Object instance.
     * @param format Specify the format to be used to parse and encode data.
     */
}
//------------------------------------------------------------------------------
void nini_root_deinit(nini_root_t *self)
{
    /**
     * @memberof nini_root_t
     * @brief Destructor.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
void nini_root_clear(nini_root_t *self)
{
    /**
     * @memberof nini_root_t
     * @brief Remove all children it contained.
     *
     * @param self Object instance.
     */
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
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
}
//------------------------------------------------------------------------------
bool nini_root_decode(nini_root_t *self, const void *data, size_t size, nini_errmsg_t *errmsg)
{
    /**
     * @memberof nini_root_t
     * @brief Decode information from the NINI format data.
     *
     * @param self   Object instance.
     * @param data   The NINI format data to be parsed.
     * @param size   Size of the input data.
     * @param errmsg The object that will be filled with failure information if decode failed,
     *               and it will be cleared otherwise.
     *               This parameter can be NULL to discard the error report.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks This function is not atomic,
     *          and all values it contained will be removed if decode failed.
     */
}
//------------------------------------------------------------------------------
size_t nini_root_encode_to_stream(nini_root_t     *self,
                                  void            *stream,
                                  nini_on_write_t  on_write,
                                  nini_errmsg_t   *errmsg)
{
    /**
     * @memberof nini_root_t
     * @brief Encode information to the NINI format data to a stream.
     *
     * @param self     Object instance.
     * @param stream   The user defined stream object.
     * @param on_write The user defined stream writer.
     * @param errmsg   The object that will be filled with failure information if decode failed,
     *                 and it will be cleared otherwise.
     *                 This parameter can be NULL to discard the error report.
     * @return Size of data be filled to the stream if succeed; and ZERO if failed.
     */
}
//------------------------------------------------------------------------------
size_t nini_root_encode_to_buffer(nini_root_t   *self,
                                  void          *buf,
                                  size_t         size,
                                  nini_errmsg_t *errmsg)
{
    /**
     * @memberof nini_root_t
     * @brief Encode information to the NINI format data to a buffer.
     *
     * @param self   Object instance.
     * @param buf    A buffer to be filled with the result data.
     * @param size   Size of the output buffer.
     * @param errmsg The object that will be filled with failure information if decode failed,
     *               and it will be cleared otherwise.
     *               This parameter can be NULL to discard the error report.
     * @return Size of data be filled to the stream if succeed; and ZERO if failed.
     */
}
//------------------------------------------------------------------------------
bool nini_root_load_file(nini_root_t *self, const char *filename, nini_errmsg_t *errmsg)
{
    /**
     * @memberof nini_root_t
     * @brief Load information from NINI format file.
     *
     * @param self     Object instance.
     * @param filename Name of the input file.
     * @param errmsg   The object that will be filled with failure information if decode failed,
     *                 and it will be cleared otherwise.
     *                 This parameter can be NULL to discard the error report.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks This function is not atomic,
     *          and all values it contained will be removed if decode failed.
     */
}
//------------------------------------------------------------------------------
bool nini_root_save_file(nini_root_t *self, const char *filename, nini_errmsg_t *errmsg)
{
    /**
     * @memberof nini_root_t
     * @brief Save information to NINI format file.
     *
     * @param self     Object instance.
     * @param filename Name of the output file.
     * @param errmsg   The object that will be filled with failure information if decode failed,
     *                 and it will be cleared otherwise.
     *                 This parameter can be NULL to discard the error report.
     * @return TRUE if succeed; and FALSE if not.
     */
}
//------------------------------------------------------------------------------
