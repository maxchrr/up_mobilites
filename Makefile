CFLAGS = -Wall -Wextra -pedantic -std=c99 -ggdb

exec = up_mobilites

.PHONY: clean

$(exec): main.o list.a #bus.a
	cc -o $(exec) main.o -L. -llist

main.o: main.c list.h #bus.h
	cc $(CFLAGS) -c main.c

list.a: list.o sort.o utils.o
	ar rcs liblist.a $^

list.o: list.c list.h
	cc $(CFLAGS) -c list.c

sort.o: sort.c sort.h
	cc $(CFLAGS) -c sort.c

utils.o: utils.c list.h utils.h
	cc $(CFLAGS) -c utils.c

bus.a: bus.o
	ar rcs libbus.a $^

bus.o: bus.c bus.h
	cc $(CFLAGS) -c bus.c

clean:
	$(RM) -r *.{a,o} $(exec)
