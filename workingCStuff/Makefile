# This is the makefile for little_test.c
# it uses:
## cc as the compiler
## automatic dependency generation (-include $(OBJ:.o=.d) and -MD flag
#  -lwiringPi to include the RPI wiring libraries, only available on RPI 

## leave this for off rpi testing
TEST=-DTEST
## use this for on RPI
#TEST=-lwiringPi

CC=cc
CFLAGS=-MD $(TEST)
CTESTFLAGS=-MD
OBJ = lib_rfid_driver.o little_test.o

all: run

run: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.d
	rm -f run

-include $(OBJ:.o=.d)


