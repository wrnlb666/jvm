#include <stdio.h>
#include <ctype.h>


#include "jvm.h"


trap_type print_int( vm_t* vm )
{
    word_t temp;
    trap_type err = vm_stack_pop( vm, &temp );
    if ( err != TRAP_OK )
    {
        return err;
    }
    printf( "%" PRId64 "", temp.as_int );
    return TRAP_OK;
}

trap_type print_uint( vm_t* vm )
{
    word_t temp;
    trap_type err = vm_stack_pop( vm, &temp );
    if ( err != TRAP_OK )
    {
        return err;
    }
    printf( "%" PRIu64 "", temp.as_uint );
    return TRAP_OK;
}

trap_type print_float( vm_t* vm )
{
    word_t temp;
    trap_type err = vm_stack_pop( vm, &temp );
    if ( err != TRAP_OK )
    {
        return err;
    }
    printf( "%lf", temp.as_double );
    return TRAP_OK;
}

trap_type print_ptr( vm_t* vm )
{
    word_t temp;
    trap_type err = vm_stack_pop( vm, &temp );
    if ( err != TRAP_OK )
    {
        return err;
    }
    printf( "%p", temp.as_ptr );
    return TRAP_OK;
}

trap_type print_utf8( vm_t* vm )
{
    word_t temp;
    trap_type err = vm_stack_pop( vm, &temp );
    if ( err != TRAP_OK )
    {
        return err;
    }
    char output[5] = { 0 };
    if ( temp.as_uint < 0x80 )
    {
        output[0] = (char) temp.as_uint;
        output[1] = '\0';
    }
    else if (temp.as_uint < 0x800)
    {
        output[0] = (char) (0xC0 | ( ( temp.as_uint >> 6 ) & 0x1F ) );
        output[1] = (char) (0x80 | ( temp.as_uint & 0x3F ) );
        output[2] = '\0';
    }
    else if (temp.as_uint < 0x10000)
    {
        output[0] = (char) ( 0xE0 | ( ( temp.as_uint >> 12 ) & 0x0F ) );
        output[1] = (char) ( 0x80 | ( ( temp.as_uint >> 6 ) & 0x3F ) );
        output[2] = (char) ( 0x80 | ( temp.as_uint & 0x3F ) );
        output[3] = '\0';
    }
    else
    {
        output[0] = (char) ( 0xF0 | ( ( temp.as_uint >> 18 ) & 0x07 ) );
        output[1] = (char) ( 0x80 | ( ( temp.as_uint >> 12 ) & 0x3F ) );
        output[2] = (char) ( 0x80 | ( ( temp.as_uint >> 6 ) & 0x3F ) );
        output[3] = (char) ( 0x80 | ( temp.as_uint & 0x3F ) );
        output[4] = '\0';
    }
    printf( "%s", output );
    return TRAP_OK;
}

trap_type print_space( vm_t* vm )
{
    word_t temp;
    trap_type err = vm_stack_pop( vm, &temp );
    if ( err != TRAP_OK )
    {
        return err;
    }
    for ( uint64_t i = 0; i < temp.as_uint; i++ )
    {
        printf( " " );
    }
    return TRAP_OK;
}

trap_type print_newline( vm_t* vm )
{
    (void) vm;
    printf( "\n" );
    return TRAP_OK;
}
