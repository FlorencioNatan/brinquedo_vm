#ifndef TABELA_JUMP_H
#define TABELA_JUMP_H

#include <stdint.h>
#include <stdio.h>

typedef struct jump {
    char* label;
    uint64_t posicao;
} jump;

jump* montar_tabela_jumps(char *assembly, int totalJumps);
jump* lookup_jump(jump *tabelaJumps, int totalJumps, char* label);

#endif /* TABELA_JUMP_H */
