#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "inst.h"
    
typedef uint8_t byte_t;
typedef union {
    uint64_t as_u64;
    int64_t as_i64;
    double as_f64;
} word_t;

typedef struct {
    byte_t type;
    word_t op;
} inst_t;

#define MAX_STACK_SIZE 1024
word_t stack[MAX_STACK_SIZE] = {0};
uint64_t stack_size = 0;

void stack_push(word_t value)
{
    if (stack_size > MAX_STACK_SIZE) {
	fprintf(stderr, "ERROR: Stack overflow\n");
	exit(1);
    }
    stack[stack_size++] = value;
}

word_t stack_pop(void)
{
    if (stack_size == 0) {
	fprintf(stderr, "ERROR: Stack underflow\n");
	exit(1);
    }
    return stack[--stack_size];
}

void stack_set(uint64_t addr, word_t value)
{
    if (addr >= stack_size) {
	fprintf(stderr, "ERROR: Illegal stack address\n");
	exit(1);
    }
    stack[addr] = value;
}

#define MAX_PROGRAM_SIZE 1024
inst_t program[MAX_PROGRAM_SIZE] = {0};
uint64_t program_size = 0;
uint64_t inst_ptr = 0;

void stack_dump()
{
    printf("Stack:\n");
    if (stack_size == 0)
	printf("  [empty]\n");
    for (uint64_t i = 0; i < stack_size; i++)
	printf("  %ld\n", stack[i].as_u64);
    printf("------------------------------\n");
}

void add_inst(inst_t inst)
{
    assert(program_size < MAX_PROGRAM_SIZE);
    program[program_size++] = inst;
}

int main(void)
{
    add_inst(MAKE_PUSH_I64(0));
    add_inst(MAKE_PUSH_I64(1));
    add_inst(MAKE_DUP(1));
    add_inst(MAKE_DUP(1));
    add_inst(MAKE_IADD);
    add_inst(MAKE_DUP(0));
    add_inst(MAKE_PRINT_INT);
    add_inst(MAKE_DUP(0));
    add_inst(MAKE_PUSH_I64(100));
    add_inst(MAKE_IGT);
    add_inst(MAKE_JNZ(12));
    add_inst(MAKE_JMP(2));
    add_inst(MAKE_HALT);

    int is_halt = 0;
    while (!is_halt) {
	if (inst_ptr > program_size) {
	    fprintf(stderr, "ERROR: Illegal instruction address: %lu\n", inst_ptr);
	    exit(1);
	}

	byte_t inst_type = program[inst_ptr].type;
	word_t op = program[inst_ptr].op;
	switch ((inst_type_t)inst_type) {
	case INST_NOP:
	    inst_ptr++;
	    break;
	    
	case INST_PUSH:
	    stack_push(op);
	    inst_ptr++;
	    break;

	case INST_GET:
	    stack_push(stack[op.as_u64]);
	    inst_ptr++;
	    break;

	case INST_DUP: {
	    int64_t addr = stack_size - op.as_u64 - 1;
	    if (addr < 0) {
		fprintf(stderr, "ERROR: illegal stack address on instruction `dup`\n");
		exit(1);
	    }
	    stack_push(stack[addr]);
	    inst_ptr++;
	} break;

	case INST_SET:
	    stack_set(op.as_u64, stack_pop());
	    inst_ptr++;
	    break;
	    
	case INST_IADD: {
	    if (stack_size < 2) {
		fprintf(stderr, "ERROR: instruction `add` requires 2 arguments on the stack\n");
		exit(1);
	    }
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a + b });
	    inst_ptr++;
	} break;
	    
	case INST_ISUB: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a - b });
	    inst_ptr++;
	} break;
	    
	case INST_IMUL: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a * b });
	    inst_ptr++;
	} break;
	      
	case INST_IDIV: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a / b });
	    inst_ptr++;
	} break;
	    
	case INST_IEQ: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a == b });
	    inst_ptr++;
	} break;
	    
	case INST_IGT: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a > b });
	    inst_ptr++;
	} break;
	    
	case INST_IGE: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a >= b });
	    inst_ptr++;
	} break;
	    
	case INST_ILT: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a < b });
	    inst_ptr++;
	} break;
	    
	case INST_ILE: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a <= b });
	    inst_ptr++;
	} break;
	    
	case INST_JMP:
	    inst_ptr = op.as_u64;
	    break;
	    
	case INST_JNZ:
	    if (stack_pop().as_u64)
		inst_ptr = op.as_u64;
	    else
		inst_ptr++;
	    break;
	    
	case INST_HALT:
	    is_halt = 1;
	    break;

	case INST_PRINT_INT:
	    printf("%ld\n", stack_pop().as_i64);
	    inst_ptr++;
	    break;
	    
	default:
	    assert(0 && "unreachable");
	}
    }
    
    return 0;
}
