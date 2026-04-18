#include "../gen.h"

#define GEN_DECLARATION
#include "../util/array.gen.h"
#include "../util/table.gen.h"

int
main(void)
{
    gen(gen_array(), "array.h", (char*[]){ "int", "char *", NULL });
    gen(gen_table(), "table.h", (char*[]){ "table_make_type(char, size_t)", NULL });
}
