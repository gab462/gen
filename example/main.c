#include "array.h"

#include <stdio.h>

int
main(void)
{
	Array(int) arr = {0};

	array(int).push(&arr, 10);
	array(int).push(&arr, 11);
	array(int).push(&arr, 12);
	array(int).push(&arr, 13);

	for (int i = 0; i < arr.len; i++) {
		printf("%d\n", arr.items[i]);
	}

	array(int).delete(arr);

	return 0;
}
