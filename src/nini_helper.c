#include "nini_helper.h"

//------------------------------------------------------------------------------
bool nini_write_string(nini_root_t *root, const char *path, char deli, const char *value)
{
    /**
     * @brief Write string value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrited if it is already existed.
     */
}
//------------------------------------------------------------------------------
bool nini_write_decimal(nini_root_t *root, const char *path, char deli, long value)
{
    /**
     * @brief Write decimal value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrited if it is already existed.
     */
}
//------------------------------------------------------------------------------
bool nini_write_hexa(nini_root_t *root, const char *path, char deli, long value)
{
    /**
     * @brief Write hexadecimal value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrited if it is already existed.
     */
}
//------------------------------------------------------------------------------
bool nini_write_float(nini_root_t *root, const char *path, char deli, double value)
{
    /**
     * @brief Write floating point value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrited if it is already existed.
     */
}
//------------------------------------------------------------------------------
bool nini_write_bool(nini_root_t *root, const char *path, char deli, bool value)
{
    /**
     * @brief Write boolean value to a key.
     *
     * @param root  The root node of NINI nodes.
     * @param path  The path of the key to be operated, see @ref key-path for more details.
     * @param deli  The path delimiter.
     * @param value The value to be write to.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrited if it is already existed.
     */
}
//------------------------------------------------------------------------------
bool nini_write_null(nini_root_t *root, const char *path, char deli)
{
    /**
     * @brief Create a key without value.
     *
     * @param root The root node of NINI nodes.
     * @param path The path of the key to be operated, see @ref key-path for more details.
     * @param deli The path delimiter.
     * @return TRUE if succeed; and FALSE if not.
     *
     * @remarks
     * * The path will be created if it does not existed.
     * * The key will be overwrited if it is already existed.
     */
}
//------------------------------------------------------------------------------
const char* nini_read_string(const nini_root_t *root, const char *path, char deli, const char *failval)
{
    /**
     * @brief Read string value from a key.
     *
     * @param root    The root node of NINI nodes.
     * @param path    The path of the key to be operated, see @ref key-path for more details.
     * @param deli    The path delimiter.
     * @param failval The value that will be used if read failed.
     * @return The value if succeed; or
     *         @a failval if the key does not existed or the key type does not match.
     */
}
//------------------------------------------------------------------------------
long nini_read_integer(const nini_root_t *root, const char *path, char deli, long failval)
{
    /**
     * @brief Read integer value from a key.
     *
     * @param root    The root node of NINI nodes.
     * @param path    The path of the key to be operated, see @ref key-path for more details.
     * @param deli    The path delimiter.
     * @param failval The value that will be used if read failed.
     * @return The value if succeed; or
     *         @a failval if the key does not existed or the key type does not match.
     */
}
//------------------------------------------------------------------------------
double nini_read_float(const nini_root_t *root, const char *path, char deli, double failval)
{
    /**
     * @brief Read floating point value from a key.
     *
     * @param root    The root node of NINI nodes.
     * @param path    The path of the key to be operated, see @ref key-path for more details.
     * @param deli    The path delimiter.
     * @param failval The value that will be used if read failed.
     * @return The value if succeed; or
     *         @a failval if the key does not existed or the key type does not match.
     */
}
//------------------------------------------------------------------------------
bool nini_read_bool(const nini_root_t *root, const char *path, char deli, bool failval)
{
    /**
     * @brief Read boolean value from a key.
     *
     * @param root    The root node of NINI nodes.
     * @param path    The path of the key to be operated, see @ref key-path for more details.
     * @param deli    The path delimiter.
     * @param failval The value that will be used if read failed.
     * @return The value if succeed; or
     *         @a failval if the key does not existed or the key type does not match.
     */
}
//------------------------------------------------------------------------------
