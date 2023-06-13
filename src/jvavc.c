#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



#include "jvm.h"
#include "stack.h"

#define MAX_LABEL       1024
#define MAX_LABEL_INDEX 16

char* file = NULL;

typedef struct label
{
    vm_cstr_t label;
    int index;
} label_t;

typedef struct rev_label
{
    vm_cstr_t   label;
    int         index[ MAX_LABEL_INDEX ];
    size_t      count;
} rev_label_t;


label_t     label_table[ MAX_LABEL ] = { 0 };
size_t      label_count = 0;
rev_label_t rev_label_table[ MAX_LABEL ] = { 0 };
size_t      rev_label_count = 0;

word_t label_num( vm_cstr_t label )
{
    for ( size_t i = 0; i < label_count; i++ )
    {
        if ( cstr_eq( label_table[i].label, label ) )
        {
            return (word_t)
            {
                .as_int = label_table[i].index,
            };
        }
    }
    fprintf( stderr, "[ERRO]: label not found: %.*s\n", (int) label.size, label.str );
    free( file );
    exit(1);
}

size_t rev_label_present( vm_cstr_t label )
{
    for ( size_t i = 0; i < rev_label_count; i++ )
    {
        if ( cstr_eq( rev_label_table[i].label, label ) )
        {
            return i;
        }
    }
    return rev_label_count < MAX_LABEL - 1 ? rev_label_count++ : 
            ( ( fprintf( stderr, "[ERRO]: label overflow\n" ), exit(1), 0 ) );
}

bool label_valid( vm_cstr_t label )
{
    for ( size_t i = 0; i < label_count; i++ )
    {
        if ( cstr_eq( label_table[i].label, label ) )
        {
            return false;
        }
    }
    return true;
}

// ffi? remember library name and symbol name? 
typedef struct dll
{
    vm_cstr_t      file_name;
    vm_cstr_t      prefered_name;
    vm_cstr_t      symbol_name[ MAX_SYMBOL ];
    uint32_t    symbol_count;
} dll_t;

dll_t       dlls[ MAX_DLL ] = { 0 };
uint32_t    dll_count = 0;

uint32_t get_dll_index( vm_cstr_t dll )
{
    for ( uint32_t i = 0; i < dll_count; i++ )
    {
        if ( cstr_eq( dlls[i].file_name, dll ) )
        {
            return i;
        }
    }
    return dll_count < MAX_DLL ? dll_count++ : 
            ( fprintf( stderr, "[ERRO]: dll overflow\n" ), exit(1), 0 );
}

uint32_t get_dll_index_name( vm_cstr_t dll )
{
    for ( uint32_t i = 0; i < dll_count; i++ )
    {
        if ( cstr_eq( dlls[i].prefered_name, dll ) )
        {
            return i;
        }
    }
    fprintf( stderr, "[ERRO]: #%.*s# is not imported\n", (int) dll.size, dll.str );
    exit(1);
}

uint32_t get_symbol_index( uint32_t dll_index, vm_cstr_t symbol )
{
    for ( uint32_t i = 0; i < dlls[ dll_index ].symbol_count; i++ )
    {
        if ( cstr_eq( dlls[ dll_index ].symbol_name[i], symbol ) )
        {
            return i;
        }
    }
    return dlls[ dll_index ].symbol_count < MAX_SYMBOL ? dlls[ dll_index ].symbol_count++ : 
            ( fprintf( stderr, "[ERRO]: symbol overflow\n" ), exit(1), 0 );
}


vm_inst_t   inst[ MAX_INST ] = { 0 };
uint32_t    inst_count = 0;
bool CRLF = false;

void translate_line( vm_cstr_t line )
{
    line = cstr_ltrim( line );
    vm_cstr_t inst_name = cstr_str( &line, ' ' );

    // if comment
    if ( inst_name.str[0] == ';' )
    {
        return;
    }

    int ins = cstr_to_inst( inst_name );
    if ( ins == TOTAL_INST_COUNT )
    {
        // not an instruction, might be a label?
        size_t size = inst_name.size;
        vm_cstr_t label = cstr_str( &inst_name, ':' );
        if ( label.size == size )
        {
            // TODO: read dll
            if ( cstr_eq( label, cstr_create( "import" ) ) )
            {
                vm_cstr_t dll_name = cstr_trim( cstr_str( &line, ' ' ) );
                uint32_t index = get_dll_index( dll_name );
                dlls[ index ].file_name = dll_name;
                vm_cstr_t as = cstr_trim( cstr_str( &line, ' ') );
                if ( !cstr_eq( as, cstr_create( "as" ) ) )
                {
                    fprintf( stderr, "[ERRO]: #%.*s# is not a instruction\n", (int) label.size, label.str );
                    exit(1);
                }
                dlls[ index ].prefered_name = cstr_trim( cstr_str( &line, ' ' ) );
                return;
            }
            fprintf( stderr, "[ERRO]: #%.*s# is not a instruction\n", (int) label.size, label.str );
            exit(1);
        }
        label = cstr_trim( label );
        if ( label_valid( label ) )
        {
            label_table[ label_count++ ] = (label_t)
            {
                .label = label,
                .index = inst_count,
            };
            return;
        }
        else
        {
            fprintf( stderr, "[ERRO]: label #%.*s# already exist\n", (int) label.size, label.str );
            exit(1);
        }
    }
    if ( inst_have_oper( ins ) )
    {
        // need additional opperand information
        vm_cstr_t operand = cstr_trim( cstr_str( &line, ';' ) );
        // printf( "#%.*s %.*s#\n", (int) inst_name.size, inst_name.str, (int) operand.size, operand.str );
        word_t oper = cstr_to_num( operand );
        if ( oper.as_double == vm_get_nan() )
        {
            if ( ins >= INST_JMP && ins <= INST_CALL )
            {
                size_t index = rev_label_present( operand );
                if ( rev_label_table[ index ].count == 0 )
                {
                    rev_label_table[ index ].label = operand;
                }
                rev_label_table[ index ].index[ rev_label_table[ index ].count++ ] = (int) inst_count;
            }
            else if ( ins == INST_NATIVE )
            {
                vm_cstr_t dll = cstr_str( &operand, '.' );
                uint32_t dll_index = get_dll_index_name( dll );
                vm_cstr_t sym = cstr_trim( cstr_str( &operand, ' ' ) );
                uint32_t sym_index = get_symbol_index( dll_index, sym );
                dlls[ dll_index ].symbol_name[ sym_index ] = sym;
                oper.as_int = ( (uint64_t) dll_index << 32LL ) | ( (uint64_t) sym_index );
            }
            else
            {
                fprintf( stderr, "[ERRO]: #%.*s# is not a number\n", (int) operand.size, operand.str );
                free( file );
                exit(1);
            }
        }
        inst[ inst_count < MAX_INST ? inst_count++ : ( fprintf( stderr, "[ERRO]: instruction overflow\n" ), exit(1), 0 ) ] = (vm_inst_t)
        {
            .inst = ins,
            .oper = oper,
        };
    }
    else
    {
        // printf( "#%.*s#\n", (int) inst_name.size, inst_name.str );
        inst[ inst_count < MAX_INST ? inst_count++ : ( fprintf( stderr, "[ERRO]: instruction overflow\n" ), exit(1), 0 ) ] = (vm_inst_t)
        {
            .inst = ins,
        };
    }
}

bool is_crlf( vm_cstr_t src )
{
    for ( size_t i = 0; i < src.size; i++ )
    {
        if ( src.str[i] == '\n' )
        {
            return false;
        }
        else if ( src.str[i] == '\r' )
        {
            if ( i + 1 < src.size && src.str[ i + 1 ] == '\n' )
            {
                return true;
            }
        }
    }
    return false;
}

size_t read_str_to_mem( vm_cstr_t src )
{
    CRLF = is_crlf( src );
    char* code = src.str;
    size_t size = src.size;
    while( size > 0 )
    {
        char* end;
        if ( CRLF )
        {
            end = memchr( code, '\r', size );
        }
        else
        {
            end = memchr( code, '\n', size );
        }
        size_t n = end != NULL ? (size_t) ( end - code ) : size;
        // printf( "%-3zu#%.*s#\n", n, (int) n, code );
        vm_cstr_t line = cstr_trim( (vm_cstr_t) { .str = code, .size = n } );

        code = end;
        size -= n; 
        if ( code != NULL )
        {
            if ( CRLF )
            {
                code += 2;
                size -= 2;
            }
            else
            {
                code += 1;
                size -= 1;
            }
        }


        if ( line.size == 0 )
        {
            continue;
        }
        translate_line( line );
    }
    return 0;
}


void save_dll_sym_to_file( FILE* fp )
{
    fwrite( &dll_count, sizeof (uint32_t), 1, fp );
    for ( uint32_t i = 0; i < dll_count; i++ )
    {
        fwrite( &dlls[i].file_name.size, sizeof (uint32_t), 1, fp );
        fprintf( fp, "%.*s", 
                (int) dlls[i].file_name.size, 
                dlls[i].file_name.str );
        fwrite( &dlls[i].symbol_count, sizeof (uint32_t), 1, fp );
        for ( uint32_t j = 0; j < dlls[i].symbol_count; j++ )
        {
            fwrite( &dlls[i].symbol_name[j].size, sizeof (uint32_t), 1, fp );
            fprintf( fp, "%.*s", 
                    (int) dlls[i].symbol_name[j].size, 
                    dlls[i].symbol_name[j].str );
        }
    }
}


int main( int argc, char** argv )
{
    if ( argc < 2 )
    {
        fprintf( stderr, "Usage: jvavc <source file>\n" );
        exit(0);
    }

    char* target = argv[1];
    FILE* fp = fopen( target, "rb" );
    if ( fp == NULL )
    {
        fprintf( stderr, "error: %s: '%s'\n", strerror( errno ), target );
        fprintf( stderr, "Usage: jvavc <source file>\n" );
        exit(0);
    }

    char* dot_ptr = NULL;
    if ( ( dot_ptr = strchr( target, '.' ) ) == NULL || dot_ptr == target || strcmp( dot_ptr + 1, "jvav" ) )
    {
        fprintf( stderr, "error: file name not accepted: '%s'\n", target );
        fprintf( stderr, "Usage: jvavc <source file>\n" );
        fclose( fp );
        exit(0);
    }

    if ( fseek( fp, 0, SEEK_END ) < 0 )
    {
        fprintf( stderr, "[ERRO]: %s: %s\n", strerror( errno ), target );
        exit(1);
    }
    int size = ftell( fp );
    if ( size < 0 )
    {
        fprintf( stderr, "[ERRO]: %s: %s\n", strerror( errno ), target );
        exit(1);
    }
    if ( fseek( fp, 0, SEEK_SET ) < 0 )
    {
        fprintf( stderr, "[ERRO]: %s: %s\n", strerror( errno ), target );
        exit(1);
    }


    file = malloc( size );
    fread( file, 1, size, fp );
    fclose( fp );

    strcpy( dot_ptr + 1, "clss" );

    vm_cstr_t f_str = { .str = file, .size = size };
    read_str_to_mem( f_str );

    for ( size_t i = 0; i < rev_label_count; i++ )
    {
        word_t oper = label_num( rev_label_table[i].label );
        for ( size_t j = 0; j < rev_label_table[i].count; j++ )
        {
            if ( inst[ rev_label_table[i].index[j] ].oper.as_double == vm_get_nan() )
            {
                inst[ rev_label_table[i].index[j] ].oper = oper;
            }
            else
            {
                fprintf( stderr, "[ERRO]: I fucked up\n" );
            }
        }
    }

    fp = fopen( target, "w+b" );
    uint64_t magic = MAGIC_NUM;
    fwrite( &magic, sizeof (uint64_t), 1, fp );
    fwrite( &inst_count, sizeof( uint32_t), 1, fp );
    fwrite( inst, sizeof (vm_inst_t), inst_count, fp );
    save_dll_sym_to_file( fp );
    free( file );
    fclose( fp );

    return 0;
}

