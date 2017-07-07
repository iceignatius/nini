#include "test_node.h"
#include "test_decode.h"
#include "test_encode.h"
#include "test_helper.h"

int main(int argc, char *argv[])
{
    int res;

    if(( res = test_node() )) return res;
    if(( res = test_decode() )) return res;
    if(( res = test_encode() )) return res;
    if(( res = test_helper() )) return res;

    return 0;
}
