CC = gcc
OUT = libfundamental
CFLAGS = -march=native -Wno-pointer-arith -Wno-unused-result -Wswitch-enum -Wno-unused-variable
INCLUDE = -Iinc
SOURCE = `find ./src -name *.c ! -name test.c`

.PHONY: clean docs
clean:
	rm -rf docs
	rm -rf $(OUT)*

test:
	clear
	$(CC) -O0 $(CFLAGS) -o $(OUT)_test -g -Wall -Wconversion $(INCLUDE) $(SOURCE) src/test.c $(LDFLAGS) 
	./$(OUT)_test

docs:
	clear
	make clean
	doxygen Doxyfile