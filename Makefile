CC = gcc
CFLAG = -Wall -Wextra -std=c11 -Wpedantic -g
LDFLAG = -L. -ljvm
POST_FIX = 
ELF_FILES = 

ifeq ($(OS),Windows_NT)
	POST_FIX = dll
else
	UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CFLAG += -Wno-unused-result # -fsanitize=leak,bounds,address
		LDFLAG += -Wl,-rpath=./
		POST_FIX = so
		ELF_FILES := $(shell find . -type f -executable -exec sh -c 'file -b {} | grep -q ELF' \; -print)
    endif
endif

.PHONY: all jvm jvavc jvav print scan heap
all: jvm jvavc jvav print scan heap

jvm: src/jvm.c src/stack.c
	$(CC) $(CFLAG) -fPIC -shared $^ -o lib$@.$(POST_FIX)

jvavc: src/jvavc.c
	$(CC) $(CFLAG) $< -o $@ $(LDFLAG)

jvav: src/jvav.c
	$(CC) $(CFLAG) $< -o $@ $(LDFLAG) -ldl

print: src/print.c
	$(CC) $(CFLAG) -fPIC -shared $< -o lib$@.$(POST_FIX) $(LDFLAG)

scan: src/scan.c
	$(CC) $(CFLAG) -fPIC -shared $< -o lib$@.$(POST_FIX) $(LDFLAG)

heap: src/heap.c
	$(CC) $(CFLAG) -fPIC -shared $< -o lib$@.$(POST_FIX) $(LDFLAG) -lpthread

clean:
	rm *.dll *.exe *.clss $(ELF_FILES)
