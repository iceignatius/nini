#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nini_parser.h"
#include "nini_root.h"

typedef struct buffer_stream_t
{
    uint8_t *buf;
    size_t   rest_size;
    size_t   write_size;
} buffer_stream_t;

//------------------------------------------------------------------------------
static
void buffer_stream_init(buffer_stream_t *stream, void *buf, size_t size)
{
    stream->buf        = buf;
    stream->rest_size  = size;
    stream->write_size = 0;
}
//------------------------------------------------------------------------------
static
size_t buffer_stream_get_write_size(const buffer_stream_t *stream)
{
    return stream->write_size;
}
//------------------------------------------------------------------------------
static
bool buffer_stream_push_data(buffer_stream_t *stream, const void *data, size_t size)
{
    if( stream->rest_size < size ) return false;

    memcpy(stream->buf, data, size);
    stream->buf        += size;
    stream->rest_size  -= size;
    stream->write_size += size;

    return true;
}
//------------------------------------------------------------------------------
static
bool buffer_stream_push_str(buffer_stream_t *stream, const char *str)
{
    return buffer_stream_push_data(stream, str, strlen(str));
}
//------------------------------------------------------------------------------
static
bool buffer_stream_push_ch(buffer_stream_t *stream, char ch)
{
    return buffer_stream_push_data(stream, &ch, 1);
}
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
    memset(self, 0, sizeof(*self));
    self->super.type = NINI_ROOT;

    assert( format );
    self->format = *format;
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
    nini_root_clear(self);
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
    nini_node_t *node = nini_node_get_first_child(&self->super);
    while( node )
    {
        nini_node_t *node_remove = node;
        node = nini_node_get_next_sibling(node);

        nini_node_release(node_remove);
    }

    self->super.childs.first = NULL;
    self->super.childs.last  = NULL;
}
//------------------------------------------------------------------------------
static
nini_node_t* decode_on_item(nini_root_t         *self,
                            nini_node_t         *parent,
                            int                  level,
                            nini_type_t          type,
                            const char          *name,
                            nini_parser_value_t *value)
{
    nini_node_t *node;
    switch( type )
    {
    case NINI_SECTION:
        node = nini_node_create_section(name);
        break;

    case NINI_STRING:
        node = nini_node_create_string(name, value->string);
        break;

    case NINI_DECIMAL:
        node = nini_node_create_decimal(name, value->integer);
        break;

    case NINI_HEXA:
        node = nini_node_create_hexa(name, value->integer);
        break;

    case NINI_FLOAT:
        node = nini_node_create_float(name, value->floating);
        break;

    case NINI_BOOL:
        node = nini_node_create_bool(name, value->boolean);
        break;

    case NINI_NULL:
        node = nini_node_create_null(name);
        break;

    default:
        node = NULL;
        break;
    }

    if( node )
    {
        parent = parent ? parent : &self->super;
        if( !nini_node_link_child(parent, node) )
        {
            nini_node_release(node);
            node = NULL;
        }
    }

    return node;
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
    nini_root_clear(self);

    if( !data || !size ) return true;

    nini_parser_t parser;
    nini_parser_init(&parser,
                     &self->format,
                     self,
                     (void*(*)(void*,void*,int,nini_type_t,const char*,nini_parser_value_t*)) decode_on_item);

    bool res = nini_parser_parse(&parser, data, size);
    if( errmsg ) nini_parser_get_errmsg(&parser, errmsg);

    if( !res ) nini_root_clear(self);

    return res;
}
//------------------------------------------------------------------------------
static
void encode_char_with_escapes(char *dest, size_t bufsize, char ch)
{
    const char *predefined;
    switch( ch )
    {
    case 0x07:
        predefined = "\\a";
        break;

    case 0x08:
        predefined = "\\b";
        break;

    case 0x0C:
        predefined = "\\f";
        break;

    case 0x0A:
        predefined = "\\n";
        break;

    case 0x0D:
        predefined = "\\r";
        break;

    case 0x09:
        predefined = "\\t";
        break;

    case 0x0B:
        predefined = "\\v";
        break;

    case '\'':
        predefined = "\\\'";
        break;

    case '\"':
        predefined = "\\\"";
        break;

    case '\\':
        predefined = "\\\\";
        break;

    default:
        predefined = NULL;
        break;
    }

    if( predefined )
    {
        strncpy(dest, predefined, bufsize);
    }
    else if( ch < 0x20 || 0x7F <= ch )
    {
        snprintf(dest, bufsize, "\\x%02X", (unsigned)(uint8_t)ch);
    }
    else
    {
        dest[0] = ch;
        dest[1] = 0;
    }
}
//------------------------------------------------------------------------------
static
int encode_quoted_string(char *buf, size_t bufsize, const char *src)
{
    buffer_stream_t stream;
    buffer_stream_init(&stream, buf, bufsize);

    if( !buffer_stream_push_ch(&stream, ' ') ) return -1;
    if( !buffer_stream_push_ch(&stream, '\"') ) return -1;

    for(; *src; ++src)
    {
        char encstr[4+1];
        encode_char_with_escapes(encstr, sizeof(encstr), *src);

        if( !buffer_stream_push_str(&stream, encstr) )
            return -1;
    }

    if( !buffer_stream_push_ch(&stream, '\"') ) return -1;
    if( !buffer_stream_push_ch(&stream, 0) ) return -1;

    return buffer_stream_get_write_size(&stream);
}
//------------------------------------------------------------------------------
static
bool encode_value(char *buf, size_t bufsize, const nini_node_t *node)
{
    int fillsize;
    switch( nini_node_get_type(node) )
    {
    case NINI_STRING:
        fillsize = encode_quoted_string(buf, bufsize, nini_node_get_string(node));
        break;

    case NINI_DECIMAL:
        fillsize = snprintf(buf, bufsize, " %ld", nini_node_get_integer(node));
        break;

    case NINI_HEXA:
        fillsize = snprintf(buf, bufsize, " %lX", nini_node_get_integer(node));
        break;

    case NINI_FLOAT:
        fillsize = snprintf(buf, bufsize, " %lf", nini_node_get_float(node));
        break;

    case NINI_BOOL:
        fillsize = snprintf(buf, bufsize, " %s", nini_node_get_bool(node) ? "true" : "false");
        break;

    default:
        buf[0] = 0;
        fillsize = 1;
        break;
    }

    return 0 < fillsize && fillsize <= bufsize;
}
//------------------------------------------------------------------------------
static
size_t encode_node(const nini_format_t *format,
                   void                *stream,
                   nini_on_write_t      on_write,
                   int                  level,
                   const nini_node_t   *node)
{
    // Generate indents.
    char indents[NINI_MAX_LINE_CHARS+1] = {0};
    for(int i = 0; i < format->indent * level; ++i)
        indents[i] = ' ';

    // Generate name.
    char name[NINI_MAX_LINE_CHARS+1];
    if( nini_node_get_type(node) == NINI_SECTION )
    {
        if( !format->sec_head ) return 0;

        char stx[2] = { format->sec_head };
        char etx[2] = { format->sec_tail };
        snprintf(name, sizeof(name), "%s%s%s", stx, nini_node_get_name(node), etx);
    }
    else
    {
        assert( format->keymark );
        snprintf(name, sizeof(name), "%s %c", nini_node_get_name(node), format->keymark);
    }

    // Generate value.
    char value[NINI_MAX_LINE_CHARS+1];
    if( !encode_value(value, sizeof(value), node) ) return 0;

    // Generate line.

    static const char ASCII_LF = 0x0A;

    char line[NINI_MAX_LINE_CHARS+1];
    int fillsize = snprintf(line, sizeof(line), "%s%s%s%c", indents, name, value, ASCII_LF);
    if( fillsize < 0 || sizeof(line) < fillsize ) return 0;

    // Write stream.
    if( !on_write(stream, line, fillsize) ) return 0;

    // Process children.
    size_t total_size = fillsize;
    for(const nini_node_t *child = nini_node_get_first_child_c(node);
        child;
        child = nini_node_get_next_sibling_c(child))
    {
        size_t fillsize = encode_node(format, stream, on_write, level+1, child);
        if( !fillsize ) return 0;

        total_size += fillsize;
    }

    return total_size;
}
//------------------------------------------------------------------------------
size_t nini_root_encode_to_stream(const nini_root_t *self,
                                  void              *stream,
                                  nini_on_write_t    on_write,
                                  nini_errmsg_t     *errmsg)
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
    size_t total_size = 0;

    for(const nini_node_t *node = nini_root_get_first_child_c(self);
        node;
        node = nini_node_get_next_sibling_c(node))
    {
        size_t fillsize = encode_node(&self->format, stream, on_write, 0, node);
        if( !fillsize ) return 0;

        total_size += fillsize;
    }

    return total_size;
}
//------------------------------------------------------------------------------
size_t nini_root_encode_to_buffer(const nini_root_t *self,
                                  void              *buf,
                                  size_t             size,
                                  nini_errmsg_t     *errmsg)
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
    buffer_stream_t stream;
    buffer_stream_init(&stream, buf, size);

    return nini_root_encode_to_stream(self,
                                      &stream,
                                      (bool(*)(void*,const char*,size_t)) buffer_stream_push_data,
                                      errmsg);
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
    FILE    *file = NULL;
    uint8_t *data = NULL;
    long     size;

    nini_root_clear(self);

    bool res;
    do
    {
        if( !filename ) break;
        file = fopen(filename, "rb");
        if( !file ) break;

        if( fseek(file, 0, SEEK_END) ) break;
        if( 0 > ( size = ftell(file) ) ) break;
        rewind(file);

        data = malloc(size);
        if( !data ) break;

        if( size != fread(data, 1, size, file) ) break;

        if( !nini_root_decode(self, data, size, errmsg) ) break;

        res = true;
    } while(false);

    if( data ) free(data);
    if( file ) fclose(file);

    return res;
}
//------------------------------------------------------------------------------
static
bool file_on_write(FILE *stream, const char *line, size_t len)
{
    return len == fwrite(line, 1, len, stream);
}
//------------------------------------------------------------------------------
bool nini_root_save_file(const nini_root_t *self, const char *filename, nini_errmsg_t *errmsg)
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
    FILE *file = fopen(filename, "wb");
    if( !file ) return false;

    size_t recsize = nini_root_encode_to_stream(self,
                                                file,
                                                (bool(*)(void*,const char*,size_t)) file_on_write,
                                                errmsg);

    fclose(file);

    return recsize;
}
//------------------------------------------------------------------------------
