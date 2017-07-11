#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "nini_root.h"
#include "formats.h"
#include "ninidump.h"
#include "test_decode.h"

//------------------------------------------------------------------------------
static
void value_types_decode_test(void **state)
{
    nini_root_t root;
    nini_root_init(&root, &format_no_indents);

    assert_true( nini_root_load_file(&root, "samples/value-types.ini", NULL) );
    ninidump(&root, '/', "value-types.dump");
    assert_int_equal( 0, system("diff value-types.dump samples/value-types.dump") );

    nini_root_deinit(&root);
}
//------------------------------------------------------------------------------
static
void indents_decode_test(void **state)
{
    nini_root_t root;
    nini_root_init(&root, &format_have_indents);

    assert_true( nini_root_load_file(&root, "samples/indents.ini", NULL) );
    ninidump(&root, '/', "indents.dump");
    assert_int_equal( 0, system("diff indents.dump samples/indents.dump") );

    nini_root_deinit(&root);
}
//------------------------------------------------------------------------------
static
void comments_decode_test(void **state)
{
    nini_root_t root;
    nini_root_init(&root, &format_have_indents);

    assert_true( nini_root_load_file(&root, "samples/comments.ini", NULL) );
    ninidump(&root, '/', "comments.dump");
    assert_int_equal( 0, system("diff comments.dump samples/comments.dump") );

    nini_root_deinit(&root);
}
//------------------------------------------------------------------------------
int test_decode(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(value_types_decode_test),
        cmocka_unit_test(indents_decode_test),
        cmocka_unit_test(comments_decode_test),
    };

    return cmocka_run_group_tests_name("decode_test", tests, NULL, NULL);
}
//------------------------------------------------------------------------------
