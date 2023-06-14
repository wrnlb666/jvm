#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

#include "jvm.h"

#define BUF_SIZE 32

trap_type scan_int( vm_t* vm )
{
    word_t temp;
    errno = 0;
    char buf[ BUF_SIZE ] = { 0 };
    char* end_ptr = NULL;
    fgets( buf, BUF_SIZE, stdin );
    temp.as_int = strtoll( buf, &end_ptr, 10 );
    if ( errno == ERANGE )
    {
        temp.as_int = 0;
    }
    if ( *end_ptr != '\n' && *end_ptr != '\0' )
    {
        temp.as_int = 0;
    }
    vm_stack_push( vm, temp );
    return TRAP_OK;
}

trap_type scan_uint( vm_t* vm )
{
    word_t temp;
    errno = 0;
    char buf[ BUF_SIZE ] = { 0 };
    char* end_ptr = NULL;
    fgets( buf, BUF_SIZE, stdin );
    temp.as_uint = strtoull( buf, &end_ptr, 10 );
    if ( errno == ERANGE )
    {
        temp.as_uint = 0;
    }
    if ( *end_ptr != '\n' && *end_ptr != '\0' )
    {
        temp.as_uint = 0;
    }
    vm_stack_push( vm, temp );
    return TRAP_OK;
}

trap_type scan_float( vm_t* vm )
{
    word_t temp;
    errno = 0;
    char buf[ BUF_SIZE ] = { 0 };
    char* end_ptr = NULL;
    fgets( buf, BUF_SIZE, stdin );
    temp.as_double = strtod( buf, &end_ptr );
    if ( errno == ERANGE )
    {
        temp.as_double = 0;
    }
    if ( *end_ptr != '\n' && *end_ptr != '\0' )
    {
        temp.as_double = 0;
    }
    vm_stack_push( vm, temp );
    return TRAP_OK;
}

trap_type scan_utf32( vm_t* vm )
{
    word_t temp = { 0 };
    char str[5] = { 0 };
    fgets( str, 5, stdin );
    
    if ( !( str[0] & 0x80 ) )
    {
        temp.as_uint = str[0];
    }
    else if ( ( str[0] & 0xE0 ) == 0xC0 )
    {
        temp.as_uint = ( (uint64_t) ( str[0] & 0x1F ) << 6 ) | ( str[1] & 0x3F );
    }
    else if ( ( str[0] & 0xF0 ) == 0xE0 )
    {
        temp.as_uint = ( (uint64_t) ( str[0] & 0x0F ) << 12 ) | ( ( str[1] & 0x3F ) << 6 ) | ( str[2] & 0x3F );
    }
    else if ( ( str[0] & 0xF8 ) == 0xF0 )
    {
        temp.as_uint = ( (uint64_t) ( str[0] & 0x07 ) << 18 ) | ( ( str[1] & 0x3F ) << 12 ) | ( ( str[2] & 0x3F ) << 6 ) | ( str[3] & 0x3F );
    }
    else
    {
        temp.as_uint = 0;
    }

    vm_stack_push( vm, temp );
    return TRAP_OK;
}
