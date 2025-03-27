exec = up_mobilites

$(exec): main.o bus.a list.a
	cc -o $(exec) main.o -L. -lbus -llist

main.o: main.c bus.h list.h
	cc -c main.c

bus.a: bus.o
	ar rcs libbus.a bus.o

bus.o: bus.c bus.h
	cc -c bus.c

list.a: list.o sort.o
	ar rcs liblist.a list.o sort.o

list.o: list.c list.h
	cc -c list.c

sort.o: sort.c sort.h
	cc -c sort.c

clean:
	$(RM) -r *.{a,o} $(exec)
