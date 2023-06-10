CC = gcc
CFLAG = -Wall -Wextra -std=c11 -Wpedantic -g -O3
LIB = -L. -lstack -ljvm
POST_FIX = 
ELF_FILES = 

ifeq ($(OS),Windows_NT)
	POST_FIX = dll
else
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -fsanitize=leak
		POST_FIX = so
		ELF_FILES := $(shell find . -type f -executable -exec sh -c 'file -b {} | grep -q ELF' \; -print)
    endif
endif

.PHONY: all stack jvm jvavc jvav print heap
all: stack jvm jvavc jvav print heap

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

heap: src/heap.c
	$(CC) $(CFLAG) -fPIC -shared $< -o lib$@.$(POST_FIX) $(LIB)

clean:
	rm *.dll *.exe *.clss $(ELF_FILES)
