#ifndef _NINI_PARSER_H_
#define _NINI_PARSER_H_

#include <stdbool.h>
#include "nini_format.h"
#include "nini_errmsg.h"
#include "nini_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef union nini_parser_value_t
{
    const char *string;
    long        integer;
    double      floating;
    bool        boolean;
} nini_parser_value_t;

typedef void*(*nini_parser_on_item_t)(void                *userarg,
                                      void                *parent,
                                      int                  level,
                                      nini_type_t          type,
                                      const char          *name,
                                      nini_parser_value_t *value);

typedef struct nini_parser_text_t
{
    char     str[NINI_MAX_LINE_CHARS+1];
    unsigned len;
} nini_parser_text_t;

typedef struct nini_parser_stack_t
{
    void     *items[NINI_MAX_PARSE_LEVEL];
    unsigned  count;
} nini_parser_stack_t;

typedef struct nini_parser_t
{
    nini_format_t format;
    nini_errmsg_t errmsg;

    void                  *userarg;
    nini_parser_on_item_t  on_item;

    int line_index;

    int state;

    int                indents;
    int                last_level;
    nini_parser_text_t line;
    nini_parser_text_t section;
    nini_parser_text_t key;
    nini_parser_text_t value;

    char escape_x_chars[2+1];

    nini_parser_stack_t stack;

} nini_parser_t;

void nini_parser_init(nini_parser_t         *self,
                      const nini_format_t   *format,
                      void                  *userarg,
                      nini_parser_on_item_t  on_item);

bool nini_parser_parse(nini_parser_t *self, const void *data, size_t size);
void nini_parser_get_errmsg(const nini_parser_t *self, nini_errmsg_t *errmsg);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
