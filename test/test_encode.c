#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include "nini_root.h"
#include "formats.h"
#include "ninidump.h"
#include "test_encode.h"

//------------------------------------------------------------------------------
static
void value_types_encode_test(void **state)
{
    nini_root_t root;
    nini_root_init(&root, &format_no_indents);

    nini_node_t *sec_string = nini_node_create_section("string");
    assert_non_null( sec_string );
    assert_true( nini_root_link_child(&root, sec_string) );

    nini_node_t *node_spaces = nini_node_create_string("spaces", "string with spaces");
    assert_non_null( node_spaces );
    assert_true( nini_node_link_child(sec_string, node_spaces) );

    nini_node_t *node_quotes = nini_node_create_string("quotes", "string with quotes");
    assert_non_null( node_quotes );
    assert_true( nini_node_link_child(sec_string, node_quotes) );

    nini_node_t *node_special = nini_node_create_string("special", "string with special characters: ;#");
    assert_non_null( node_special );
    assert_true( nini_node_link_child(sec_string, node_special) );

    nini_node_t *node_escapes = nini_node_create_string("escapes", "string with escape characters: \\\'\":=#;\n\r\t\x41\0");
    assert_non_null( node_escapes );
    assert_true( nini_node_link_child(sec_string, node_escapes) );

    nini_node_t *sec_integer = nini_node_create_section("integer");
    assert_non_null( sec_integer );
    assert_true( nini_root_link_child(&root, sec_integer) );

    nini_node_t *node_decimal = nini_node_create_decimal("decimal", 13579);
    assert_non_null( node_decimal );
    assert_true( nini_node_link_child(sec_integer, node_decimal) );

    nini_node_t *node_hexa = nini_node_create_hexa("hexadecimal", 0x1A7B);
    assert_non_null( node_hexa );
    assert_true( nini_node_link_child(sec_integer, node_hexa) );

    nini_node_t *sec_floating = nini_node_create_section("floating");
    assert_non_null( sec_floating );
    assert_true( nini_root_link_child(&root, sec_floating) );

    nini_node_t *node_float = nini_node_create_float("float", 3.14159265);
    assert_non_null( node_float );
    assert_true( nini_node_link_child(sec_floating, node_float) );

    nini_node_t *sec_boolean = nini_node_create_section("boolean");
    assert_non_null( sec_boolean );
    assert_true( nini_root_link_child(&root, sec_boolean) );

    nini_node_t *node_true = nini_node_create_bool("true", true);
    assert_non_null( node_true );
    assert_true( nini_node_link_child(sec_boolean, node_true) );

    nini_node_t *node_false = nini_node_create_bool("false", false);
    assert_non_null( node_false );
    assert_true( nini_node_link_child(sec_boolean, node_false) );

    nini_node_t *sec_null = nini_node_create_section("null");
    assert_non_null( sec_null );
    assert_true( nini_root_link_child(&root, sec_null) );

    nini_node_t *node_null = nini_node_create_null("null");
    assert_non_null( node_null );
    assert_true( nini_node_link_child(sec_null, node_null) );

    assert_true( nini_root_save_file(&root, "temp/value-types.ini", NULL) );

    // Compare file with the target.
    nini_root_clear(&root);
    assert_true( nini_root_load_file(&root, "temp/value-types.ini", NULL) );
    ninidump(&root, '/', "temp/value-types.dump");
    assert_int_equal( 0, system("diff temp/value-types.dump samples/value-types.dump") );

    nini_root_deinit(&root);
}
//------------------------------------------------------------------------------
static
void indents_encode_test(void **state)
{
    nini_root_t root;
    nini_root_init(&root, &format_have_indents);

    nini_node_t *base1 = nini_node_create_section("base-1");
    assert_non_null( base1 );
    assert_true( nini_root_link_child(&root, base1) );

    nini_node_t *child1 = nini_node_create_string("child-1", "value-1");
    assert_non_null( child1 );
    assert_true( nini_node_link_child(base1, child1) );

    nini_node_t *child2 = nini_node_create_string("child-2", "value-2");
    assert_non_null( child2 );
    assert_true( nini_node_link_child(base1, child2) );

    nini_node_t *sub1 = nini_node_create_section("sub-1");
    assert_non_null( sub1 );
    assert_true( nini_node_link_child(base1, sub1) );

    nini_node_t *child3 = nini_node_create_string("child-3", "value-3");
    assert_non_null( child3 );
    assert_true( nini_node_link_child(sub1, child3) );

    nini_node_t *child4 = nini_node_create_string("child-4", "value-4");
    assert_non_null( child4 );
    assert_true( nini_node_link_child(sub1, child4) );

    nini_node_t *sub2 = nini_node_create_section("sub-2");
    assert_non_null( sub2 );
    assert_true( nini_node_link_child(base1, sub2) );

    nini_node_t *child5 = nini_node_create_string("child-5", "value-5");
    assert_non_null( child5 );
    assert_true( nini_node_link_child(sub2, child5) );

    nini_node_t *child6 = nini_node_create_string("child-6", "value-6");
    assert_non_null( child6 );
    assert_true( nini_node_link_child(sub2, child6) );

    nini_node_t *sub3 = nini_node_create_section("sub-3");
    assert_non_null( sub3 );
    assert_true( nini_node_link_child(base1, sub3) );

    nini_node_t *sub4 = nini_node_create_section("sub-4");
    assert_non_null( sub4 );
    assert_true( nini_node_link_child(base1, sub4) );

    nini_node_t *child7 = nini_node_create_string("child-7", "value-7");
    assert_non_null( child7 );
    assert_true( nini_root_link_child(&root, child7) );

    nini_node_t *base2 = nini_node_create_section("base-2");
    assert_non_null( base2 );
    assert_true( nini_root_link_child(&root, base2) );

    assert_true( nini_root_save_file(&root, "temp/indents.ini", NULL) );

    // Compare file with the target.
    nini_root_clear(&root);
    assert_true( nini_root_load_file(&root, "temp/indents.ini", NULL) );
    ninidump(&root, '/', "temp/indents.dump");
    assert_int_equal( 0, system("diff temp/indents.dump samples/indents.dump") );

    nini_root_deinit(&root);
}
//------------------------------------------------------------------------------
int test_encode(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(value_types_encode_test),
        cmocka_unit_test(indents_encode_test),
    };

    return cmocka_run_group_tests_name("encode_test", tests, NULL, NULL);
}
//------------------------------------------------------------------------------
