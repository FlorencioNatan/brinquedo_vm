#include <stdio.h>
#include <stdlib.h>
#include "ext.h"
#include "bvm.h"

int extensao_stdinout_print(int operacao, uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (((operacao & MASK_STRING) >> 30) == 1) {
        if (tamanho < 1) {
            return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
        }

        char* string = malloc(tamanho);
        for (uint64_t i = 0; i < tamanho ; i++) {
            string[i] = vm->memoria[inicio + i];
        }

        printf("%s", string);

        return EXEC_SUCESSO;
    }

    if (((operacao & MASK_DOUBLE) >> 29) == 1) {
        if (tamanho < 8) {
            return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
        }

        uint64_t valorInt = 0;
        for (uint64_t i = 64; i >= 8;i -= 8) {
            valorInt |= ((uint64_t) vm->memoria[inicio++]) << (64 - i);
        }

        double* valor = (double *) &valorInt;
        printf("%f", *valor);

        return EXEC_SUCESSO;
    }

    int tamanhoInt = (operacao & MASK_TAMANHO_INT);
    uint64_t valor = 0;

    if (tamanhoInt == 8) {
        for (uint64_t i = 64; i >= 8;i -= 8) {
            valor |= ((uint64_t) vm->memoria[inicio++]) << (64 - i);
        }
    } else if (tamanhoInt == 4) {
        for (uint64_t i = 32; i >= 8;i -= 8) {
            valor |= ((uint64_t) vm->memoria[inicio++]) << (64 - i);
        }
    } else if (tamanhoInt == 2) {
        for (uint64_t i = 16; i >= 8;i -= 8) {
            valor |= ((uint64_t) vm->memoria[inicio++]) << (64 - i);
        }
    } else {
        valor = vm->memoria[inicio];
    }

    printf("%ld", valor);

    return EXEC_SUCESSO;
}

int extensao_stdinout(int operacao, uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (((operacao & MASK_PRINT) >> 31) == 1) {
        return extensao_stdinout_print(operacao, inicio, tamanho, vm);
    }
    // return extensao_stdinout_scan(operacao, inicio, fim, *vm);
    return EXEC_SUCESSO;
}

int processar_extensao(uint64_t extensao, uint64_t inicio, uint64_t tamanho, bvm *vm) {
    int id = (extensao & MASK_ID);
    int operacao = (extensao & MASK_OPERACAO) >> 32;

    switch (id) {
        case EXT_STDINOUT:
            return extensao_stdinout(operacao, inicio, tamanho, vm);
            break;
        default:
            return EXEC_ERRO_EXTENSAO_NAO_EXISTE;
            break;
    }

    return EXEC_SUCESSO;
}
