#ifndef INST_H
#define INST_H

typedef enum {
    INST_NOP,
    INST_PUSH,
    INST_GET, // absolute address
    INST_DUP, // relative address
    INST_SET,
    INST_IADD,
    INST_ISUB,
    INST_IMUL,
    INST_IDIV,
    INST_IEQ,
    INST_IGT,
    INST_IGE,
    INST_ILT,
    INST_ILE,
    INST_JMP,
    INST_JNZ,
    INST_ALLOC,
    INST_FREE,
    INST_HALT,
    INST_PRINT_INT // temporary for debug purposes
} inst_type_t;

const char *inst_type_to_cstr(inst_type_t type)
{
    switch (type) {
    case INST_NOP: return "INST_NOP";
    case INST_PUSH: return "INST_PUSH";
    case INST_GET: return "INST_GET";
    case INST_DUP: return "INST_DUP";
    case INST_SET: return "INST_SET";
    case INST_IADD: return "INST_IADD";
    case INST_ISUB: return "INST_ISUB";
    case INST_IMUL: return "INST_IMUL";
    case INST_IDIV: return "INST_IDIV";
    case INST_IEQ: return "INST_IEQ";
    case INST_IGT: return "INST_IGT";
    case INST_IGE: return "INST_IGE";
    case INST_ILT: return "INST_ILT";
    case INST_ILE: return "INST_ILE";
    case INST_JMP: return "INST_JMP";
    case INST_JNZ: return "INST_JNZ";
    case INST_ALLOC: return "INST_ALLOC";
    case INST_FREE: return "INST_FREE";
    case INST_HALT: return "INST_HALT";
    case INST_PRINT_INT: return "INST_PRINT_INT";
    default: assert(0 && "unreachable");
    }
}

#define MAKE_NOP (inst_t) { .inst_type = INST_NOP }
#define MAKE_PUSH_I64(opr) (inst_t) { .type = INST_PUSH, .op = { .as_i64 = (opr) } }
#define MAKE_GET(opr) (inst_t) { .type = INST_GET, .op = { .as_u64 = (opr) } }
#define MAKE_DUP(opr) (inst_t) { .type = INST_DUP, .op = { .as_u64 = (opr) } }
#define MAKE_SET(opr) (inst_t) { .type = INST_SET, .op = { .as_u64 = (opr) } }
#define MAKE_IADD (inst_t) { .type = INST_IADD }
#define MAKE_ISUB (inst_t) { .type = INST_ISUB }
#define MAKE_IMUL (inst_t) { .type = INST_IMUL }
#define MAKE_IDIV (inst_t) { .type = INST_IDIV }
#define MAKE_IEQ (inst_t) { .type = INST_IEQ }
#define MAKE_IGT (inst_t) { .type = INST_IGT }
#define MAKE_IGE (inst_t) { .type = INST_IGE }
#define MAKE_ILT (inst_t) { .type = INST_ILT }
#define MAKE_ILE (inst_t) { .type = INST_ILE }
#define MAKE_JMP(opr) (inst_t) { .type = INST_JMP, .op = { .as_u64 = (opr) } }
#define MAKE_JNZ(opr) (inst_t) { .type = INST_JNZ, .op = { .as_u64 = (opr) } }
#define MAKE_ALLOC(opr) (inst_t) { .type = INST_ALLOC, .op = { .as_u64 = (opr) } }
#define MAKE_FREE (inst_t) { .type = INST_FREE }
#define MAKE_HALT (inst_t) { .type = INST_HALT }
#define MAKE_PRINT_INT (inst_t) { .type = INST_PRINT_INT }

#endif // INST_H
