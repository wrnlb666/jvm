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

vm_stack_t* stack_create( void )
{
    vm_stack_t* result = stack_alloc( sizeof (vm_stack_t) );
    result->head = result->tail = stack_alloc( sizeof (stack_node_t) );
    result->curr_capacity = PAGE_SIZE;
    return result;
}

static void _stack_free( stack_node_t* node )
{
    while ( node->next != NULL )
    {
        _stack_free( node->next );
    }
    free( node );
    node = NULL;
}

void stack_free( vm_stack_t* stack )
{
    _stack_free( stack->head );
    free( stack );
}

void stack_grow( vm_stack_t* stack, size_t new_size )
{
    if ( new_size < stack->curr_capacity )
    {
        return;
    }
    size_t page_count = ( stack->curr_capacity / PAGE_SIZE ) + 1;
    size_t new_page_count = ( new_size / PAGE_SIZE ) + 1;
    size_t counts = new_page_count - page_count;
    stack_node_t* temp = stack->tail;
    for ( size_t i = 0; i < counts; i++ )
    {
        temp->next = stack_alloc( sizeof (stack_node_t) );
        temp = temp->next;
    }
    stack->tail = temp;
    stack->curr_capacity = new_page_count * PAGE_SIZE;
}

void stack_shrink( vm_stack_t* stack, size_t new_size )
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
    stack_node_t* target = stack->head;
    for ( size_t i = 0; i < new_page_count; i++, target = target->next );
    _stack_free( target->next );
    target->next = NULL;
    stack->tail = target;
}

word_t* stack_get( vm_stack_t* stack, size_t index )
{
    if ( index >= stack->curr_capacity )
    {
        fprintf( stderr, "[ERRO]: 'stack_get' index of out bounds\n" );
        exit(1);
    }
    stack_node_t* target    = stack->head;
    size_t page_i           = index / PAGE_SIZE;
    size_t inner            = index % PAGE_SIZE;
    for ( size_t i = 0; i < page_i; i++ )
    {
        target = target->next;
    }
    return &( target->data[inner] );
}

void stack_obtain( vm_stack_t* stack, word_t* arr, size_t stack_size )
{
    stack_node_t* target    = stack->head;
    size_t page_count       = stack_size / PAGE_SIZE;
    size_t inner            = stack_size % PAGE_SIZE;
    for ( size_t i = 0; i < page_count; i++ )
    {
        memcpy( arr + ( PAGE_SIZE * i ), target->data, sizeof (word_t) * PAGE_SIZE );
        target = target->next;
    }
    memcpy( arr + ( PAGE_SIZE * page_count ), target, sizeof (word_t) * inner );
}