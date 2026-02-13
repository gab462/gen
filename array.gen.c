#include "array.gen.h"

#include <stdlib.h>

static
void
push(Array(T) *arr, T item)
{
	if (arr->len + 1 > arr->cap) {
		arr->cap = (arr->cap + 1) * 2;
		arr->items = realloc(arr->items, sizeof(T) * arr->cap);
	}
	arr->items[arr->len++] = item;
}

static
void
delete(Array(T) *arr)
{
	free(arr->items);
}

typeof(array(T)) array(T) = {
	.push = push,
	.delete = delete,
};
