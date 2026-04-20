#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *source;
    char *name;
    char **typenames;
    char **member_functions;
} GenModule;

#define gen_foreach(it, arr) for (typeof(arr) it = arr; *it != NULL; ++it)

void
gen_declaration(FILE *output, GenModule mod, char **types, int index)
{
    // Set each typename to correspondent type
    int i = 0;
    gen_foreach(typename, mod.typenames)
        fprintf(output, "#define %s %s\n", *typename, types[i++]);

    // Define concrete type
    fprintf(output, "#define %s(...) %s__generic_%d\n", mod.name, mod.name, index);

    // Define concrete function names
    gen_foreach(func, mod.member_functions)
        fprintf(output, "#define %s %s__generic_%d\n", *func, *func, index);

    fprintf(output, "#include \"%s\"\n", mod.source);

    fprintf(output, "#undef %s\n", mod.name);
    gen_foreach(func, mod.member_functions)
        fprintf(output, "#undef %s\n", *func);
    gen_foreach(typename, mod.typenames)
        fprintf(output, "#undef %s\n", *typename);
    fprintf(output, "\n");
}

void
gen_print_typenames(void *output, GenModule mod)
{
    int i = 0;
    gen_foreach(type, mod.typenames) {
        fprintf(output, "%s", mod.typenames[i++]);
        if (mod.typenames[i] != NULL)
            fprintf(output, ", ");
    }
}

void
gen_type_accessor(FILE *output, GenModule mod, char ***typelists)
{
    // Dispatch on function pointer type with typenames as arguments
    fprintf(output, "#define %s(", mod.name);
    gen_print_typenames(output, mod);
    fprintf(output, ") typeof(_Generic((void (*)(");
    gen_print_typenames(output, mod);
    fprintf(output, "))0, \\\n");

    int typename_count = 0;
    gen_foreach(type, mod.typenames) ++typename_count;

    int i = 0;
    gen_foreach(types, typelists) {
        // Dispatch on function pointer for actual types
        fprintf(output, "   void (*)(");
        for (int j = 0; j < typename_count; ++j) {
            fprintf(output, "%s", (*types)[j]);
            if (j + 1 < typename_count)
                fprintf(output, ", ");
        }
        fprintf(output, "): (%s__generic_%d){0}", mod.name, i++);
        if (*(types + 1) != NULL)
            fprintf(output, ",");
        fprintf(output, " \\\n");
    }

    fprintf(output, "))\n\n");
}

void
gen_function_accessor(FILE *output, GenModule mod, char *function, void **args)
{
    fprintf(output, "#define %s(self, ...) _Generic(self, \\\n", function);

    int i = 0;
    gen_foreach(arg, args) { // arg only used for NULL checking
        fprintf(output, "   %s__generic_%d *: %s__generic_%d",
                mod.name, i, function, i);
        i++;
        if (*(arg + 1) != NULL)
            fprintf(output, ",");
        fprintf(output, " \\\n");
    }

    fprintf(output, ")(self __VA_OPT__(,) __VA_ARGS__)\n\n");
}

void
gen(GenModule mod, char *output_path, char ***typelists)
{
    FILE *output = fopen(output_path, "w+");

    fprintf(output, "// TEMPLATE INITIALIZATION\n\n");

    fprintf(output, "#define GEN_INSTANTIATION\n\n");

    int i = 0;
    gen_foreach(types, typelists)
        gen_declaration(output, mod, *types, i++);

    fprintf(output, "#undef GEN_INSTANTIATION\n\n");

    fprintf(output, "// TYPE ACCESSOR\n\n");

    gen_type_accessor(output, mod, typelists);

    fprintf(output, "// FUNCTION ACCESSORS\n\n");

    gen_foreach(func, mod.member_functions)
        gen_function_accessor(output, mod, *func, (void **) typelists);

    fclose(output);
}

void
gen1(GenModule mod, char *output_path, char **types)
{
    int type_count = 0;
    gen_foreach(type, types) ++type_count;

    // Transform types into array of arrays of single types
    char ***typelists = malloc(sizeof(char **) * (type_count + 1));
    typelists[type_count] = NULL;
    for (int i = 0; i < type_count; ++i)
        typelists[i] = &types[i];

    gen(mod, output_path, typelists);

    free(typelists);
}
