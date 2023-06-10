#include "jvm.h"


const char* inst_to_str( int inst )
{
    switch ( inst )
    {
        // no operation
        case INST_NOP:      return "INST_NOP";

        // push and pop and swap
        case INST_PUSH:     return "INST_PUSH";
        case INST_POP:      return "INST_POP";
        case INST_DUP:      return "INST_DUP";
        case INST_SWAP:     return "INST_SWAP";

        // inst for i64
        case INST_ADD_I:    return "INST_ADD_I";
        case INST_SUB_I:    return "INST_SUB_I";
        case INST_MUL_I:    return "INST_MUL_I";
        case INST_DIV_I:    return "INST_DIV_I";
        case INST_MOD_I:    return "INST_MOD_I";

        // inst for u64 
        case INST_ADD_U:    return "INST_ADD_U";
        case INST_SUB_U:    return "INST_SUB_U";
        case INST_MUL_U:    return "INST_MUL_U";
        case INST_DIV_U:    return "INST_DIV_U";
        case INST_MOD_U:    return "INST_MOD_U";

        // inst for f64 
        case INST_ADD_F:    return "INST_ADD_F";
        case INST_SUB_F:    return "INST_SUB_F";
        case INST_MUL_F:    return "INST_MUL_F";
        case INST_DIV_F:    return "INST_DIV_F";

        // inst for comparsion
        case INST_CMP_I:    return "INST_CMP_I";
        case INST_CMP_U:    return "INST_CMP_U";
        case INST_CMP_F:    return "INST_CMP_F";

        // inst for condition
        case INST_AND:      return "INST_AND";
        case INST_OR:       return "INST_OR";
        case INST_NOT:      return "INST_NOT";

        // inst for conversion
        case INST_I_TO_U:   return "INST_I_TO_U";
        case INST_I_TO_F:   return "INST_I_TO_F";
        case INST_U_TO_I:   return "INST_U_TO_I";
        case INST_U_TO_F:   return "INST_U_TO_F";
        case INST_F_TO_I:   return "INST_F_TO_I";
        case INST_F_TO_U:   return "INST_F_TO_U";
        
        // inst for jump
        case INST_JMP:      return "INST_JMP";
        case INST_JMP_E:    return "INST_JMP_E";
        case INST_JMP_NE:   return "INST_JMP_NE";
        case INST_JMP_G:    return "INST_JMP_G";
        case INST_JMP_L:    return "INST_JMP_L";
        case INST_JMP_GE:   return "INST_JMP_GE";
        case INST_JMP_LE:   return "INST_JMP_LE";

        // function calls
        case INST_CALL:     return "INST_CALL";
        case INST_RET:      return "INST_RET";

        // native calls
        case INST_NATIVE:   return "INST_NATIVE";

        // HALT
        case INST_HALT:     return "INST_HALT";

        // default case
        case TOTAL_INST_COUNT:
        default:
            fprintf( stderr, "[ERRO]: invalid instruction %d\n", inst );
            exit(1);
    }
}


inst_type cstr_to_inst( cstr_t inst )
{
    // no operation
    if ( cstr_eq( inst, cstr_create( "nop" ) ) )
        return INST_NOP;

    // push and pop and swap
    if ( cstr_eq( inst, cstr_create( "push" ) ) )
        return INST_PUSH;
    if ( cstr_eq( inst, cstr_create( "pop" ) ) )
        return INST_POP;
    if ( cstr_eq( inst, cstr_create( "dup" ) ) )
        return INST_DUP;
    if ( cstr_eq( inst, cstr_create( "swap" ) ) )
        return INST_SWAP;
    
    // inst for i64
    if ( cstr_eq( inst, cstr_create( "addi" ) ) )
        return INST_ADD_I;
    if ( cstr_eq( inst, cstr_create( "subi" ) ) )
        return INST_SUB_I;
    if ( cstr_eq( inst, cstr_create( "muli" ) ) )
        return INST_MUL_I;
    if ( cstr_eq( inst, cstr_create( "divi" ) ) )
        return INST_DIV_I;
    if ( cstr_eq( inst, cstr_create( "modi" ) ) )
        return INST_MOD_I;

    // inst for u64
    if ( cstr_eq( inst, cstr_create( "addu" ) ) )
        return INST_ADD_U;
    if ( cstr_eq( inst, cstr_create( "subu" ) ) )
        return INST_SUB_U;
    if ( cstr_eq( inst, cstr_create( "mulu" ) ) )
        return INST_MUL_U;
    if ( cstr_eq( inst, cstr_create( "divu" ) ) )
        return INST_DIV_U;
    if ( cstr_eq( inst, cstr_create( "modu" ) ) )
        return INST_MOD_U;

    // inst for f64
    if ( cstr_eq( inst, cstr_create( "addf" ) ) )
        return INST_ADD_F;
    if ( cstr_eq( inst, cstr_create( "subf" ) ) )
        return INST_SUB_F;
    if ( cstr_eq( inst, cstr_create( "mulf" ) ) )
        return INST_MUL_F;
    if ( cstr_eq( inst, cstr_create( "divf" ) ) )
        return INST_DIV_F;

    // inst for comparsion
    if ( cstr_eq( inst, cstr_create( "cmpi" ) ) )
        return INST_CMP_I;
    if ( cstr_eq( inst, cstr_create( "cmpu" ) ) )
        return INST_CMP_U;
    if ( cstr_eq( inst, cstr_create( "cmpf" ) ) )
        return INST_CMP_F;

    // inst for condition
    if ( cstr_eq( inst, cstr_create( "and" ) ) )
        return INST_AND;
    if ( cstr_eq( inst, cstr_create( "or" ) ) )
        return INST_OR;
    if ( cstr_eq( inst, cstr_create( "not" ) ) )
        return INST_NOT;

    // inst for conversion
    if ( cstr_eq( inst, cstr_create( "i2u" ) ) )
        return INST_I_TO_U;
    if ( cstr_eq( inst, cstr_create( "i2f" ) ) )
        return INST_I_TO_F;
    if ( cstr_eq( inst, cstr_create( "u2i" ) ) )
        return INST_U_TO_I;
    if ( cstr_eq( inst, cstr_create( "u2f" ) ) )
        return INST_U_TO_F;
    if ( cstr_eq( inst, cstr_create( "f2i" ) ) )
        return INST_F_TO_I;
    if ( cstr_eq( inst, cstr_create( "f2u" ) ) )
        return INST_F_TO_U;

    // inst for jump
    if ( cstr_eq( inst, cstr_create( "jmp" ) ) )
        return INST_JMP;
    if ( cstr_eq( inst, cstr_create( "je" ) ) )
        return INST_JMP_E;
    if ( cstr_eq( inst, cstr_create( "jne" ) ) )
        return INST_JMP_NE;
    if ( cstr_eq( inst, cstr_create( "jg" ) ) )
        return INST_JMP_G;
    if ( cstr_eq( inst, cstr_create( "jl" ) ) )
        return INST_JMP_L;
    if ( cstr_eq( inst, cstr_create( "jge" ) ) )
        return INST_JMP_GE;
    if ( cstr_eq( inst, cstr_create( "jle" ) ) )
        return INST_JMP_LE;

    // function calls
    if ( cstr_eq( inst, cstr_create( "call" ) ) )
        return INST_CALL;
    if ( cstr_eq( inst, cstr_create( "ret" ) ) )
        return INST_RET;

    // native calls
    if ( cstr_eq( inst, cstr_create( "native" ) ) )
        return INST_NATIVE;

    // HALT
    if ( cstr_eq( inst, cstr_create( "halt" ) ) )
        return INST_HALT;
    
    // not a inst
    return TOTAL_INST_COUNT;
}


bool inst_have_oper( int inst )
{
    switch ( inst )
    {
        // no operation
        case INST_NOP:      return false;

        // push and pop and swap
        case INST_PUSH:     return true;
        case INST_POP:      return true;
        case INST_DUP:      return true;
        case INST_SWAP:     return true;

        // inst for i64
        case INST_ADD_I:    return false;
        case INST_SUB_I:    return false;
        case INST_MUL_I:    return false;
        case INST_DIV_I:    return false;
        case INST_MOD_I:    return false;

        // inst for u64 
        case INST_ADD_U:    return false;
        case INST_SUB_U:    return false;
        case INST_MUL_U:    return false;
        case INST_DIV_U:    return false;
        case INST_MOD_U:    return false;

        // inst for f64 
        case INST_ADD_F:    return false;
        case INST_SUB_F:    return false;
        case INST_MUL_F:    return false;
        case INST_DIV_F:    return false;

        // inst for comparsion
        case INST_CMP_I:    return false;
        case INST_CMP_U:    return false;
        case INST_CMP_F:    return false;

        // inst for condition
        case INST_AND:      return false;
        case INST_OR:       return false;
        case INST_NOT:      return false;

        // inst for conversion
        case INST_I_TO_U:   return false;
        case INST_I_TO_F:   return false;
        case INST_U_TO_I:   return false;
        case INST_U_TO_F:   return false;
        case INST_F_TO_I:   return false;
        case INST_F_TO_U:   return false;
        
        // inst for jump
        case INST_JMP:      return true;
        case INST_JMP_E:    return true;
        case INST_JMP_NE:   return true;
        case INST_JMP_G:    return true;
        case INST_JMP_L:    return true;
        case INST_JMP_GE:   return true;
        case INST_JMP_LE:   return true;

        // function calls
        case INST_CALL:     return true;
        case INST_RET:      return false;

        // native calls
        case INST_NATIVE:   return true;

        // HALT
        case INST_HALT:     return false;

        // default case
        case TOTAL_INST_COUNT:
        default:
            fprintf( stderr, "[ERRO]: invalid instruction %d\n", inst );
            exit(1);
    }
}


trap_type inst_exec( vm_t* vm )
{
    if ( vm->ip >= vm->program_size )
    {
        return TRAP_ILLGAL_ACCESS;
    }
    inst_t inst = vm->instructions[ vm->ip ];
    switch ( inst.inst )
    {
        // no operation
        case INST_NOP:
        {
            vm->ip++;
            return TRAP_OK;
        }

        // push and pop and swap
        case INST_PUSH:     
        {
            stack_grow( vm->stack, vm->stack_size + 1 );
            word_t *temp = stack_get( vm->stack, vm->stack_size++ );
            *temp = inst.oper;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_POP: 
        {
            if ( vm->stack_size < inst.oper.as_uint )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            vm->stack_size -= inst.oper.as_uint;
            stack_shrink( vm->stack, vm->stack_size );
            vm->ip++;
            return TRAP_OK;
        }
        case INST_DUP:
        {
            if ( vm->stack_size <= inst.oper.as_uint )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            stack_grow( vm->stack, vm->stack_size + 1 );
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - inst.oper.as_uint - 1 );
            word_t *temp2 = stack_get( vm->stack, vm->stack_size++ );
            *temp2 = *temp1;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_SWAP:  
        {
            if ( vm->stack_size <= inst.oper.as_uint )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - inst.oper.as_uint - 1 );
            word_t  temp3 = *temp1;
            *temp1 = *temp2;
            *temp2 = temp3;
            vm->ip++;
            return TRAP_OK;
        }

        // inst for i64
        case INST_ADD_I:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_int  = temp2->as_int + temp1->as_int;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_SUB_I:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_int  = temp2->as_int - temp1->as_int;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_MUL_I:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_int  = temp2->as_int * temp1->as_int;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_DIV_I:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            if ( temp2->as_int == 0 )
            {
                return TRAP_DEVIDE_BY_ZERO;
            }
            temp2->as_int  = temp2->as_int / temp1->as_int;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_MOD_I:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            if ( temp2->as_int == 0 )
            {
                return TRAP_DEVIDE_BY_ZERO;
            }
            temp2->as_int  = temp2->as_int % temp1->as_int;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        
        // inst for u64
        case INST_ADD_U:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_uint  = temp2->as_uint + temp1->as_uint;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_SUB_U:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_uint  = temp2->as_uint - temp1->as_uint;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_MUL_U:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_uint  = temp2->as_uint * temp1->as_uint;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_DIV_U:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            if ( temp2->as_uint == 0 )
            {
                return TRAP_DEVIDE_BY_ZERO;
            }
            temp2->as_uint  = temp2->as_uint / temp1->as_uint;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_MOD_U:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            if ( temp2->as_uint == 0 )
            {
                return TRAP_DEVIDE_BY_ZERO;
            }
            temp2->as_uint  = temp2->as_uint % temp1->as_uint;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }

        // inst for f64
        case INST_ADD_F:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_double  = temp2->as_double + temp1->as_double;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_SUB_F:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_double  = temp2->as_double - temp1->as_double;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_MUL_F:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_double  = temp2->as_double * temp1->as_double;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_DIV_F:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_double  = temp2->as_double / temp1->as_double;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }

        // inst for comparsion
        case INST_CMP_I:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_int = temp2->as_int - temp1->as_int;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_CMP_U:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_int = (int64_t) temp2->as_uint - (int64_t) temp1->as_uint;
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_CMP_F:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_int = ( temp2->as_double > temp1->as_double ) - ( temp2->as_double < temp1->as_double );
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }

        // inst for condition
        case INST_AND:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_int = ! ( ( !temp1->as_int ) && ( !temp2->as_int ) );
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_OR:
        {
            if ( vm->stack_size < 2 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp1 = stack_get( vm->stack, vm->stack_size - 1 );
            word_t* temp2 = stack_get( vm->stack, vm->stack_size - 2 );
            temp2->as_int = ! ( ( !temp1->as_int ) || ( !temp2->as_int ) );
            vm->stack_size--;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_NOT:
        {
            if ( vm->stack_size < 1 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp = stack_get( vm->stack, vm->stack_size - 1 );
            temp->as_int = !temp->as_int;
            vm->ip++;
            return TRAP_OK;
        }

        // inst for conversion
        case INST_I_TO_U:
        {
            if ( vm->stack_size < 1 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp = stack_get( vm->stack, vm->stack_size - 1 );
            temp->as_uint = (uint64_t) temp->as_int;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_I_TO_F:
        {
            if ( vm->stack_size < 1 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp = stack_get( vm->stack, vm->stack_size - 1 );
            temp->as_double = (double) temp->as_int;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_U_TO_I:
        {
            if ( vm->stack_size < 1 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp = stack_get( vm->stack, vm->stack_size - 1 );
            temp->as_int = (int64_t) temp->as_uint;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_U_TO_F:
        {
            if ( vm->stack_size < 1 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp = stack_get( vm->stack, vm->stack_size - 1 );
            temp->as_double = (double) temp->as_uint;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_F_TO_I:
        {
            if ( vm->stack_size < 1 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp = stack_get( vm->stack, vm->stack_size - 1 );
            temp->as_int = (int64_t) temp->as_double;
            vm->ip++;
            return TRAP_OK;
        }
        case INST_F_TO_U:
        {
            if ( vm->stack_size < 1 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp = stack_get( vm->stack, vm->stack_size - 1 );
            temp->as_uint = (uint64_t) temp->as_double;
            vm->ip++;
            return TRAP_OK;
        }
        
        // inst for jump
        case INST_JMP:
        {
            if ( inst.oper.as_uint > vm->program_size )
            {
                return TRAP_ILLGAL_ACCESS;
            }
            vm->ip = inst.oper.as_uint;
            return TRAP_OK;
        }
        case INST_JMP_E:
        {
            if ( inst.oper.as_uint > vm->program_size )
            {
                return TRAP_ILLGAL_ACCESS;
            }
            word_t* temp = stack_get( vm->stack, --vm->stack_size );
            if ( temp->as_int == 0 )
            {
                vm->ip = inst.oper.as_uint;
            }
            else
            {
                vm->ip++;
            }
            return TRAP_OK;
        }
        case INST_JMP_NE:
        {
            if ( inst.oper.as_uint > vm->program_size )
            {
                return TRAP_ILLGAL_ACCESS;
            }
            word_t* temp = stack_get( vm->stack, --vm->stack_size );
            if ( temp->as_int != 0 )
            {
                vm->ip = inst.oper.as_uint;
            }
            else
            {
                vm->ip++;
            }
            return TRAP_OK;
        }
        case INST_JMP_G:
        {
            if ( inst.oper.as_uint > vm->program_size )
            {
                return TRAP_ILLGAL_ACCESS;
            }
            word_t* temp = stack_get( vm->stack, --vm->stack_size );
            if ( temp->as_int > 0 )
            {
                vm->ip = inst.oper.as_uint;
            }
            else
            {
                vm->ip++;
            }
            return TRAP_OK;
        }
        case INST_JMP_L:
        {
            if ( inst.oper.as_uint > vm->program_size )
            {
                return TRAP_ILLGAL_ACCESS;
            }
            word_t* temp = stack_get( vm->stack, --vm->stack_size );
            if ( temp->as_int < 0 )
            {
                vm->ip = inst.oper.as_uint;
            }
            else
            {
                vm->ip++;
            }
            return TRAP_OK;
        }
        case INST_JMP_GE:
        {
            if ( inst.oper.as_uint > vm->program_size )
            {
                return TRAP_ILLGAL_ACCESS;
            }
            word_t* temp = stack_get( vm->stack, --vm->stack_size );
            if ( temp->as_int >= 0 )
            {
                vm->ip = inst.oper.as_uint;
            }
            else
            {
                vm->ip++;
            }
            return TRAP_OK;
        }
        case INST_JMP_LE:
        {
            if ( inst.oper.as_uint > vm->program_size )
            {
                return TRAP_ILLGAL_ACCESS;
            }
            word_t* temp = stack_get( vm->stack, --vm->stack_size );
            if ( temp->as_int <= 0 )
            {
                vm->ip = inst.oper.as_uint;
            }
            else
            {
                vm->ip++;
            }
            return TRAP_OK;
        }

        // function calls
        case INST_CALL:
        {
            if ( inst.oper.as_uint > vm->program_size )
            {
                return TRAP_ILLGAL_ACCESS;
            }
            word_t* temp = stack_get( vm->stack, vm->stack_size++ );
            temp->as_uint = vm->ip + 1;
            vm->ip = inst.oper.as_uint;
            return TRAP_OK;
        }
        case INST_RET:
        {
            if ( vm->stack_size < 1 )
            {
                return TRAP_STACK_UNDERFLOW;
            }
            word_t* temp = stack_get( vm->stack, --vm->stack_size );
            vm->ip = temp->as_uint;
            return TRAP_OK;
        }

        // native calls
        case INST_NATIVE:
        {
            uint64_t temp = inst.oper.as_uint;
            uint32_t dll = temp >> 32LL;
            uint32_t sym = (uint32_t) temp;
            vm->ip++;
            return vm->libs[ dll ].func[ sym ]( vm );
        }

        // HALT
        case INST_HALT:
        {
            vm->halt = true;
            return TRAP_OK;
        }

        // default case
        case TOTAL_INST_COUNT:
        default:
            fprintf( stderr, "[ERRO]: invalid instruction %d\n", inst.inst );
            exit(1);
    }

}


void vm_print_stack( vm_t* vm )
{
    printf( "[stack]:\n" );
    for ( size_t i = 0; i < vm->stack_size; i++ )
    {
        word_t* temp = stack_get( vm->stack, i );
        printf( "   i64: %" PRId64 "   u64: %" PRIu64 "   f64: %lf   ptr: %p\n",  temp->as_int, temp->as_uint, temp->as_double, temp->as_ptr );
    }
}


void vm_exec_loop( vm_t* vm )
{
    while ( !vm->halt )
    {
        switch ( inst_exec( vm ) )
        {
            case TRAP_OK:
                continue;
            case TRAP_STACK_OVERFLOW:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_STACK_OVERFLOW at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            case TRAP_STACK_UNDERFLOW:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_STACK_UNDERFLOW at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            case TRAP_ILLGAL_ACCESS:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_ILLGAL_ACCESS at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            case TRAP_INVALID_INSTRUCTION:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_INVALID_INSTRUCTION at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            case TRAP_DEVIDE_BY_ZERO:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_DEVIDE_BY_ZERO at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            default:
                fprintf( stderr, "[ERRO]: 'vm': unknown trap information\n" );
                vm_print_stack( vm );
                return;
        }
    }
}


void vm_debug_loop( vm_t* vm )
{
    while ( !vm->halt )
    {
        printf( "inst: %s, oper: %" PRId64 "\n", inst_to_str( vm->instructions[ vm->ip ].inst ), vm->instructions[ vm->ip ].oper.as_int );
        switch ( inst_exec( vm ) )
        {
            case TRAP_OK:
                vm_print_stack( vm );
                getc( stdin );
                continue;
            case TRAP_STACK_OVERFLOW:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_STACK_OVERFLOW at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            case TRAP_STACK_UNDERFLOW:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_STACK_UNDERFLOW at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            case TRAP_ILLGAL_ACCESS:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_ILLGAL_ACCESS at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            case TRAP_INVALID_INSTRUCTION:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_INVALID_INSTRUCTION at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            case TRAP_DEVIDE_BY_ZERO:
                fprintf( stderr, "[ERRO]: 'vm': TRAP_DEVIDE_BY_ZERO at ip %" PRIu32 "\n", vm->ip );
                vm_print_stack( vm );
                return;
            default:
                fprintf( stderr, "[ERRO]: 'vm': unknown trap information\n" );
                vm_print_stack( vm );
                return;
        }
    }
}


trap_type vm_stack_push( vm_t* vm, word_t word )
{
    stack_grow( vm->stack, vm->stack_size + 1 );
    word_t *temp = stack_get( vm->stack, vm->stack_size++ );
    *temp = word;
    return TRAP_OK;
}


trap_type vm_stack_pop( vm_t* vm, word_t* word )
{
    if ( vm->stack_size < 1 )
    {
        return TRAP_STACK_UNDERFLOW;
    }
    *word = *stack_get( vm->stack, --vm->stack_size );
    stack_shrink( vm->stack, vm->stack_size );
    return TRAP_OK;;
}


cstr_t cstr_create( char* str )
{
    return (cstr_t) 
    {
        .str = str,
        .size = strlen( str ),
    };
}


cstr_t cstr_ltrim( cstr_t str )
{
    size_t i = 0;
    while ( i < str.size && isspace( str.str[i] ) )
    {
        i++;
    }
    return ( cstr_t )
    {
        .str = str.str + i,
        .size = str.size - i,
    };
}


cstr_t cstr_rtrim( cstr_t str )
{
    size_t i = 0;
    while ( i < str.size && isspace( str.str[ str.size - i - 1 ] ) )
    {
        i++;
    }
    return ( cstr_t )
    {
        .str = str.str,
        .size = str.size - i,
    };
}


cstr_t cstr_trim( cstr_t str )
{
    return cstr_ltrim( cstr_rtrim( str ) );
}


cstr_t cstr_str( cstr_t* str, char delm )
{
    size_t i = 0;
    while ( i < str->size && str->str[i] != delm )
    {
        i++;
    }
    cstr_t res = 
    {
        .str = str->str,
        .size = i,
    };
    if ( i < str->size )
    {
        str->str += i + 1;
        str->size -= i + 1;
    }
    else
    {
        str->str += i;
        str->size -= i;
    }
    return res;
}


double vm_get_nan( void )
{
    word_t result = { 0 };
    for ( int i = 52; i < 63; i++ )
    {
        result.as_int |= 1LL << i;
    }
    return result.as_double;
}


word_t cstr_to_num( cstr_t str )
{
    word_t num;
    char* end_ptr = NULL;
    num.as_uint = strtoull( str.str, &end_ptr, 10 );
    
    if ( end_ptr - str.str != (int) str.size )
    {
        num.as_double = strtod( str.str, &end_ptr );
        if ( end_ptr - str.str != (int) str.size )
        {
            // fprintf( stderr, "[ERRO]: %.*s is not a valid number\n", (int) str.size, str.str );
            num.as_double = vm_get_nan();
        }
    }
    return num;
}


bool cstr_eq( cstr_t str1, cstr_t str2 )
{
    if ( str1.size != str2.size )
    {
        return false;
    }
    if ( str1.str[0] != str2.str[0] )
    {
        return false;
    }
    if ( strncmp( str1.str + 1, str2.str + 1, str1.size - 1 ) != 0 )
    {
        return false;
    }
    return true;
}



