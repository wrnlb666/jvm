CC = gcc
CFLAG = -Wall -Wextra -std=c11 -Wpedantic -g -O3
OBJ = stack.o jvm.o
LIB = -L. -lstack -ljvm
POST_FIX = 

ifeq ($(OS),Windows_NT)
	POST_FIX = dll
else
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -fsanitize=leak
		POST_FIX = so
    endif
endif

.PHONY: all stack jvm jvavc jvav print
all: stack jvm jvavc jvav print clean_install

stack: src/stack.c
	$(CC) $(CFLAG) -fPIC -shared $< -o lib$@.$(POST_FIX)

jvm: src/jvm.c
	$(CC) $(CFLAG) -fPIC -shared $< -o lib$@.$(POST_FIX) -L. -lstack

jvavc: src/jvavc.c
	$(CC) $(CFLAG) $< -o $@ $(LIB)

jvav: src/jvav.c
	$(CC) $(CFLAG) $< -o $@ $(LIB) -ldl

print: src/print.c
	$(CC) $(CFLAG) -fPIC -shared $< -o lib$@.$(POST_FIX) $(LIB)

clean:
	rm *.dll *.so *.exe *.clss

clean_install:
	rm *.o