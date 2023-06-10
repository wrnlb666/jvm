#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PAGE_SIZE 511

typedef union word
{
    uint64_t    as_uint;
    int64_t     as_int;
    double      as_double;
    void*       as_ptr;
} word_t;

typedef struct node
{
    word_t          data[PAGE_SIZE];
    struct node*    next;
} node_t;


typedef struct stack
{
    size_t      curr_capacity;
    node_t*     head;
    node_t*     tail;
} stack_t;



stack_t*    stack_create( void );
void        stack_free( stack_t* stack );
word_t*     stack_get( stack_t* stack, size_t index );
void        stack_grow( stack_t* stack, size_t new_size );
void        stack_shrink( stack_t* stack, size_t new_size );




#endif  // __STACK_H__