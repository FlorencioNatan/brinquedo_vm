#include "bvm.h"
#include "instrucoes.h"

#define X_7_0 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define X_6_0 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
#define X_5_0 0x00, 0x00, 0x00, 0x00, 0x00
#define X_4_0 0x00, 0x00, 0x00, 0x00
#define TESTE_PILHA 0x1F, 0x00, 0xFF, 0xEE, 0xDD, 0x20, 0xCC, 0x88

int main() {
    bvm vm = {
        .pc = 0,
        .pilha = {0},
        .tam_pilha = 0,
        .programa = {
            INST_PUSH, TESTE_PILHA,
            INST_POP,
            INST_PUSH, X_7_0, 0xF4,
            INST_PUSH, X_7_0, 0x24,
            INST_REMU,
            INST_HALT
        },
        .tam_programa = 3,
        .memoria = {0},
        .halt = 0
    };

    while (!vm.halt || vm.pc >= sizeof(vm.programa)) {
        processar_instrucoes(&vm);
    }
}
