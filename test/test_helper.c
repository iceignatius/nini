#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include "nini_helper.h"
#include "formats.h"
#include "test_helper.h"

//------------------------------------------------------------------------------
static
void helper_read_test(void **state)
{
    nini_root_t root;
    nini_root_init(&root, &format_no_indents);

    assert_true( nini_root_load_file(&root, "samples/value-types.ini", NULL) );

    assert_string_equal( nini_read_string(&root, "string/spaces", '/', "fail-string"), "string with spaces" );
    assert_string_equal( nini_read_string(&root, "string-fail/spaces", '/', "fail-string"), "fail-string" );

    assert_int_equal( nini_read_integer(&root, "integer/decimal", '/', -1), 13579 );
    assert_int_equal( nini_read_integer(&root, "integer-fail/decimal", '/', -1), -1 );
    assert_int_equal( nini_read_integer(&root, "integer/hexadecimal", '/', -1), 0x1A7B );
    assert_int_equal( nini_read_integer(&root, "integer-fail/hexadecimal", '/', -1), -1 );

    assert_true( nini_read_float(&root, "floating/float", '/', -1) - 3.14159265 < 0.001 );
    assert_true( nini_read_float(&root, "floating-fail/float", '/', -1) - (-1) < 0.001 );

    assert_true( nini_read_bool(&root, "boolean/true", '/', false) );
    assert_false( nini_read_bool(&root, "boolean-fail/true", '/', false) );
    assert_false( nini_read_bool(&root, "boolean/false", '/', true) );
    assert_true( nini_read_bool(&root, "boolean-fail/false", '/', true) );

    nini_root_deinit(&root);
}
//------------------------------------------------------------------------------
static
void helper_write_test(void **state)
{
    nini_root_t root;
    nini_root_init(&root, &format_have_indents);

    assert_true( nini_write_null(&root, "path/to/string", '/') );
    assert_true( nini_write_string(&root, "path/to/string", '/', "string with spaces") );
    assert_string_equal( nini_read_string(&root, "path/to/string", '/', "fail-string"), "string with spaces" );

    assert_true( nini_write_null(&root, "path/to/decimal", '/') );
    assert_true( nini_write_decimal(&root, "path/to/decimal", '/', 13579) );
    assert_int_equal( nini_read_integer(&root, "path/to/decimal", '/', -1), 13579 );

    assert_true( nini_write_null(&root, "path/to/hexadecimal", '/') );
    assert_true( nini_write_hexa(&root, "path/to/hexadecimal", '/', 0x1A7B) );
    assert_int_equal( nini_read_integer(&root, "path/to/hexadecimal", '/', -1), 0x1A7B );

    assert_true( nini_write_null(&root, "path/to/float", '/') );
    assert_true( nini_write_float(&root, "path/to/float", '/', 3.14159265) );
    assert_true( nini_read_float(&root, "path/to/float", '/', -1) - 3.14159265 < 0.001 );

    assert_true( nini_write_null(&root, "path/to/true", '/') );
    assert_true( nini_write_bool(&root, "path/to/true", '/', true) );
    assert_true( nini_read_bool(&root, "path/to/true", '/', false) );

    assert_true( nini_write_null(&root, "path/to/false", '/') );
    assert_true( nini_write_bool(&root, "path/to/false", '/', false) );
    assert_false( nini_read_bool(&root, "path/to/false", '/', true) );

    nini_root_deinit(&root);
}
//------------------------------------------------------------------------------
int test_helper(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(helper_read_test),
        cmocka_unit_test(helper_write_test),
    };

    return cmocka_run_group_tests_name("helper_test", tests, NULL, NULL);
}
//------------------------------------------------------------------------------
