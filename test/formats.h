#ifndef _FORMATS_H_
#define _FORMATS_H_

#include "nini_format.h"

static const nini_format_t format_no_indents =
{
    .sec_head   = '[',
    .sec_tail   = ']',
    .keymark    = '=',
    .comment    = ';',
    .indent     = 0,
};

static const nini_format_t format_have_indents =
{
    .sec_head   = '[',
    .sec_tail   = ']',
    .keymark    = '=',
    .comment    = ';',
    .indent     = 4,
};

#endif
