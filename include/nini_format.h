/**
 * @file
 * @brief     Nested INI format definition.
 * @author    王文佑
 * @date      2017/07/03
 * @copyright ZLib Licence
 */
#ifndef _NINI_FORMAT_H_
#define _NINI_FORMAT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Maximum line length.
 * @details The maximum characters of a line to be parsed.
 *          It can be configured to fit user need at compile time.
 */
#define NINI_MAX_LINE_CHARS 1024

/**
 * @brief   Maximum nested level.
 * @details The maximum indent level the parser can parse for nested items.
 *          It can be configured to fit user need at compile time.
 */
#define NINI_MAX_PARSE_LEVEL 1024

/**
 * @brief   INI format definition.
 * @details Dedine what characters will be used for each marks and symbols,
 *          so that it can be used for most of INI derived formats.
 *
 * @remarks Characters in a format profile must not be duplicated (excluding zero),
 *          or it will lead to undefined behaviour!
 */
typedef struct nini_format_t
{
    char     sec_head;  ///< The character to mark the start of a section name,
                        ///< and can be ZERO to not support section.
    char     sec_tail;  ///< The character to mark the end of a section name,
                        ///< and can be ZERO if the section does not have the end mark.
    char     keymark;   ///< The character to separate the key and value,
                        ///< and this parameter MUST be presented.
    char     comment;   ///< The character to mark the start of a comment,
                        ///< and can be ZERO to not support comments.
    unsigned indent;    ///< It defines how many spaces will be used in one indent level,
                        ///< and can be ZERO to disable nested section support.
                        ///< It is recommended to set it to 4 for general purpose.
} nini_format_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
