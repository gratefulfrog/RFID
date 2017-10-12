# This is the makefile for test.c
# it uses:
## g++ as the compiler
## automatic dependency generation (-include $(OBJ:.o=.d) and -MD flag
#  -lwiringPi to include the RPI wiring libraries, only available on RPI 
#
# use target: all to build applicatin
# use target: test to build csv writer test, and uncomment -include line at end
# 

CC=cc
#CFLAGS=-fPIC -MD
CFLAGS=-MD
MAKE_MAIN=-DMAIN
LIBCFLAGS=$(CFLAGS) -shared	

OBJ =  lib_rfid_driver.o  little_test.o
#common_uart.o

all: run

run: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(MAKE_MAIN)

lib: lib.so

lib.so: $(OBJ)
	$(CC) -o $@ $< $(LIBCFLAGS)

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.d
	rm -f run

-include $(OBJ:.o=.d)

