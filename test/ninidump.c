#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "ninidump.h"

//------------------------------------------------------------------------------
static
const char* get_type_desc(nini_type_t type)
{
    switch( type )
    {
    case NINI_ROOT    :  return "root";
    case NINI_SECTION :  return "section";
    case NINI_STRING  :  return "string";
    case NINI_DECIMAL :  return "decimal";
    case NINI_HEXA    :  return "hexa";
    case NINI_FLOAT   :  return "float";
    case NINI_BOOL    :  return "bool";
    case NINI_NULL    :  return "null";

    default:
        return "UNKNOWN";
    }
}
//------------------------------------------------------------------------------
static
const char* get_value_str(char *buf, size_t size, const nini_node_t *node)
{
    switch( nini_node_get_type(node) )
    {
    case NINI_STRING:
        snprintf(buf, size, "\"%s\"", nini_node_get_string(node));
        break;

    case NINI_DECIMAL:
        snprintf(buf, size, "\"%ld\"", nini_node_get_integer(node));
        break;

    case NINI_HEXA:
        snprintf(buf, size, "\"%lX\"", nini_node_get_integer(node));
        break;

    case NINI_FLOAT:
        snprintf(buf, size, "\"%lf\"", nini_node_get_float(node));
        break;

    case NINI_BOOL:
        snprintf(buf, size, "\"%s\"", nini_node_get_bool(node) ? "true" : "false");
        break;

    default:
        snprintf(buf, size, "\"\"");
        break;
    }

    return buf;
}
//------------------------------------------------------------------------------
static
void dump_node(FILE *file, const char *prefix, char delimiter, const nini_node_t *node)
{
    char sep[1+1] = {0};
    sep[0] = strlen(prefix) ? delimiter : 0;

    char name[NINI_MAX_LINE_CHARS+1];
    snprintf(name, sizeof(name), "%s%s%s", prefix, sep, nini_node_get_name(node));

    char value[NINI_MAX_LINE_CHARS+1];
    fprintf(file,
            "%s\t%s\t%s\n",
            name,
            get_type_desc(nini_node_get_type(node)),
            get_value_str(value, sizeof(value), node));

    for(const nini_node_t *child = nini_node_get_first_child_c(node);
        child;
        child = nini_node_get_next_sibling_c(child))
    {
        dump_node(file, name, delimiter, child);
    }
}
//------------------------------------------------------------------------------
void ninidump(const nini_root_t *root, char delimiter, const char *filename)
{
    /*
     * Print all data to text file, one item to one line, with format:
     *
     * path/to/the/key<HT><type-in-text><HT>"<value-in-text>"<LF>
     */
    FILE *file = fopen(filename, "w");
    assert( file );

    for(const nini_node_t *node = nini_root_get_first_child_c(root);
        node;
        node = nini_node_get_next_sibling_c(node))
    {
        dump_node(file, "", delimiter, node);
    }

    fclose(file);
}
//------------------------------------------------------------------------------
