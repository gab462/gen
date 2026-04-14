#include "../gen.h"

int
main(void)
{
    GenModule array = {
        .source = "array.gen.h",
        .output = "array.h",
        .name = "Array",
        .typename = "T",
        .member_functions = (char*[]){
            "array_push",
            "array_delete",
            NULL
        },
    };

    gen(array, (char*[]){ "int", "char *", NULL });
}
