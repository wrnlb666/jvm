#include "stack.h"

static void* stack_alloc( size_t size )
{
    void* temp = calloc( 1, size );
    if ( temp == NULL )
    {
        fprintf( stderr, "[ERRO]: VM out of memory\n" );
        exit(1);
    }
    return temp;
}

stack_t* stack_create( void )
{
    stack_t* result = stack_alloc( sizeof (stack_t) );
    result->head = result->tail = stack_alloc( sizeof (node_t) );
    result->curr_capacity = PAGE_SIZE;
    return result;
}

static void _stack_free( node_t* node )
{
    while ( node->next != NULL )
    {
        _stack_free( node->next );
    }
    free( node );
    node = NULL;
}

void stack_free( stack_t* stack )
{
    _stack_free( stack->head );
    free( stack );
}

void stack_grow( stack_t* stack, size_t new_size )
{
    if ( new_size < stack->curr_capacity )
    {
        return;
    }
    size_t page_count = ( stack->curr_capacity / PAGE_SIZE ) + 1;
    size_t new_page_count = ( new_size / PAGE_SIZE ) + 1;
    size_t counts = new_page_count - page_count;
    node_t* temp = stack->tail;
    for ( size_t i = 0; i < counts; i++ )
    {
        temp->next = stack_alloc( sizeof (node_t) );
        temp = temp->next;
    }
    stack->tail = temp;
    stack->curr_capacity = new_page_count * PAGE_SIZE;
}

void stack_shrink( stack_t* stack, size_t new_size )
{
    if ( new_size > stack->curr_capacity )
    {
        fprintf( stderr, "[ERRO]: 'stack_shrink': unable to shrink to a bigger size\n" );
        exit(1);
    }
    size_t page_count = ( stack->curr_capacity / PAGE_SIZE ) + 1;
    size_t new_page_count = ( new_size / PAGE_SIZE ) + 1;
    size_t counts = page_count - new_page_count;
    if ( counts <= 2 )
    {
        return;
    }
    node_t* target = stack->head;
    for ( size_t i = 0; i < new_page_count; i++, target = target->next );
    _stack_free( target->next );
    target->next = NULL;
    stack->tail = target;
}

word_t* stack_get( stack_t* stack, size_t index )
{
    if ( index >= stack->curr_capacity )
    {
        fprintf( stderr, "[ERRO]: 'stack_get' index of out bounds\n" );
        exit(1);
    }
    node_t* target  = stack->head;
    size_t page_i   = index / PAGE_SIZE;
    size_t inner    = index % PAGE_SIZE;
    for ( size_t i = 0; i < page_i; i++ )
    {
        target = target->next;
    }
    return &( target->data[inner] );
}