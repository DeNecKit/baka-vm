#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
    
typedef uint8_t byte_t;
typedef union {
    uint64_t as_u64;
    int64_t as_i64;
    double as_f64;
} word_t;

typedef enum {
    INST_NOP,
    INST_PUSH,
    
    INST_IADD,
    INST_ISUB,
    INST_IMUL,
    INST_IDIV,

    INST_JMP,
    INST_JNZ,
    
    INST_HALT
} inst_type_t;

typedef struct {
    byte_t inst_type;
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

#define MAKE_NOP (inst_t) { .inst_type = INST_NOP }
#define MAKE_PUSH_I64(opr) (inst_t) { .inst_type = INST_PUSH, .op = { .as_i64 = (opr) } }
#define MAKE_IADD (inst_t) { .inst_type = INST_IADD }
#define MAKE_ISUB (inst_t) { .inst_type = INST_ISUB }
#define MAKE_IMUL (inst_t) { .inst_type = INST_IMUL }
#define MAKE_IDIV (inst_t) { .inst_type = INST_IDIV }
#define MAKE_JMP(opr) (inst_t) { .inst_type = INST_JMP, .op = { .as_u64 = (opr) } }
#define MAKE_JNZ(opr) (inst_t) { .inst_type = INST_JNZ, .op = { .as_u64 = (opr) } }
#define MAKE_HALT (inst_t) { .inst_type = INST_HALT }

int main(void)
{
    add_inst(MAKE_PUSH_I64(5));
    add_inst(MAKE_PUSH_I64(7));
    add_inst(MAKE_IADD);
    add_inst(MAKE_HALT);
    
    int is_halt = 0;
    while (!is_halt) {
	if (inst_ptr > program_size) {
	    fprintf(stderr, "Illegal instruction address\n");
	    exit(1);
	}
	
	byte_t inst_type = program[inst_ptr].inst_type;
	word_t op = program[inst_ptr].op;
	switch ((inst_type_t)inst_type) {
	case INST_NOP:
	    inst_ptr++;
	    break;
	    
	case INST_PUSH:
	    stack_push(op);
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
	    
	case INST_JMP:
	case INST_JNZ:
	    assert(0 && "TODO: not implemented");
	    
	case INST_HALT:
	    is_halt = 1;
	    break;
	    
	default:
	    assert(0 && "unreachable");
	}
    }

    stack_dump();
    
    return 0;
}
