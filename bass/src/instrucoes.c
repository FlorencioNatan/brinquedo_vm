#include "instrucoes.h"
#include <string.h>

registroInstrucao instrucoes[QUANTIDADE_INSTRUCOES] = {
    {INST_CMD_NOP , INST_NOP ,  1},
    {INST_CMD_HALT, INST_HALT,  1},
    {INST_CMD_PUSH, INST_PUSH,  9},
    {INST_CMD_POP , INST_POP ,  1},
    {INST_CMD_ADDU, INST_ADDU,  1},
    {INST_CMD_SUBU, INST_SUBU,  1},
    {INST_CMD_MULU, INST_MULU,  1},
    {INST_CMD_DIVU, INST_DIVU,  1},
    {INST_CMD_REMU, INST_REMU,  1},
    {INST_CMD_ADD , INST_ADD ,  1},
    {INST_CMD_SUB , INST_SUB ,  1},
    {INST_CMD_MUL , INST_MUL ,  1},
    {INST_CMD_DIV , INST_DIV ,  1},
    {INST_CMD_REM , INST_REM ,  1},
    {INST_CMD_ADDF, INST_ADDF,  1},
    {INST_CMD_SUBF, INST_SUBF,  1},
    {INST_CMD_MULF, INST_MULF,  1},
    {INST_CMD_DIVF, INST_DIVF,  1},
    {INST_CMD_BEQ , INST_BEQ , 10},
    {INST_CMD_BNE , INST_BNE , 10},
    {INST_CMD_BLTZ, INST_BLTZ, 10},
    {INST_CMD_BLEZ, INST_BLEZ, 10},
    {INST_CMD_BGTZ, INST_BGTZ, 10},
    {INST_CMD_BGEZ, INST_BGEZ, 10},
    {INST_CMD_CALL, INST_CALL, 10},
    {INST_CMD_JUMP, INST_JUMP, 10},
    {INST_CMD_SW,   INST_SW,   10},
    {INST_CMD_SH,   INST_SH,   10},
    {INST_CMD_SQ,   INST_SQ,   10},
    {INST_CMD_SB,   INST_SB,   10},
    {INST_CMD_LW,   INST_LW,   10},
    {INST_CMD_LH,   INST_LH,   10},
    {INST_CMD_LQ,   INST_LQ,   10},
    {INST_CMD_LB,   INST_LB,   10}
};

registroInstrucao lookup_instrucao(char* instrucao) {
    for (int i = 0; i < QUANTIDADE_INSTRUCOES; i++) {
        if (strcmp(instrucoes[i].cmd, instrucao) == 0) {
            return instrucoes[i];
        }
    }

    return instrucoes[0];
}
