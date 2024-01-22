#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
    
typedef int8_t byte_t;
typedef int64_t word_t;

typedef enum { INST_NOP, INST_PUSH, INST_ADD, INST_HALT } inst_type_t;

typedef struct {
    byte_t inst_type;
    word_t op;
} inst_t;

#define MAX_STACK_SIZE 1024
word_t stack[MAX_STACK_SIZE] = {0};
word_t stack_size = 0;

void push(word_t value)
{
    if (stack_size > MAX_STACK_SIZE) {
	fprintf(stderr, "ERROR: Stack overflow\n");
	exit(1);
    }
    stack[stack_size++] = value;
}

word_t pop()
{
    if (stack_size == 0) {
	fprintf(stderr, "ERROR: Stack underflow\n");
	exit(1);
    }
    return stack[--stack_size];
}

#define MAX_PROGRAM_SIZE 1024
inst_t program[MAX_PROGRAM_SIZE] = {0};
word_t inst_ptr = 0;

void stack_dump()
{
    printf("Stack:\n");
    if (stack_size == 0)
	printf("  [empty]\n");
    for (word_t i = 0; i < stack_size; i++)
	printf("  %ld\n", stack[i]);
    printf("------------------------------\n");
}

int main(void)
{
    {
	size_t iota = 0;
	program[iota++] = (inst_t) { .inst_type = INST_PUSH, .op = 5 };
	program[iota++] = (inst_t) { .inst_type = INST_PUSH, .op = 7 };
	program[iota++] = (inst_t) { .inst_type = INST_ADD };
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
	    push(op);
	    inst_ptr++;
	    break;
	case INST_ADD:
	    if (stack_size < 2) {
		fprintf(stderr, "ERROR: instruction `add` requires 2 arguments on the stack\n");
		exit(1);
	    }
	    word_t b = pop();
	    word_t a = pop();
	    push(a + b);
	    inst_ptr++;
	    break;
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
