CC=gcc
CFLAGS=-fPIC -MD
LIBFLAGS=-shared -MD
OBJ=printInt.o printChar.o printFloat.o printString.o

all: lib.so

lib.so: $(OBJ)
	$(CC) -o $@ $? $(LIBFLAGS)

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.d
	rm -f *.so
	rm -f a.out

-include $(OBJ:.o=.d)


