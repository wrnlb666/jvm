# Cross platform vm
* for Windows user, download: [libdl](https://github.com/dlfcn-win32/dlfcn-win32).


## no operation
* INST_NOP
    * `nop`
    * no operation. 

## push and pop and swap
* INST_PUSH
    * `push`
    * push a `i64`/`u64`/`f64` to the stack, effectively increase stack size by one. 
* INST_POP
    * `pop`
    * remove specified amount of variable from the stack starting from the top of the stack. 
* INST_DUP
    * `dup`
    * duplicate the variable relative to the top of the stack to the stack top, effectively increase stack size by one. 
* INST_SWAP
    * `swap`
    * swap the current top of the stack to the variable relative to the top of the stack, does not change stack size. 

## inst for i64
* INST_ADD_I
    * `addi`
    * add the top of the stack to the second top of the stack as integers, effectively decrease stack size by one. 
* INST_SUB_I
    * `subi`
    * subtract the top of the stack from the second top of the stack as integers, effectively decrease stack size by one. 
* INST_MUL_I
    * `muli`
    * multiply the top of the stack to the second top of the stack as integers, effectively decrease stack size by one. 
* INST_DIV_I
    * `divi`
    * divide the top of the stack from the second top of the stack as integers, effectively decrease stack size by one. 
* INST_MOD_I
    * `modi`
    * modulo the top of the stack from the second top of the stack as integers, effectively decrease stack size by one. 

## inst for u64
* INST_ADD_U
    * `addu`
* INST_SUB_U
    * `subu`
* INST_MUL_U
    * `mulu`
* INST_DIV_U
    * `divu`

## inst for f64
* INST_ADD_F
    * `addf`
* INST_SUB_F
    * `subf`
* INST_MUL_F
    * `mulf`
* INST_DIV_F
    * `divf`

## inst for comparsion
* INST_CMP_I
    * `cmpi`
* INST_CMP_U
    * `cmpu`
* INST_CMP_F
    * `cmpf`

## inst for condition
* INST_AND
    * `and`
* INST_OR
    * `or`
* INST_NOT
    * `not`

## inst for conversion
* INST_I_TO_U
    * `i2u`
* INST_I_TO_F
    * `i2f`
* INST_U_TO_I
    * `u2i`
* INST_U_TO_F
    * `u2f`
* INST_F_TO_I
    * `f2i`
* INST_F_TO_U
    * `f2u`

## inst for jump
* INST_JMP
    * `jmp`
* INST_JMP_E
    * `je`
* INST_JMP_NE
    * `jne`
* INST_JMP_G
    * `jg`
* INST_JMP_L
    * `jl`
* INST_JMP_GE
    * `jge`
* INST_JMP_LE
    * `jle`

## function calls
* INST_CALL
    * `call`
* INST_RET
    * `ret`

## HALT
* INST_HALT
    * `halt`