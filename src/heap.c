#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


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


// mark and sweep GC

