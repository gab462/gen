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

    Table(char *, size_t) table = { .hasheq = table_cstr_hasheq };

    for (size_t i = 0; i < 26; ++i) {
        char *s = calloc(2, sizeof(char));
        s[0] = 'a' + i;
        table_set(&table, s, i);
    }

    table_del(&table, "b");

    printf("%zu %zu %zu\n", table.count, table.cap, *table_get(&table, "z"));

    table_free(&table);

    return 0;
}
