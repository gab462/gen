#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef CC
#define CC "cc "
#endif

typedef struct {
	char buf[1024];
	int end;
} Cmd;

void
cmd_append(Cmd *cmd, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	cmd->end += vsnprintf(cmd->buf + cmd->end, sizeof(cmd->buf) - cmd->end, fmt, args);
	va_end(args);
}

void
gen_declaration(FILE *header, char *param, char *param_type, char *name, char *gen_types[], int gen_type_count, int index)
{
	fprintf(header, "#define %s %s\n", param, param_type);
	fprintf(header, "#define %s(...) %s__generic_%d\n", name, name, index);
	for (int i = 0; i < gen_type_count; i++)
		fprintf(header, "#define %s(...) %s__generic_%d\n",
			gen_types[i], gen_types[i], index);

	fprintf(header, "#include \"%s.gen.h\"\n", name);

	fprintf(header, "#undef %s\n", name);
	for (int i = 0; i < gen_type_count; i++)
		fprintf(header, "#undef %s\n", gen_types[i]);
	fprintf(header, "#undef %s\n", param);
	fprintf(header, "\n");
}

void
gen_module_accessor(FILE *header, char *name, char *param, char *param_types[], int param_type_count)
{
	fprintf(header, "#define %s(%s) _Generic((%s){0}, \\\n",
		name, param, param);

	for (int i = 0; i < param_type_count; i++) {
		fprintf(header, "	%s: %s__generic_%d", param_types[i], name, i);
		if (i + 1 != param_type_count)
			fprintf(header, ",");
		fprintf(header, " \\\n");
	}

	fprintf(header, ")\n\n");
}

void
gen_type_accessor(FILE *header, char *gen_type, char *param, char *param_types[], int param_type_count)
{
	fprintf(header, "#define %s(%s) typeof(_Generic((%s){0}, \\\n",
		gen_type, param, param);

	for (int i = 0; i < param_type_count; i++) {
		fprintf(header, "	%s: (%s__generic_%d){0}",
			param_types[i], gen_type, i);
		if (i + 1 != param_type_count)
			fprintf(header, ",");
		fprintf(header, " \\\n");
	}


	fprintf(header, "))\n\n");
}

void
gen_header(char *name, char *param, char *param_types[], int param_type_count, char *gen_types[], int gen_type_count)
{
	char path[255] = {0};
	snprintf(path, sizeof(path), "%s.h", name);

	FILE *header = fopen(path, "w+");

	for (int i = 0; i < param_type_count; i++) {
		gen_declaration(header, param, param_types[i], name, gen_types, gen_type_count, i);
	}

	gen_module_accessor(header, name, param, param_types, param_type_count);

	for (int i = 0; i < gen_type_count; i++) {
		gen_type_accessor(header, gen_types[i], param, param_types, param_type_count);
	}

	fclose(header);
}

void
gen_object(char *param, char *param_type, char *name, char *gen_types[], int gen_type_count, int index)
{
	Cmd cmd = {0};

	cmd_append(&cmd, CC);
	cmd_append(&cmd, "-c ");

#ifdef CFLAGS
	cmd_append(&cmd, CFLAGS);
#endif

	for (int i = 0; i < gen_type_count; i++) 
		cmd_append(&cmd, "-D'%s(...)=%s__generic_%d' ",
			gen_types[i], gen_types[i], index);

	cmd_append(&cmd, "-D'%s(...)=%s__generic_%d' ", name, name, index);

	cmd_append(&cmd, "-D'%s=%s' ", param, param_type);

	cmd_append(&cmd, "%s.gen.c -o %s__generic_%d.o ", name, name, index);

#ifdef LDFLAGS
	cmd_append(&cmd, LDFLAGS);
#endif

	printf("%s\n", cmd.buf);
	system(cmd.buf);
}
