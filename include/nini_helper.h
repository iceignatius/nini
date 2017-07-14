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

bool        nini_is_existed(const nini_root_t *root, const char *path, char deli);
nini_type_t nini_get_type  (const nini_root_t *root, const char *path, char deli);

const char* nini_read_string (const nini_root_t *root, const char *path, char deli, const char *failval);
long        nini_read_integer(const nini_root_t *root, const char *path, char deli, long failval);
double      nini_read_float  (const nini_root_t *root, const char *path, char deli, double failval);
bool        nini_read_bool   (const nini_root_t *root, const char *path, char deli, bool failval);

bool nini_write_string (nini_root_t *root, const char *path, char deli, const char *value);
bool nini_write_decimal(nini_root_t *root, const char *path, char deli, long value);
bool nini_write_hexa   (nini_root_t *root, const char *path, char deli, long value);
bool nini_write_float  (nini_root_t *root, const char *path, char deli, double value);
bool nini_write_bool   (nini_root_t *root, const char *path, char deli, bool value);
bool nini_write_null   (nini_root_t *root, const char *path, char deli);

void nini_remove(nini_root_t *root, const char *path, char deli);

#ifdef __cplusplus
}  // extern "C"
#endif

#ifdef __cplusplus

namespace nini
{

/// NINI helper.
class TNini : public TRoot
{
protected:
    char deli;

public:
    /// Constructor.
    TNini(const TFormat *format, char delimiter) : TRoot(format), deli(delimiter) {}

public:
    /// The same as nini_is_existed.
    bool IsExisted(const std::string &path) const
    { return nini_is_existed(this, path.c_str(), this->deli); }

    /// The same as nini_get_type.
    TType GetType(const std::string &path) const
    { return nini_get_type(this, path.c_str(), this->deli); }

    /// The same as nini_read_string.
    std::string ReadString(const std::string &path, const std::string &failval="") const
    { return nini_read_string(this, path.c_str(), this->deli, failval.c_str()); }

    /// The same as nini_read_integer.
    long ReadInteger(const std::string &path, long failval=0) const
    { return nini_read_integer(this, path.c_str(), this->deli, failval); }

    /// The same as nini_read_float.
    double ReadFloat(const std::string &path, double failval=0) const
    { return nini_read_float(this, path.c_str(), this->deli, failval); }

    /// The same as nini_read_bool.
    bool ReadBool(const std::string &path, bool failval=false) const
    { return nini_read_bool(this, path.c_str(), this->deli, failval); }

    /// The same as nini_write_string.
    bool WriteString(const std::string &path, const std::string &value)
    { return nini_write_string(this, path.c_str(), this->deli, value.c_str()); }

    /// The same as nini_write_decimal.
    bool WriteDecimal(const std::string &path, long value)
    { return nini_write_decimal(this, path.c_str(), this->deli, value); }

    /// The same as nini_write_hexa.
    bool WriteHexa(const std::string &path, long value)
    { return nini_write_hexa(this, path.c_str(), this->deli, value); }

    /// The same as nini_write_float.
    bool WriteFloat(const std::string &path, double value)
    { return nini_write_float(this, path.c_str(), this->deli, value); }

    /// The same as nini_write_bool.
    bool WriteBool(const std::string &path, bool value)
    { return nini_write_bool(this, path.c_str(), this->deli, value); }

    /// The same as nini_write_null.
    bool WriteNull(const std::string &path)
    { return nini_write_null(this, path.c_str(), this->deli); }

    /// The same as nini_remove.
    void Remove(const std::string &path) { nini_remove(this, path.c_str(), this->deli); }

};

}

#endif

#endif
