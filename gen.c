#include "gen.h"

#define arrlen(arr) (sizeof(arr) / sizeof((arr)[0]))

int
main(void)
{
	char *name = "array";
	char *param = "T";
	char *gen_types[] = { "Array" };
	char *param_types[] = { "int", "char *" };

	gen_header(name, param, param_types, arrlen(param_types), gen_types, arrlen(gen_types));

	for (int i = 0; i < (int) arrlen(param_types); i++) {
		gen_object(param, param_types[i], name, gen_types, arrlen(gen_types), i);
	}
}
