#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ext.h"
#include "bvm.h"
#include "../lib/raylib/raylib.h"

int extensao_stdinout_print(int operacao, uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (((operacao & MASK_STRING) >> 30) == 1) {
        if (tamanho < 1) {
            return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
        }

        for (uint64_t i = 0; i < tamanho ; i++) {
            printf("%c", vm->memoria[inicio + i]);
        }

        return EXEC_SUCESSO;
    }

    if (((operacao & MASK_CSTRING) >> 26) == 1) {
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
    char formato[6] = "%016lx";
    uint64_t valor = 0;

    if ((uint64_t) tamanhoInt > tamanho) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    if (tamanhoInt == 8) {
        for (uint64_t i = 64; i >= 8;i -= 8) {
            valor |= ((uint64_t) vm->memoria[inicio++]) << (64 - i);
        }
    } else if (tamanhoInt == 4) {
        for (uint64_t i = 64; i >= 32;i -= 8) {
            valor |= ((uint64_t) vm->memoria[inicio++]) << (64 - i);
        }
        strcpy(formato, "%08lx");
    } else if (tamanhoInt == 2) {
        for (uint64_t i = 64; i >= 48;i -= 8) {
            valor |= ((uint64_t) vm->memoria[inicio++]) << (64 - i);
        }
        strcpy(formato, "%04lx");
    } else {
        valor = vm->memoria[inicio];
        strcpy(formato, "%02lx");
    }

    if (((operacao & MASK_HEX) >> 28) != 1 && ((operacao & MASK_SINAL) >> 27) != 1) {
        strcpy(formato, "%lu");
    } else if (((operacao & MASK_SINAL) >> 27) == 1) {
        strcpy(formato, "%ld");
    }

    printf(formato, valor);

    return EXEC_SUCESSO;
}

int extensao_stdinout_scan(int operacao, uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (((operacao & MASK_STRING) >> 30) == 1) {
        if (tamanho < 1) {
            return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
        }

        char string[256];
        scanf("%255[^\n]s", string);

        if (strlen(string) >= tamanho) {
            printf("String lida maior que a memória disponível.\n");
        }

        for (uint64_t i = 0; i < tamanho ; i++) {
            vm->memoria[inicio + i] = string[i];
        }

        return EXEC_SUCESSO;
    }

    if (((operacao & MASK_DOUBLE) >> 29) == 1) {
        if (tamanho < 8) {
            return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
        }

        double valor = 0;
        scanf("%lf", &valor);
        uint8_t *bytesValor = (uint8_t*)&valor;

        for(uint64_t i = 0; i < sizeof(uint64_t); i++) {
            vm->memoria[inicio+i] = bytesValor[i];
        }

        return EXEC_SUCESSO;
    }

    int tamanhoInt = (operacao & MASK_TAMANHO_INT);
    char formato[6] = "%lx";
    uint64_t valor = 0;
    uint8_t *bytesValor = (uint8_t*)&valor;

    if (((operacao & MASK_HEX) >> 28) != 1 && ((operacao & MASK_SINAL) >> 27) != 1) {
        strcpy(formato, "%lu");
    } else if (((operacao & MASK_SINAL) >> 27) == 1) {
        strcpy(formato, "%ld");
    }

    scanf(formato, &valor);

    if ((uint64_t) tamanhoInt > tamanho) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    if (tamanhoInt == 8) {
        for(uint64_t i = 0; i < sizeof(uint64_t); i++) {
            vm->memoria[inicio+i] = bytesValor[i];
        }
    } else if (tamanhoInt == 4) {
        for(uint64_t i = 0; i < sizeof(uint32_t); i++) {
            vm->memoria[inicio+i] = bytesValor[i];
        }
    } else if (tamanhoInt == 2) {
        for(uint64_t i = 0; i < sizeof(uint16_t); i++) {
            vm->memoria[inicio+i] = bytesValor[i];
        }
    } else {
        vm->memoria[inicio] = *bytesValor;
    }

    return EXEC_SUCESSO;
}

int extensao_stdinout(int operacao, uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (((operacao & MASK_PRINT) >> 31) == 1) {
        return extensao_stdinout_print(operacao, inicio, tamanho, vm);
    }
    return extensao_stdinout_scan(operacao, inicio, tamanho, vm);
}

int extensao_gui_init_window(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    int largura = 600;
    int altura = 400;
    char *titulo = "BVM!";

    if (tamanho >= 8) {
        largura = le_int_da_memoria(vm, inicio);
        inicio+=4;

        altura = le_int_da_memoria(vm, inicio);
        inicio+=4;
    }

    if (tamanho >= 8) {
        titulo = le_string_da_memoria(vm, inicio, tamanho - 8);
    }

    InitWindow(largura, altura, titulo);
    SetTargetFPS(60);
    return EXEC_SUCESSO;
}

int extensao_gui_window_should_close(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (tamanho < 1) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    bool deveFechar = WindowShouldClose();
    vm->memoria[inicio] = deveFechar;
    return EXEC_SUCESSO;
}

int extensao_gui_close_window() {
    return EXEC_SUCESSO;
}

int extensao_gui_clear_background(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (tamanho < 4) {
        ClearBackground(RAYWHITE);
        return EXEC_SUCESSO;
    }

    int vermelho = vm->memoria[inicio+3];
    int verde    = vm->memoria[inicio+2];
    int azul     = vm->memoria[inicio+1];
    int alfa     = vm->memoria[inicio];
    ClearBackground((Color){vermelho, verde, azul, alfa});
    return EXEC_SUCESSO;
}

int extensao_gui_begin_drawing() {
    BeginDrawing();
    return EXEC_SUCESSO;
}

int extensao_gui_end_drawing() {
    EndDrawing();
    return EXEC_SUCESSO;
}

int extensao_gui_draw_pixel(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (tamanho < 12) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    int posX = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int vermelho = vm->memoria[inicio+3];
    int verde    = vm->memoria[inicio+2];
    int azul     = vm->memoria[inicio+1];
    int alfa     = vm->memoria[inicio];
    DrawPixel(posX, posY, (Color){vermelho, verde, azul, alfa});
    return EXEC_SUCESSO;
}

int extensao_gui_draw_line(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (tamanho < 28) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    int posX1 = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY1 = le_int_da_memoria(vm, inicio);
    inicio+=4;

    int posX2 = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY2 = le_int_da_memoria(vm, inicio);
    inicio+=4;

    double espessura = le_double_da_memoria(vm, inicio);
    inicio+=8;

    int vermelho = vm->memoria[inicio+3];
    int verde    = vm->memoria[inicio+2];
    int azul     = vm->memoria[inicio+1];
    int alfa     = vm->memoria[inicio];

    DrawLineEx(
        (Vector2){ posX1, posY1 },
        (Vector2){ posX2, posY2 },
        espessura,
        (Color){vermelho, verde, azul, alfa}
    );
    return EXEC_SUCESSO;
}

int extensao_gui_draw_triangle(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (tamanho < 28) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    int posX1 = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY1 = le_int_da_memoria(vm, inicio);
    inicio+=4;

    int posX2 = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY2 = le_int_da_memoria(vm, inicio);
    inicio+=4;

    int posX3 = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY3 = le_int_da_memoria(vm, inicio);
    inicio+=4;

    int vermelho = vm->memoria[inicio+3];
    int verde    = vm->memoria[inicio+2];
    int azul     = vm->memoria[inicio+1];
    int alfa     = vm->memoria[inicio];

    DrawTriangle(
        (Vector2){ posX1, posY1 },
        (Vector2){ posX2, posY2 },
        (Vector2){ posX3, posY3 },
        (Color){vermelho, verde, azul, alfa}
    );
    return EXEC_SUCESSO;
}

int extensao_gui_draw_reactangle(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (tamanho < 20) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    int posX = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY = le_int_da_memoria(vm, inicio);
    inicio+=4;

    int largura = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int altura = le_int_da_memoria(vm, inicio);
    inicio+=4;

    int vermelho = vm->memoria[inicio+3];
    int verde    = vm->memoria[inicio+2];
    int azul     = vm->memoria[inicio+1];
    int alfa     = vm->memoria[inicio];

    DrawRectangle(
        posX, posY,
        largura, altura,
        (Color){vermelho, verde, azul, alfa}
    );
    return EXEC_SUCESSO;
}

int extensao_gui_draw_ellipse(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (tamanho < 28) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    int posX = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY = le_int_da_memoria(vm, inicio);
    inicio+=4;

    double raioHorizontal = le_double_da_memoria(vm, inicio);
    inicio+=8;
    double raioVertical = le_double_da_memoria(vm, inicio);
    inicio+=8;

    int vermelho = vm->memoria[inicio+3];
    int verde    = vm->memoria[inicio+2];
    int azul     = vm->memoria[inicio+1];
    int alfa     = vm->memoria[inicio];

    DrawEllipse(
        posX, posY,
        raioHorizontal, raioVertical,
        (Color){vermelho, verde, azul, alfa}
    );
    return EXEC_SUCESSO;
}

int extensao_gui_draw_ellipse_lines(uint64_t inicio, uint64_t tamanho, bvm *vm) {
    if (tamanho < 28) {
        return EXEC_ERRO_TAMANHO_MEMORIA_PEQUENO_PARA_EXTENSAO;
    }

    int posX = le_int_da_memoria(vm, inicio);
    inicio+=4;
    int posY = le_int_da_memoria(vm, inicio);
    inicio+=4;

    double raioHorizontal = le_double_da_memoria(vm, inicio);
    inicio+=8;
    double raioVertical = le_double_da_memoria(vm, inicio);
    inicio+=8;

    int vermelho = vm->memoria[inicio+3];
    int verde    = vm->memoria[inicio+2];
    int azul     = vm->memoria[inicio+1];
    int alfa     = vm->memoria[inicio];

    DrawEllipseLines(
        posX, posY,
        raioHorizontal, raioVertical,
        (Color){vermelho, verde, azul, alfa}
    );
    return EXEC_SUCESSO;
}

int extensao_gui(int operacao, uint64_t inicio, uint64_t tamanho, bvm *vm) {
    switch (operacao) {
        case INIT_WINDOW:
            return extensao_gui_init_window(inicio, tamanho, vm);
            break;
        case WINDOW_SHOULD_CLOSE:
            return extensao_gui_window_should_close(inicio, tamanho, vm);
            break;
        case CLOSE_WINDOW:
            return extensao_gui_close_window();
            break;
        case CLEAR_BACKGROUND:
            return extensao_gui_clear_background(inicio, tamanho, vm);
            break;
        case BEGIN_DRAWING:
            return extensao_gui_begin_drawing();
            break;
        case END_DRAWING:
            return extensao_gui_end_drawing(inicio, tamanho, vm);
            break;
        case DRAW_PIXEL:
            return extensao_gui_draw_pixel(inicio, tamanho, vm);
            break;
        case DRAW_LINE:
            return extensao_gui_draw_line(inicio, tamanho, vm);
            break;
        case DRAW_TRIANGLE:
            return extensao_gui_draw_triangle(inicio, tamanho, vm);
            break;
        case DRAW_RECTANGLE:
            return extensao_gui_draw_reactangle(inicio, tamanho, vm);
            break;
        case DRAW_ELLIPSE:
            return extensao_gui_draw_ellipse(inicio, tamanho, vm);
            break;
        case DRAW_ELLIPSE_LINES:
            return extensao_gui_draw_ellipse_lines(inicio, tamanho, vm);
            break;
    }
    return EXEC_SUCESSO;
}

int processar_extensao(uint64_t extensao, uint64_t inicio, uint64_t tamanho, bvm *vm) {
    int id = (extensao & MASK_ID);
    int operacao = (extensao & MASK_OPERACAO) >> 32;

    switch (id) {
        case EXT_STDINOUT:
            return extensao_stdinout(operacao, inicio, tamanho, vm);
            break;
        case EXT_GUI:
            return extensao_gui(operacao, inicio, tamanho, vm);
            break;
        default:
            return EXEC_ERRO_EXTENSAO_NAO_EXISTE;
            break;
    }

    return EXEC_SUCESSO;
}
