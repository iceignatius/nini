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
 */
typedef void(*nini_on_write_t)(void *stream, const char *line, size_t len);

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

nini_node_t* nini_root_get_first_child(nini_root_t *self);
nini_node_t* nini_root_get_last_child (nini_root_t *self);
nini_node_t* nini_root_find_child     (nini_root_t *self, const char *name);

const nini_node_t* nini_root_get_first_child_c(const nini_root_t *self);
const nini_node_t* nini_root_get_last_child_c (const nini_root_t *self);
const nini_node_t* nini_root_find_child_c     (const nini_root_t *self, const char *name);

bool nini_root_link_child(nini_root_t *self, nini_node_t *node);

bool nini_root_decode(nini_root_t *self, const void *data, size_t size, nini_errmsg_t *errmsg);

size_t nini_root_encode_to_stream(nini_root_t     *self,
                                  void            *stream,
                                  nini_on_write_t  on_write,
                                  nini_errmsg_t   *errmsg);

size_t nini_root_encode_to_buffer(nini_root_t   *self,
                                  void          *buf,
                                  size_t         size,
                                  nini_errmsg_t *errmsg);

bool nini_root_load_file(nini_root_t *self, const char *filename, nini_errmsg_t *errmsg);
bool nini_root_save_file(nini_root_t *self, const char *filename, nini_errmsg_t *errmsg);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
