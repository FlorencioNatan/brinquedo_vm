#ifndef EXT_H
#define EXT_H
#include <stdint.h>
#include "bvm.h"

#define EXT_ARGS 0
#define EXT_STDINOUT 1
#define EXT_FILE_SYSTEM 2
#define EXT_GUI 3

#define MASK_ID 0XFFFFFFFF
#define MASK_OPERACAO 0XFFFFFFFF00000000
#define MASK_PRINT 0X80000000
#define MASK_STRING 0X40000000
#define MASK_DOUBLE 0X20000000
#define MASK_HEX 0X10000000
#define MASK_SINAL 0X08000000
#define MASK_TAMANHO_INT 0X0000000F

int processar_extensao(uint64_t extensao, uint64_t inicio, uint64_t tamanho, bvm *vm);

#endif /* EXT_H */
