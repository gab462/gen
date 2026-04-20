#include "../gen.h"

#define GEN_DECLARATION
#include "../util/array.gen.h"
#include "../util/table.gen.h"

int
main(void)
{
    gen(gen_array(), "array.h", (char*[]){ "int", "char *", NULL });
    genn(genn_table(), "table.h", (char**[]){
        (char*[]){ "char *", "size_t" },
        NULL,
    });
}
