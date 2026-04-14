#include <stdio.h>

typedef struct {
    char *source;
    char *output;
    char *name;
    char *typename;
    char **member_functions;
    char **types;
} GenModule;

#define gen_foreach(it, arr) for (char **it = arr; *it != NULL; ++it)

void
gen_declaration(FILE *output, GenModule mod, char *type, int index)
{
    fprintf(output, "#define %s %s\n", mod.typename, type);
    fprintf(output, "#define %s(...) %s__generic_%d\n", mod.name, mod.name, index);
    gen_foreach(func, mod.member_functions)
        fprintf(output, "#define %s %s__generic_%d\n", *func, *func, index);

    fprintf(output, "#include \"%s\"\n", mod.source);

    fprintf(output, "#undef %s\n", mod.name);
    gen_foreach(func, mod.member_functions)
        fprintf(output, "#undef %s\n", *func);
    fprintf(output, "#undef %s\n", mod.typename);
    fprintf(output, "\n");
}

void
gen_type_accessor(FILE *output, GenModule mod, char **types)
{
    fprintf(output, "#define %s(%s) typeof(_Generic((%s){0}, \\\n",
            mod.name, mod.typename, mod.typename);

    int i = 0;
    gen_foreach(type, types) {
        fprintf(output, "   %s: (%s__generic_%d){0}", *type, mod.name, i++);
        if (*(type + 1) != NULL)
            fprintf(output, ",");
        fprintf(output, " \\\n");
    }

    fprintf(output, "))\n\n");
}

void
gen_function_accessor(FILE *output, GenModule mod, char *function, char **types)
{
    fprintf(output, "#define %s(self, ...) _Generic(self, \\\n", function);

    int i = 0;
    gen_foreach(type, types) {
        fprintf(output, "   %s__generic_%d *: %s__generic_%d",
                mod.name, i, function, i);
        i++;
        if (*(type + 1) != NULL)
            fprintf(output, ",");
        fprintf(output, " \\\n");
    }

    fprintf(output, ")(self __VA_OPT__(,) __VA_ARGS__)\n\n");
}

void
gen(GenModule mod, char *output_path, char **types)
{
    FILE *output = fopen(output_path, "w+");

    int i = 0;
    gen_foreach(type, types)
        gen_declaration(output, mod, *type, i++);

    gen_type_accessor(output, mod, types);

    gen_foreach(func, mod.member_functions)
        gen_function_accessor(output, mod, *func, types);

    fclose(output);
}
