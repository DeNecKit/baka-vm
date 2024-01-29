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

int main(void)
{
    {
	size_t iota = 0;
	program[iota++] = (inst_t) { .inst_type = INST_PUSH, .op = { .as_i64 = 5 } };
	program[iota++] = (inst_t) { .inst_type = INST_PUSH, .op = { .as_i64 = 7 } };
	program[iota++] = (inst_t) { .inst_type = INST_IADD };
	program[iota++] = (inst_t) { .inst_type = INST_HALT };
    }
    
    int is_halt = 0;
    while (!is_halt) {
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
	    break;
	}
	case INST_ISUB: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a - b });
	    inst_ptr++;
	    break;
	}
	case INST_IMUL: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a * b });
	    inst_ptr++;
	    break;
	}
	case INST_IDIV: {
	    int64_t b = stack_pop().as_i64;
	    int64_t a = stack_pop().as_i64;
	    stack_push((word_t) { .as_i64 = a / b });
	    inst_ptr++;
	    break;
	}
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
