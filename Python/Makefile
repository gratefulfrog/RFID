CC=cc
CFLAGS=-fPIC -shared -MD

all: libsum.so

libsum.so: sum.c
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.d
	rm -f *.so

-include $(OBJ:.o=.d)
