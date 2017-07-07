#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include "nini_node.h"
#include "test_node.h"

//------------------------------------------------------------------------------
static
void node_null_test(void **state)
{
    nini_node_t *node = nini_node_create_null("name");
    assert_non_null( node );

    assert_string_equal( nini_node_get_name(node), "name" );
    assert_int_equal( nini_node_get_type(node), NINI_NULL );

    assert_false( nini_node_have_value(node) );
    assert_false( nini_node_have_child(node) );

    nini_node_release(node);
}
//------------------------------------------------------------------------------
static
void node_bool_test(void **state)
{
    nini_node_t *node = nini_node_create_bool("name", true);
    assert_non_null( node );

    assert_string_equal( nini_node_get_name(node), "name" );
    assert_int_equal( nini_node_get_type(node), NINI_BOOL );

    assert_true( nini_node_get_bool(node) );

    assert_false( nini_node_have_value(node) );
    assert_false( nini_node_have_child(node) );

    nini_node_release(node);
}
//------------------------------------------------------------------------------
static
void node_float_test(void **state)
{
    nini_node_t *node = nini_node_create_float("name", 3.14);
    assert_non_null( node );

    assert_string_equal( nini_node_get_name(node), "name" );
    assert_int_equal( nini_node_get_type(node), NINI_FLOAT );

    assert_true( nini_node_get_float(node) - 3.14 < 0.001 );

    assert_false( nini_node_have_value(node) );
    assert_false( nini_node_have_child(node) );

    nini_node_release(node);
}
//------------------------------------------------------------------------------
static
void node_hexa_test(void **state)
{
    nini_node_t *node = nini_node_create_hexa("name", 0x1357);
    assert_non_null( node );

    assert_string_equal( nini_node_get_name(node), "name" );
    assert_int_equal( nini_node_get_type(node), NINI_HEXA );

    assert_int_equal( nini_node_get_integer(node), 0x1357 );

    assert_false( nini_node_have_value(node) );
    assert_false( nini_node_have_child(node) );

    nini_node_release(node);
}
//------------------------------------------------------------------------------
static
void node_decimal_test(void **state)
{
    nini_node_t *node = nini_node_create_decimal("name", 13579);
    assert_non_null( node );

    assert_string_equal( nini_node_get_name(node), "name" );
    assert_int_equal( nini_node_get_type(node), NINI_DECIMAL );

    assert_int_equal( nini_node_get_integer(node), 13579 );

    assert_false( nini_node_have_value(node) );
    assert_false( nini_node_have_child(node) );

    nini_node_release(node);
}
//------------------------------------------------------------------------------
static
void node_string_test(void **state)
{
    nini_node_t *node = nini_node_create_string("name", "text");
    assert_non_null( node );

    assert_string_equal( nini_node_get_name(node), "name" );
    assert_int_equal( nini_node_get_type(node), NINI_STRING );

    assert_string_equal( nini_node_get_string(node), "text" );

    assert_false( nini_node_have_value(node) );
    assert_false( nini_node_have_child(node) );

    nini_node_release(node);
}
//------------------------------------------------------------------------------
static
void node_section_test(void **state)
{
    // Create section.

    nini_node_t *section = nini_node_create_section("section");
    assert_non_null( section );

    // Add children.

    nini_node_t *child1 = nini_node_create_null("child1");
    assert_non_null( child1 );
    assert_true( nini_node_link_child(section, child1) );

    nini_node_t *child2 = nini_node_create_null("child2");
    assert_non_null( child2 );
    assert_true( nini_node_link_child(section, child2) );

    nini_node_t *child3 = nini_node_create_null("child3");
    assert_non_null( child3 );
    assert_true( nini_node_link_child(section, child3) );

    // Check properties.

    assert_string_equal( nini_node_get_name(section), "section" );
    assert_int_equal( nini_node_get_type(section), NINI_SECTION );
    assert_false( nini_node_have_value(section) );
    assert_true( nini_node_have_child(section) );

    // Access children.

    assert_ptr_equal( nini_node_get_first_child(section), child1 );
    assert_ptr_equal( nini_node_get_last_child(section), child3 );
    assert_ptr_equal( nini_node_find_child(section, "child1"), child1 );
    assert_ptr_equal( nini_node_find_child(section, "child2"), child2 );
    assert_ptr_equal( nini_node_find_child(section, "child3"), child3 );
    assert_ptr_equal( nini_node_find_child(section, "child4"), NULL );

    // Access siblings.

    assert_ptr_equal( nini_node_get_next_sibling(child1), child2 );
    assert_ptr_equal( nini_node_get_next_sibling(child2), child3 );
    assert_ptr_equal( nini_node_get_next_sibling(child3), NULL );

    assert_ptr_equal( nini_node_get_prev_sibling(child3), child2 );
    assert_ptr_equal( nini_node_get_prev_sibling(child2), child1 );
    assert_ptr_equal( nini_node_get_prev_sibling(child1), NULL );

    // Clean up.

    nini_node_release(section);
}
//------------------------------------------------------------------------------
static
void node_unlink_test(void **state)
{
    // Create section.

    nini_node_t *section = nini_node_create_section("section");
    assert_non_null( section );

    // Add children.

    nini_node_t *child1 = nini_node_create_null("child1");
    assert_non_null( child1 );
    assert_true( nini_node_link_child(section, child1) );

    nini_node_t *child2 = nini_node_create_null("child2");
    assert_non_null( child2 );
    assert_true( nini_node_link_child(section, child2) );

    nini_node_t *child3 = nini_node_create_null("child3");
    assert_non_null( child3 );
    assert_true( nini_node_link_child(section, child3) );

    nini_node_t *child4 = nini_node_create_null("child4");
    assert_non_null( child4 );
    assert_true( nini_node_link_child(section, child4) );

    nini_node_t *child5 = nini_node_create_null("child5");
    assert_non_null( child5 );
    assert_true( nini_node_link_child(section, child5) );

    // Remove some children.

    nini_node_unlink(child1);
    nini_node_release(child1);

    nini_node_unlink(child3);
    nini_node_release(child3);

    nini_node_unlink(child5);
    nini_node_release(child5);

    // Access children.

    assert_ptr_equal( nini_node_get_first_child(section), child2 );
    assert_ptr_equal( nini_node_get_last_child(section), child4 );
    assert_ptr_equal( nini_node_find_child(section, "child1"), NULL );
    assert_ptr_equal( nini_node_find_child(section, "child2"), child2 );
    assert_ptr_equal( nini_node_find_child(section, "child3"), NULL );
    assert_ptr_equal( nini_node_find_child(section, "child4"), child4 );
    assert_ptr_equal( nini_node_find_child(section, "child5"), NULL );

    // Access siblings.

    assert_ptr_equal( nini_node_get_next_sibling(child2), child4 );
    assert_ptr_equal( nini_node_get_next_sibling(child4), NULL );

    assert_ptr_equal( nini_node_get_prev_sibling(child4), child2 );
    assert_ptr_equal( nini_node_get_prev_sibling(child2), NULL );

    // Clean up.

    nini_node_release(section);
}
//------------------------------------------------------------------------------
int test_node(void)
{
    struct CMUnitTest tests[] =
    {
        cmocka_unit_test(node_null_test),
        cmocka_unit_test(node_bool_test),
        cmocka_unit_test(node_float_test),
        cmocka_unit_test(node_hexa_test),
        cmocka_unit_test(node_decimal_test),
        cmocka_unit_test(node_string_test),
        cmocka_unit_test(node_section_test),
        cmocka_unit_test(node_unlink_test),
    };

    return cmocka_run_group_tests_name("node test", tests, NULL, NULL);
}
//------------------------------------------------------------------------------
