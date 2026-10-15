CC := clang
CFLAGS := -std=c23 -pedantic -Wall -Wextra -Wshadow
LDFLAGS := -fsanitize=address,undefined -ggdb

main: main.c array.h table.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

array.h: ../util/array.gen.h
	../gen $< int "char *" > $@

table.h: ../util/table.gen.h
	../gen $< "char *,size_t" > $@

.PHONY: clean
clean:
	rm -rf gen array.h table.h main *.dSYM
