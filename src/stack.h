#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define PAGE_SIZE 511

typedef union word
{
    uint64_t    as_uint;
    int64_t     as_int;
    double      as_double;
    void*       as_ptr;
} word_t;

typedef struct stack_node
{
    word_t              data[PAGE_SIZE];
    struct stack_node*  next;
} stack_node_t;


typedef struct vm_stack
{
    size_t          curr_capacity;
    stack_node_t*   head;
    stack_node_t*   tail;
} vm_stack_t;



vm_stack_t* stack_create( void );
void        stack_free( vm_stack_t* stack );
void        stack_grow( vm_stack_t* stack, size_t new_size );
void        stack_shrink( vm_stack_t* stack, size_t new_size );
word_t*     stack_get( vm_stack_t* stack, size_t index );
void        stack_obtain( vm_stack_t* stack, word_t* arr, size_t stack_size );




#endif  // __STACK_H__