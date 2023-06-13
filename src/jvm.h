#ifndef __VM_H__
#define __VM_H__

#define _GNU_SOURCE

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <inttypes.h>

#include "stack.h"

#define MAX_INST        4096
#define MAGIC_NUM       114514LL

#define MAX_DLL         16
#define MAX_SYMBOL      32


/*
typedef union word
{
    uint64_t    as_uint;
    int64_t     as_int;
    double      as_double;
    void*       as_ptr;
} word_t;
*/

typedef struct vm_cstr
{
    char*       str;
    uint32_t    size;
} vm_cstr_t;

typedef enum vm_inst_type
{
    // no operation
    INST_NOP,

    // push and pop and swap
    INST_PUSH,
    INST_POP,
    INST_DUP,
    INST_SWAP,

    // inst for i64
    INST_ADD_I,
    INST_SUB_I,
    INST_MUL_I,
    INST_DIV_I,
    INST_MOD_I,

    // inst for u64
    INST_ADD_U,
    INST_SUB_U,
    INST_MUL_U,
    INST_DIV_U,
    INST_MOD_U,

    // inst for f64
    INST_ADD_F,
    INST_SUB_F,
    INST_MUL_F,
    INST_DIV_F,

    // inst for comparsion
    INST_CMP_I,
    INST_CMP_U,
    INST_CMP_F,

    // inst for condition
    INST_AND,
    INST_OR,
    INST_NOT,

    // inst for conversion
    INST_I_TO_U,
    INST_I_TO_F,
    INST_U_TO_I,
    INST_U_TO_F,
    INST_F_TO_I,
    INST_F_TO_U,

    // inst for jump
    INST_JMP,
    INST_JMP_E,
    INST_JMP_NE,
    INST_JMP_G,
    INST_JMP_L,
    INST_JMP_GE,
    INST_JMP_LE, 

    // function calls
    INST_CALL,
    INST_RET,

    // native calls
    INST_NATIVE,

    // HALT
    INST_HALT,

    // count
    TOTAL_INST_COUNT,
} vm_inst_type;

typedef struct vm_inst
{
    vm_inst_type    inst;
    word_t          oper;
} vm_inst_t;

typedef enum trap
{
    TRAP_OK,
    TRAP_STACK_OVERFLOW,
    TRAP_STACK_UNDERFLOW,
    TRAP_ILLGAL_ACCESS,
    TRAP_INVALID_INSTRUCTION,
    TRAP_DEVIDE_BY_ZERO,
} trap_type;

typedef struct vm vm_t;

typedef trap_type (*FFI)( vm_t* vm );

typedef struct lib
{
    vm_cstr_t   lib_name;
    void*       handle;
    FFI         func[ MAX_SYMBOL ];
    uint32_t    symbol_count;
} lib_t;

struct vm
{
    // halt
    bool            halt;
    pthread_mutex_t pause;

    // stack
    vm_stack_t*     stack;
    size_t          stack_size;

    // code
    vm_inst_t*      instructions;
    uint32_t        program_size;
    uint32_t        ip;

    // FFI native functions
    lib_t           libs[ MAX_DLL ];
    uint32_t        lib_count;

};






const char*     inst_to_str( int inst );
vm_inst_type    cstr_to_inst( vm_cstr_t inst );
bool            inst_have_oper( int inst );
trap_type       inst_exec( vm_t* vm );
void            vm_print_stack( vm_t* vm );
void            vm_exec_loop( vm_t* vm );
void            vm_debug_loop( vm_t* vm );
vm_cstr_t       cstr_create( char* str );
vm_cstr_t       cstr_ltrim( vm_cstr_t str );
vm_cstr_t       cstr_rtrim( vm_cstr_t str );
vm_cstr_t       cstr_trim( vm_cstr_t str );
vm_cstr_t       cstr_str( vm_cstr_t* str, char delm );
double          vm_get_nan( void );
word_t          cstr_to_num( vm_cstr_t str );
bool            cstr_eq( vm_cstr_t str1, vm_cstr_t str2 );

// functions for FFI interface. User should use these two functions for push and pop
trap_type       vm_stack_push( vm_t* vm, word_t word );
trap_type       vm_stack_pop( vm_t* vm, word_t* word );




#endif  // __VM_H__