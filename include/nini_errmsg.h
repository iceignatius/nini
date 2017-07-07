/**
 * @file
 * @brief     Nested INI encode/decode error message.
 * @author    王文佑
 * @date      2017/07/03
 * @copyright ZLib Licence
 */
#ifndef _NINI_ERRMSG_H_
#define _NINI_ERRMSG_H_

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

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
