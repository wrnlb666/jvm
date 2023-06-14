#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "jvm.h"


trap_type vm_malloc( vm_t* vm )
{
    word_t size, addr;
    trap_type err = vm_stack_pop( vm, &size );
    if ( err != TRAP_OK )
    {
        return err;
    }
    addr.as_ptr = calloc( size.as_uint, sizeof (word_t) );
    if ( addr.as_ptr == NULL )
    {
        fprintf( stderr, "[ERRO]: Out of memory\n" );
        exit(1);
    }
    vm_stack_push( vm, addr );
    return TRAP_OK;
}

trap_type vm_free( vm_t* vm )
{
    word_t addr;
    trap_type err = vm_stack_pop( vm, &addr );
    if ( err != TRAP_OK )
    {
        return err;
    }
    free( addr.as_ptr );
    return TRAP_OK;
}

trap_type vm_heap_push( vm_t* vm )
{
    word_t addr, index, value;
    trap_type err = vm_stack_pop( vm, &value );
    if ( err != TRAP_OK )
    {
        return err;
    }
    err = vm_stack_pop( vm, &index );
    if ( err != TRAP_OK )
    {
        return err;
    }
    err = vm_stack_pop( vm, &addr );
    if ( err != TRAP_OK )
    {
        return err;
    }
    word_t* address = addr.as_ptr;
    address[ index.as_uint ] = value;
    return TRAP_OK;
}

trap_type vm_heap_pop( vm_t* vm )
{
    word_t addr, index, value;
    trap_type err = vm_stack_pop( vm, &index );
    if ( err != TRAP_OK )
    {
        return err;
    }
    err = vm_stack_pop( vm, &addr );
    if ( err != TRAP_OK )
    {
        return err;
    }
    word_t* address = addr.as_ptr;
    value = address[ index.as_uint ];
    vm_stack_push( vm, value );
    return TRAP_OK;
}

#define GC

// mark and sweep GC
#ifdef GC
#include <pthread.h>
#include <stdatomic.h>

typedef struct addr
{
    void*   ptr;
    size_t  size;
    bool    is_atomic;
    bool    is_marked;
} addr_t;

typedef struct node
{
    addr_t          addr;
    struct node*    prev;
    struct node*    next;
} node_t;

typedef struct minfo
{
    node_t* head;
    node_t* tail;
    size_t  size;
    size_t  total_size;
} minfo_t;

static void ll_push( minfo_t* minfo, node_t* node )
{
    if ( minfo->size == 0 )
    {
        minfo->head = minfo->tail = node;
        minfo->size++;
        minfo->total_size += node->addr.size;
        return;
    }
    node->prev = minfo->tail;
    minfo->tail->next = node;
    minfo->tail = node;
    minfo->size++;
    minfo->total_size += node->addr.size;
    return;
}

static void ll_pop( minfo_t* minfo, node_t* node )
{
    minfo->total_size -= node->addr.size;
    minfo->size--;
    if ( minfo->size == 0 )
    {
        minfo->head = minfo->tail = NULL;
    }
    else if ( node == minfo->head )
    {
        minfo->head = node->next;
        minfo->head->prev = NULL;
    }
    else if ( node == minfo->tail )
    {
        minfo->tail = node->prev;
        minfo->tail->next = NULL;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    free( node->addr.ptr );
    free( node );
}

static void ll_cleanup( node_t* node )
{
    if ( node == NULL )
    {
        return;
    }
    ll_cleanup( node->next );
    free( node->addr.ptr );
    free( node );
}

#define DEFAULT_HEAP_CHECK  8

minfo_t         allocated = { 0 };
pthread_t       gc_thread = { 0 };
bool            init = false;
volatile bool   gc_should_finish = false;
vm_t*           this_vm;
size_t          heap_check = DEFAULT_HEAP_CHECK;

static void gc_operate( vm_t* vm );
static void* _gc_thread( void* args )
{
    (void) args;
    while ( gc_should_finish == false && this_vm->halt == false )
    {
        while ( allocated.total_size < heap_check )
        {
            if ( gc_should_finish == true || this_vm->halt == true )
            {
                pthread_exit( NULL );
            }
            sched_yield();
        }
        gc_operate( this_vm );
        size_t size = allocated.total_size * 2;
        heap_check = size < DEFAULT_HEAP_CHECK ? DEFAULT_HEAP_CHECK : size;
    }
    return NULL;
}

static void vm_gc_start( vm_t* vm )
{
    this_vm = vm;
    pthread_create( &gc_thread, NULL, _gc_thread, NULL );
}

#ifdef __GNUC__
__attribute__((destructor))
#endif  // __GNUC__
static void vm_gc_finish( void )
{
    gc_should_finish = true;
    pthread_join( gc_thread, NULL );
    ll_cleanup( allocated.head );
}

trap_type vm_alloc( vm_t* vm )
{
    if ( init == false )
    {
        vm_gc_start(vm);
        #ifndef __GNUC__
        atexit( vm_gc_finish );
        #endif  // __GNUC__
        init = true;
    }

    word_t size, addr;
    trap_type err = vm_stack_pop( vm, &size );
    if ( err != TRAP_OK )
    {
        return err;
    }
    addr.as_ptr = calloc( size.as_uint, sizeof (word_t) );
    if ( addr.as_ptr == NULL )
    {
        fprintf( stderr, "[ERRO]: Out of memory\n" );
        exit(1);
    }
    vm_stack_push( vm, addr );

    node_t* node = malloc( sizeof (node_t) );
    *node = (node_t)
    {
        .addr = 
        {
            .ptr = addr.as_ptr,
            .size = size.as_uint,
            .is_atomic = false,
        },
    };
    ll_push( &allocated, node );

    return TRAP_OK;
}

trap_type vm_alloc_atomic( vm_t* vm )
{
    if ( init == false )
    {
        vm_gc_start(vm);
        atexit( vm_gc_finish );
        init = true;
    }

    word_t size, addr;
    trap_type err = vm_stack_pop( vm, &size );
    if ( err != TRAP_OK )
    {
        return err;
    }
    addr.as_ptr = calloc( size.as_uint, sizeof (word_t) );
    if ( addr.as_ptr == NULL )
    {
        fprintf( stderr, "[ERRO]: Out of memory\n" );
        exit(1);
    }
    vm_stack_push( vm, addr );

    node_t* node = malloc( sizeof (node_t) );
    *node = (node_t)
    {
        .addr = 
        {
            .ptr = addr.as_ptr,
            .size = size.as_uint,
            .is_atomic = true,
        },
    };
    ll_push( &allocated, node );

    return TRAP_OK;
}

static void gc_mark_heap( addr_t* addr )
{
    word_t* ptr = (word_t*) addr->ptr;
    for ( size_t i = 0; i < addr->size; i++ )
    {
        for ( node_t* curr_node = allocated.head; curr_node != NULL; curr_node = curr_node->next )
        {
            // already marked
            if ( curr_node->addr.is_marked == true )
            {
                continue;
            }
            if ( ptr[i].as_ptr == curr_node->addr.ptr )
            {
                if ( curr_node->addr.is_atomic == true )
                {
                    // atomic
                    curr_node->addr.is_marked = true;
                    gc_mark_heap( &curr_node->addr );
                    break;
                }
                else
                {
                    // non-atomic
                    curr_node->addr.is_marked = true;
                    break;
                }
            }
        }
    }
}

static void gc_mark( word_t* stack, size_t size )
{
    for ( size_t i = 0; i < size; i++ )
    {
        for ( node_t* curr_node = allocated.head; curr_node != NULL; curr_node = curr_node->next )
        {
            // already marked
            if ( curr_node->addr.is_marked == true )
            {
                continue;
            }
            if ( stack[i].as_ptr == curr_node->addr.ptr )
            {
                if ( curr_node->addr.is_atomic == false )
                {
                    // atomic
                    curr_node->addr.is_marked = true;
                    gc_mark_heap( &curr_node->addr );
                    break;
                }
                else
                {
                    // non-atomic
                    curr_node->addr.is_marked = true;
                    break;
                }
            }
        }
    }
}

static void gc_sweep( void )
{
    for ( node_t* addr = allocated.head; addr != NULL; )
    {
        if ( addr->addr.is_marked == true )
        {
            addr->addr.is_marked = false;
            addr = addr->next;
        }
        else
        {
            node_t* temp = addr;
            addr = addr->next;
            ll_pop( &allocated, temp );
        }
    }
}

static void gc_operate( vm_t* vm )
{
    // pause the vm
    pthread_mutex_lock( &vm->pause );

    size_t size = vm->stack_size;
    #ifdef __STDC_NO_VLA__
    word_t* stack   = malloc( sizeof (word_t) * size );
    #else
    word_t  stack[size];
    #endif  // __STDC_NO_VLA__

    // copy vm's stack to buffer stack
    stack_obtain( vm->stack, stack, size );

    // resume the vm
    pthread_mutex_unlock( &vm->pause );

    // mark
    gc_mark( stack, size );
    
    // sweep
    gc_sweep();


    #ifdef __STDC_NO_VLA__
    free( stack );
    #endif  // __STDC_NO_VLA__
}

void gc_run( vm_t* vm )
{
    size_t size = vm->stack_size;
    #ifdef __STDC_NO_VLA__
    word_t* stack   = malloc( sizeof (word_t) * size );
    #else
    word_t  stack[size];
    #endif  // __STDC_NO_VLA__

    // copy vm's stack to buffer stack
    stack_obtain( vm->stack, stack, size );

    // mark
    gc_mark( stack, size );
    
    // sweep
    gc_sweep();


    #ifdef __STDC_NO_VLA__
    free( stack );
    #endif  // __STDC_NO_VLA__
}


#endif  // GC