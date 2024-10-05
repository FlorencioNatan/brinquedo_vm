#include "instrucoes.h"
#include <string.h>

registroInstrucao instrucoes[QUANTIDADE_INSTRUCOES] = {
    {INST_CMD_NOP , INST_NOP ,  false,  1},
    {INST_CMD_HALT, INST_HALT,  false,  1},
    {INST_CMD_PUSH, INST_PUSH,  true,   9},
    {INST_CMD_POP , INST_POP ,  false,  1},
    {INST_CMD_ADDU, INST_ADDU,  false,  1},
    {INST_CMD_SUBU, INST_SUBU,  false,  1},
    {INST_CMD_MULU, INST_MULU,  false,  1},
    {INST_CMD_DIVU, INST_DIVU,  false,  1},
    {INST_CMD_REMU, INST_REMU,  false,  1},
    {INST_CMD_ADD , INST_ADD ,  false,  1},
    {INST_CMD_SUB , INST_SUB ,  false,  1},
    {INST_CMD_MUL , INST_MUL ,  false,  1},
    {INST_CMD_DIV , INST_DIV ,  false,  1},
    {INST_CMD_REM , INST_REM ,  false,  1},
    {INST_CMD_ADDF, INST_ADDF,  false,  1},
    {INST_CMD_SUBF, INST_SUBF,  false,  1},
    {INST_CMD_MULF, INST_MULF,  false,  1},
    {INST_CMD_DIVF, INST_DIVF,  false,  1},

    {INST_CMD_SL  , INST_SL  ,  false,  1},
    {INST_CMD_SR  , INST_SR  ,  false,  1},
    {INST_CMD_AND , INST_AND ,  false,  1},
    {INST_CMD_OR  , INST_OR  ,  false,  1},
    {INST_CMD_XOR , INST_XOR ,  false,  1},
    {INST_CMD_NOR , INST_NOR ,  false,  1},

    {INST_CMD_BEQ , INST_BEQ ,  false,  1},
    {INST_CMD_BNE , INST_BNE ,  false,  1},
    {INST_CMD_BLTZ, INST_BLTZ,  false,  1},
    {INST_CMD_BLEZ, INST_BLEZ,  false,  1},
    {INST_CMD_BGTZ, INST_BGTZ,  false,  1},
    {INST_CMD_BGEZ, INST_BGEZ,  false,  1},
    {INST_CMD_CALL, INST_CALL,  false,  1},
    {INST_CMD_JUMP, INST_JUMP,  false,  1},
    {INST_CMD_SW,   INST_SW,    false,  1},
    {INST_CMD_SH,   INST_SH,    false,  1},
    {INST_CMD_SQ,   INST_SQ,    false,  1},
    {INST_CMD_SB,   INST_SB,    false,  1},
    {INST_CMD_LW,   INST_LW,    false,  1},
    {INST_CMD_LH,   INST_LH,    false,  1},
    {INST_CMD_LQ,   INST_LQ,    false,  1},
    {INST_CMD_LB,   INST_LB,    false,  1},

    {INST_CMD_BEQI , INST_BEQ , true,  10},
    {INST_CMD_BNEI , INST_BNE , true,  10},
    {INST_CMD_BLTZI, INST_BLTZ, true,  10},
    {INST_CMD_BLEZI, INST_BLEZ, true,  10},
    {INST_CMD_BGTZI, INST_BGTZ, true,  10},
    {INST_CMD_BGEZI, INST_BGEZ, true,  10},
    {INST_CMD_CALLI, INST_CALL, true,  10},
    {INST_CMD_JUMPI, INST_JUMP, true,  10},
    {INST_CMD_SWI,   INST_SW,   true,  10},
    {INST_CMD_SHI,   INST_SH,   true,  10},
    {INST_CMD_SQI,   INST_SQ,   true,  10},
    {INST_CMD_SBI,   INST_SB,   true,  10},
    {INST_CMD_LWI,   INST_LW,   true,  10},
    {INST_CMD_LHI,   INST_LH,   true,  10},
    {INST_CMD_LQI,   INST_LQ,   true,  10},
    {INST_CMD_LBI,   INST_LB,   true,  10},
    {INST_CMD_EXT , INST_EXT ,  false,  1}
};

registroInstrucao lookup_instrucao(char* instrucao) {
    for (int i = 0; i < QUANTIDADE_INSTRUCOES; i++) {
        if (strcmp(instrucoes[i].cmd, instrucao) == 0) {
            return instrucoes[i];
        }
    }

    return instrucoes[0];
}
