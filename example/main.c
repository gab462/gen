#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
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

    Table(char *, size_t) table = { .hasheq = table_cstr_hasheq };

    char *pool = calloc(26 * 2, sizeof(char));
    size_t pool_bump = 0;

    for (size_t i = 0; i < 26; ++i) {
        char *s = pool + pool_bump;
        pool_bump += 2;

        s[0] = 'a' + i;

        table_set(&table, s, i);
    }

    table_del(&table, "b");

    printf("%zu %zu %zu\n", table.count, table.cap, *table_get(&table, "z"));

    free(pool);
    array_free(&arr);
    table_free(&table);

    return 0;
}
