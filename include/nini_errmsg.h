/**
 * @file
 * @brief     Nested INI encode/decode error message.
 * @author    王文佑
 * @date      2017/07/03
 * @copyright ZLib Licence
 */
#ifndef _NINI_ERRMSG_H_
#define _NINI_ERRMSG_H_

#include <stddef.h>
#include "nini_format.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Encoder/decoder error information.
 */
typedef struct nini_errmsg_t
{
    int  line_num;                          ///< Index (start from 1) of the incorrect line.
    char line_text[NINI_MAX_LINE_CHARS+1];  ///< Text of the incorrect line.
    char message  [NINI_MAX_LINE_CHARS+1];  ///< The reason to cause the error in human readable text.
} nini_errmsg_t;

#ifdef __GNUC__
    #define NINI_ERRMSG_PRINT_ATTR __attribute__ ((format (printf, 4, 5)))
#else
    #define NINI_ERRMSG_PRINT_ATTR
#endif

void nini_errmsg_write(nini_errmsg_t *errmsg,
                       int            index,
                       const char    *line,
                       const char    *format,
                       ...) NINI_ERRMSG_PRINT_ATTR;

#ifdef __cplusplus
}  // extern "C"
#endif

#ifdef __cplusplus

namespace nini
{
    /// C++ wrapper of nini_errmsg_t.
    typedef nini_errmsg_t TErrMsg;
}

#endif

#endif
