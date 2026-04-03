#include <stdlib.h>
#include "array.h"

#include <stdio.h>

int
main(void)
{
    Array(int) arr = {0};

    array_push(&arr, 10);
    array_push(&arr, 11);
    array_push(&arr, 12);
    array_push(&arr, 13);

    for (int i = 0; i < arr.len; i++)
        printf("%d\n", arr.items[i]);

    array_delete(&arr);

    return 0;
}
