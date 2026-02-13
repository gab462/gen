CC := clang
CFLAGS := -std=c23 -pedantic -Wall -Wextra -Wshadow
LDFLAGS := -fsanitize=address,undefined -ggdb

main: main.o
	$(CC) -o $@ *.o $(LDFLAGS)

main.o: main.c array.h
	$(CC) $(CFLAGS) -o $@ -c $< $(LDFLAGS)

array.h: gen
	./gen

gen: gen.c ../gen.h
	$(CC) $(CFLAGS) -D"CC=\"$(CC) \"" -D"CFLAGS=\"$(CFLAGS) \"" -D"LDFLAGS=\"$(LDFLAGS) \"" -o $@ $< $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf gen array.h *.o main *.dSYM
