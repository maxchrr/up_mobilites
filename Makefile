exec = up_mobilites

$(exec): main.o list.a #bus.a
	cc -o $(exec) main.o -L. -llist

main.o: main.c list.h #bus.h
	cc -c main.c

list.a: list.o #sort.o
	ar rcs liblist.a $^

list.o: list.c list.h
	cc -c list.c

sort.o: sort.c sort.h
	cc -c sort.c

bus.a: bus.o
	ar rcs libbus.a $^

bus.o: bus.c bus.h
	cc -c bus.c

clean:
	$(RM) -r *.{a,o} $(exec)
