#include <stdio.h>
#include <string.h>

typedef struct {
    char *source;
    char *name;
    char *typename;
    char **member_functions;
} GenModule;

typedef struct {
    char *source;
    char *name;
    char **typenames;
    char **member_functions;
} GennModule;

#define gen_foreach(it, arr) for (typeof(arr) it = arr; *it != NULL; ++it)

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

    fprintf(output, "// TEMPLATE INITIALIZATION\n\n");

    fprintf(output, "#define GEN_INSTANTIATION\n\n");

    int i = 0;
    gen_foreach(type, types)
        gen_declaration(output, mod, *type, i++);

    fprintf(output, "#undef GEN_INSTANTIATION\n\n");

    fprintf(output, "// TYPE ACCESSOR\n\n");

    gen_type_accessor(output, mod, types);

    fprintf(output, "// FUNCTION ACCESSORS\n\n");

    gen_foreach(func, mod.member_functions)
        gen_function_accessor(output, mod, *func, types);

    fclose(output);
}

void
genn_declaration(FILE *output, GennModule mod, char **types, int index)
{
    int i = 0;
    gen_foreach(typename, mod.typenames)
        fprintf(output, "#define %s %s\n", *typename, types[i++]);
    fprintf(output, "#define %s(...) %s__generic_%d\n", mod.name, mod.name, index);
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
genn_print_typenames(void *output, GennModule mod)
{
    int i = 0;
    gen_foreach(type, mod.typenames) {
        fprintf(output, "%s", mod.typenames[i++]);
        if (mod.typenames[i] != NULL)
            fprintf(output, ", ");
    }
}

void
genn_type_accessor(FILE *output, GennModule mod, char ***typelists)
{
    fprintf(output, "#define %s(", mod.name);
    genn_print_typenames(output, mod);
    fprintf(output, ") typeof(_Generic((void (*)(");
    genn_print_typenames(output, mod);
    fprintf(output, "))0, \\\n");

    int typename_count = 0;
    gen_foreach(type, mod.typenames) ++typename_count;

    int i = 0;
    gen_foreach(types, typelists) {
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
genn_function_accessor(FILE *output, GennModule mod, char *function, char ***typelists)
{
    fprintf(output, "#define %s(self, ...) _Generic(self, \\\n", function);

    int i = 0;
    gen_foreach(typelist, typelists) {
        fprintf(output, "   %s__generic_%d *: %s__generic_%d",
                mod.name, i, function, i);
        i++;
        if (*(typelist + 1) != NULL)
            fprintf(output, ",");
        fprintf(output, " \\\n");
    }

    fprintf(output, ")(self __VA_OPT__(,) __VA_ARGS__)\n\n");
}

void
genn(GennModule mod, char *output_path, char ***typelists)
{
    FILE *output = fopen(output_path, "w+");

    fprintf(output, "// TEMPLATE INITIALIZATION\n\n");

    fprintf(output, "#define GEN_INSTANTIATION\n\n");

    int i = 0;
    gen_foreach(types, typelists)
        genn_declaration(output, mod, *types, i++);

    fprintf(output, "#undef GEN_INSTANTIATION\n\n");

    fprintf(output, "// TYPE ACCESSOR\n\n");

    genn_type_accessor(output, mod, typelists);

    fprintf(output, "// FUNCTION ACCESSORS\n\n");

    gen_foreach(func, mod.member_functions)
        genn_function_accessor(output, mod, *func, typelists);

    fclose(output);
}
