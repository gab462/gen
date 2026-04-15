CC := clang
CFLAGS := -std=c23 -pedantic -Wall -Wextra -Wshadow
LDFLAGS := -fsanitize=address,undefined -ggdb

main: main.c array.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

array.h: gen ../util/array.gen.h
	./gen

gen: gen.c ../gen.h
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf gen array.h main *.dSYM
