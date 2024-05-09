#ifndef INSTRUCOES_H
#define INSTRUCOES_H

#include <stdint.h>

#define QUANTIDADE_INSTRUCOES 26

#define INST_NOP  0x00
#define INST_HALT 0x01
#define INST_PUSH 0x02
#define INST_POP  0x03

#define INST_ADDU 0x04
#define INST_SUBU 0x05
#define INST_MULU 0x06
#define INST_DIVU 0x07
#define INST_REMU 0x08

#define INST_ADD  0x09
#define INST_SUB  0x0A
#define INST_MUL  0x0B
#define INST_DIV  0x0C
#define INST_REM  0x0D

#define INST_ADDF 0x0E
#define INST_SUBF 0x0F
#define INST_MULF 0x10
#define INST_DIVF 0x11

#define INST_BEQ  0x12
#define INST_BNE  0x13
#define INST_BLTZ 0x14
#define INST_BLEZ 0x15
#define INST_BGTZ 0x16
#define INST_BGEZ 0x17
#define INST_CALL 0x18
#define INST_JUMP 0x19

#define INST_CMD_NOP  "nop"
#define INST_CMD_HALT "halt"
#define INST_CMD_PUSH "push"
#define INST_CMD_POP  "pop"

#define INST_CMD_ADDU "addu"
#define INST_CMD_SUBU "subu"
#define INST_CMD_MULU "mulu"
#define INST_CMD_DIVU "divu"
#define INST_CMD_REMU "remu"

#define INST_CMD_ADD  "add"
#define INST_CMD_SUB  "sub"
#define INST_CMD_MUL  "mul"
#define INST_CMD_DIV  "div"
#define INST_CMD_REM  "rem"

#define INST_CMD_ADDF "addf"
#define INST_CMD_SUBF "subf"
#define INST_CMD_MULF "mulf"
#define INST_CMD_DIVF "divf"

#define INST_CMD_BEQ  "beq"
#define INST_CMD_BNE  "bne"
#define INST_CMD_BLTZ "bltz"
#define INST_CMD_BLEZ "blez"
#define INST_CMD_BGTZ "bgtz"
#define INST_CMD_BGEZ "bgez"
#define INST_CMD_CALL "call"
#define INST_CMD_JUMP "jump"

typedef struct registroInstrucao {
    char* cmd;
    uint8_t codigo;
    uint8_t tamanho;
} registroInstrucao;

registroInstrucao lookup_instrucao(char* instrucao);

#endif /* INSTRUCOES_H */
