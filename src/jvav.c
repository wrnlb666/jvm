#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <pthread.h>

// ldl for win32: https://github.com/dlfcn-win32/dlfcn-win32

#include "stack.h"
#include "jvm.h"


#if defined __GNUC__ && defined __has_builtin
    #if __has_builtin(__builtin_alloca)
    #define _malloc( size ) size < 1024 ? _alloca( size ) : malloc( size )
    #define _free( ptr, size ) size < 1024 ? : free( ptr )
    #endif  // _alloca
#endif  // __GNUC__


void load_dll( vm_t* vm, FILE* fp  )
{
    char* error;
    fread( &vm->lib_count, sizeof (uint32_t), 1, fp );
    for ( uint32_t i = 0; i < vm->lib_count; i++ )
    {
        fread( &vm->libs[i].lib_name.size, sizeof (uint32_t), 1, fp );
        #ifdef _malloca
        vm->libs[i].lib_name.str = _malloca( sizeof (char) * ( vm->libs[i].lib_name.size + 1 ) );
        #elif defined __GNUC__ && defined __has_builtin
            #if __has_builtin(__builtin_alloca)
            vm->libs[i].lib_name.str = _malloc( sizeof (char) * ( vm->libs[i].lib_name.size + 1 ) );
            #endif  // _alloca
        #else
        vm->libs[i].lib_name.str = malloc( sizeof (char) * ( vm->libs[i].lib_name.size + 1 ) );
        #endif  // _malloca
        fread( vm->libs[i].lib_name.str, sizeof (char), vm->libs[i].lib_name.size, fp );
        vm->libs[i].lib_name.str[ vm->libs[i].lib_name.size ] = 0;
        void* library = dlopen( vm->libs[i].lib_name.str, RTLD_LAZY );
        if ( library == NULL )
        {
            fprintf( stderr, "[ERRO]: dlopen: %s: %s\n", dlerror(), vm->libs[i].lib_name.str );
            exit(1);
        }
        dlerror();
        vm->libs[i].handle = library;

        fread( &vm->libs[i].symbol_count, sizeof (uint32_t), 1, fp );
        for ( uint32_t j = 0; j < vm->libs[i].symbol_count; j++ )
        {
            vm_cstr_t symbol;
            fread( &symbol.size, sizeof (uint32_t), 1, fp );
            #ifdef _malloca
            symbol.str = _malloca( sizeof (char) * ( symbol.size + 1 ) );
            #elif defined __GNUC__ && defined __has_builtin
                #if __has_builtin(__builtin_alloca)
                symbol.str = _malloc( sizeof (char) * ( symbol.size + 1 ) );
                #endif // _alloca
            #else
            symbol.str = malloc( sizeof (char) * ( symbol.size + 1 ) );
            #endif  // _malloca
            fread( symbol.str, sizeof (char), symbol.size, fp );
            symbol.str[ symbol.size ] = 0;

            *(void**) ( &vm->libs[i].func[j] ) = dlsym( vm->libs[i].handle, symbol.str );
            if ( ( error = dlerror() ) != NULL ) 
            {
                fprintf( stderr, "[ERRO]: dlsym: %s\n", error );
                exit(1);
            }

            #ifdef _malloca
            _freea( symbol.str );
            #elif defined __GNUC__ && defined __has_builtin
                #if __has_builtin(__builtin_alloca)
                _free( symbol.str, sizeof (char) * symbol.size + 1 );
                #endif  // _alloca
            #else
            free( symbol.str );
            #endif  // _malloca
        }
        #ifdef _malloca
        _freea( vm->libs[i].lib_name.str );
        #elif defined __GNUC__ && defined __has_builtin
            #if __has_builtin(__builtin_alloca)
            _free( vm->libs[i].lib_name.str, sizeof (char) * ( vm->libs[i].lib_name.size + 1 ) );
            #endif  // _alloca
        #else
        free( vm->libs[i].lib_name.str );
        #endif  // _malloc
        vm->libs[i].lib_name.str = NULL;
    }
}

void unload_dll( vm_t* vm )
{
    for ( uint32_t i = 0; i < vm->lib_count; i++ )
    {
        dlclose( vm->libs[i].handle );
    }
}


int main( int argc, char** argv )
{
    if ( argc == 1 )
    {
        fprintf( stderr, "Usage: jvav <mainclass>\n" );
        exit(0);
    }
    bool debug = false;
    int index = 0;
    for ( int i = 1; i < argc; i++ )
    {
        if ( argv[i][0] == '-' && argv[i][1] == 'g' && argv[i][2] == 0 )
        {
            debug = true;
        }
        else if ( argv[i][0] != '-' )
        {
            index = i;
        }
    }

    size_t file_size = strlen( argv[index] );
    char* file_name = malloc( file_size + 6 );
    sprintf( file_name, "%s.clss", argv[index] );

    FILE* fp = fopen( file_name, "rb" );
    if ( fp == NULL )
    {
        fprintf( stderr, "Error: Could not find or load main class %s\n", argv[index] );
        fprintf( stderr, "Caused by: jvav.lang.ClassNotFoundException: %s\n", argv[index] );
        free( file_name );
        exit(0);
    }

    if ( fseek( fp, 0, SEEK_END ) < 0 )
    {
        fprintf( stderr, "[ERRO]: %s: %s\n", strerror( errno ), file_name );
        free( file_name );
        exit(0);
    }
    int size = ftell( fp );
    if ( size < 0 )
    {
        fprintf( stderr, "[ERRO]: %s: %s\n", strerror( errno ), file_name );
        free( file_name );
        exit(0);
    }
    if ( fseek( fp, 0, SEEK_SET ) < 0 )
    {
        fprintf( stderr, "[ERRO]: %s: %s\n", strerror( errno ), file_name );
        free( file_name );
        exit(0);
    }

    uint64_t magic;
    fread( &magic, sizeof (uint64_t), 1, fp );
    if ( magic != MAGIC_NUM )
    {
        fprintf( stderr, "Error: Could not find or load main class %s\n", argv[index] );
        fprintf( stderr, "Caused by: jvav.lang.ClassNotFoundException: %s\n", argv[index] );
        free( file_name );
        exit(0);
    }
    free( file_name );
    size -= sizeof (uint64_t);



    vm_t vm = { 0 };
    fread( &vm.program_size, sizeof (uint32_t), 1, fp );
    vm.instructions = malloc( sizeof (vm_inst_t) * vm.program_size );
    size -= vm.program_size * sizeof (vm_inst_t) + sizeof (uint32_t);
    if ( vm.program_size > MAX_INST )
    {
        fprintf( stderr, "[ERRO]: %d instruction count overflow\n", (int) vm.program_size );
        exit(1);
    }
    fread( vm.instructions, sizeof (vm_inst_t), vm.program_size, fp );
    
    load_dll( &vm, fp );

    fclose( fp );
    
    pthread_mutex_init( &vm.pause, NULL );
    vm.stack = stack_create();

    if ( debug )
    {
        vm_debug_loop( &vm );
    }
    else
    {
        vm_exec_loop( &vm );
    }

    // vm_print_stack( &vm );

    unload_dll( &vm );

    // printf( "reached: %d\n", __LINE__ ); fflush( stdout );
    
    stack_free( vm.stack );
    pthread_mutex_destroy( &vm.pause );
    free( vm.instructions );

    return 0;
}


