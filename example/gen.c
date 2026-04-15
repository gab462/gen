#include "../gen.h"

#define GEN_DECLARATION
#include "../util/array.gen.h"

int
main(void)
{
    gen(gen_array(), "array.h", (char*[]){ "int", "char *", NULL });
}
