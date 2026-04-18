CC := clang
CFLAGS := -std=c23 -pedantic -Wall -Wextra -Wshadow
LDFLAGS := -fsanitize=address,undefined -ggdb

main: main.c array.h table.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

array.h table.h: gen ../util/array.gen.h ../util/table.gen.h
	./gen

gen: gen.c ../gen.h ../util/array.gen.h ../util/table.gen.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf gen array.h table.h main *.dSYM
