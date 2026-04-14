#include "../gen.h"

int
main(void)
{
    GenModule array = {
        .source = "array.gen.h",
        .name = "Array",
        .typename = "T",
        .member_functions = (char*[]){
            "array_push",
            "array_delete",
            NULL
        },
    };

    gen(array, "array.h", (char*[]){ "int", "char *", NULL });
}
