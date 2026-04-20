#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"
#include "table.h"

size_t
cstr_hasheq(void *a, void *b, size_t)
{
    char *k1 = *(char **) a;

    if (b == NULL) {
        // fnv-1a
        uint64_t hash = 0xcbf29ce484222325;

        for (size_t i = 0; i < strlen(k1); ++i) {
            hash ^= (uint64_t) k1[i];
            hash *= 0x100000001b3;
        }

        return hash;
    } else {
        char *k2 = *(char **) b;

        return strcmp(k1, k2) == 0;
    }
}

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

    Table(char *, size_t) table = { .hasheq = cstr_hasheq };

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
