/**
 * @file
 * @brief     Nested INI node type.
 * @author    王文佑
 * @date      2017/07/11
 * @copyright ZLib Licence
 */
#ifndef _NINI_TYPE_H_
#define _NINI_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Node types.
 */
typedef enum nini_type_t
{
    NINI_ROOT,      ///< Node is the root section (virtual).
    NINI_SECTION,   ///< Node is a section.
    NINI_STRING,    ///< Node is a key with string value.
    NINI_DECIMAL,   ///< Node is a key with integer value in decimal format.
    NINI_HEXA,      ///< Node is a key with integer value in hexadecimal format.
    NINI_FLOAT,     ///< Node is a key with floating point value.
    NINI_BOOL,      ///< Node is a key with boolean value.
    NINI_NULL,      ///< Node is a key without value.
} nini_type_t;

#ifdef __cplusplus
}  // extern "C"
#endif

#ifdef __cplusplus

namespace nini
{
    /// C++ wrapper of nini_type_t.
    typedef nini_type_t TType;
}

#endif

#endif
