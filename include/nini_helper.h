/**
 * @file
 * @brief     Nested INI helper.
 * @details   This module is the helper module to let user operate NINI objects easily.
 * @author    王文佑
 * @date      2017/07/03
 * @copyright ZLib Licence
 */
#ifndef _NINI_HELPER_H_
#define _NINI_HELPER_H_

#include "nini_root.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 * @section nouns-def Nouns Definition
 *
 * @subsection key-path Key Path
 *
 * A string that be combined by names of all nodes from the root to the key
 * with the specified delimiter.
 *
 * For example,
 * if there have a key (named "somekey")
 * which is placed under a section (named "sec-a"),
 * and the section is placed under another section (named "sec-b"),
 * and we define that the delimiter is '/'.
 * Then, the path will be:
 * "sec-b/sec-a/somekey".
 *
 * Obviously, all sections and keys shall not have the delimiter character,
 * or the path operation will be strange. And user will be responsible for that.
 */

bool nini_write_string (nini_root_t *root, const char *path, char deli, const char *value);
bool nini_write_decimal(nini_root_t *root, const char *path, char deli, long value);
bool nini_write_hexa   (nini_root_t *root, const char *path, char deli, long value);
bool nini_write_float  (nini_root_t *root, const char *path, char deli, double value);
bool nini_write_bool   (nini_root_t *root, const char *path, char deli, bool value);
bool nini_write_null   (nini_root_t *root, const char *path, char deli);

const char* nini_read_string (const nini_root_t *root, const char *path, char deli, const char *failval);
long        nini_read_integer(const nini_root_t *root, const char *path, char deli, long failval);
double      nini_read_float  (const nini_root_t *root, const char *path, char deli, double failval);
bool        nini_read_bool   (const nini_root_t *root, const char *path, char deli, bool failval);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
