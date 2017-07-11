#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "nini_errmsg.h"

//------------------------------------------------------------------------------
void nini_errmsg_write(nini_errmsg_t *errmsg,
                       int            index,
                       const char    *line,
                       const char    *format,
                       ...)
{
    if( !errmsg ) return;

    errmsg->line_num = index;

    strncpy(errmsg->line_text, line, sizeof(errmsg->line_text)-1);

    va_list arg;
    va_start(arg, format);

    vsnprintf(errmsg->message, sizeof(errmsg->message), format, arg);

    va_end(arg);
}
//------------------------------------------------------------------------------
