#include "../gen.h"

#define GEN_DECLARATION
#include "array.gen.h"

int
main(void)
{
    gen(array_module(), "array.h", (char*[]){ "int", "char *", NULL });
}
