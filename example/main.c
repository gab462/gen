#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"
#include "table.h"

int
main(void)
{
    Array(int) arr = {0};

    array_push(&arr, 10);
    array_push(&arr, 11);
    array_push(&arr, 12);
    array_push(&arr, 13);

    for (size_t i = 0; i < arr.len; i++)
        printf("%d\n", arr.items[i]);

    array_free(&arr);

    Table(char, size_t) table = {0};

    for (size_t i = 0; i <= 26; ++i) {
        table_set(&table, 'a' + i, i);
    }

    table_del(&table, 'd');

    printf("%zu %zu %zu\n", table.count, table.cap, *table_get(&table, 'z'));

    table_free(&table);

    return 0;
}
