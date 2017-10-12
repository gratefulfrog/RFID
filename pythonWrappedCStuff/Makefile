#DEFS=-DTEST -DMAIN
DEFS=-DTEST
CC=cc
CFLAGS=-fPIC -MD
LIBFLAGS=-shared -MD
OBJ=little_test.o lib_rfid_driver.o common_uart.o

all: run

lib: lib.so

run: $(OBJ)
	$(CC) -o $@ $? $(CFLAGS) $(DEFS)	

lib.so: $(OBJ)
	$(CC) -o $@ $? $(LIBFLAGS)

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.d
	rm -f *.so
	rm -f a.out run

-include $(OBJ:.o=.d)


