#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "imagem.h"

// Tabelas de codificação L e R para os dígitos
const char *L_code[] = {
    "0001101", "0011001", "0010011", "0111101", "0100011",
    "0110001", "0101111", "0111011", "0110111", "0001011"
};

const char *R_code[] = {
    "1110010", "1100110", "1101100", "1000010", "1011100",
    "1001110", "1010000", "1000100", "1001000", "1110100"
};

// Função para validar o código EAN-8
int validarEAN8(const char *codigo) {
    if (strlen(codigo) != 8) return 0; // O código deve ter 8 dígitos

    // Verificar se todos os caracteres são dígitos
    for (int i = 0; i < 8; i++) {
        if (!isdigit(codigo[i])) {
            return 0; // Código inválido, contém caracteres não numéricos
        }
    }

    // Calcular o dígito verificador
    int soma = 0;
    for (int i = 0; i < 7; i++) {
        if (i % 2 == 0) {
            soma += (codigo[i] - '0') * 3;
        } else {
            soma += (codigo[i] - '0');
        }
    }

    int digitoVerificador = (10 - (soma % 10)) % 10;

    // Verificar se o dígito verificador está correto
    return (codigo[7] - '0') == digitoVerificador;
}

void desenharBarra(ImagemPBM *imagem, int *pos, const char *codigoBinario, int espessura, int margemSuperior, int margemInferior) {
    for (int i = 0; i < strlen(codigoBinario); i++) {
        for (int e = 0; e < espessura; e++) { // Replicação horizontal
            for (int k = margemSuperior; k < imagem->altura - margemInferior; k++) {
                imagem->pixels[k][*pos] = (codigoBinario[i] == '1') ? PIXEL_PRETO : PIXEL_BRANCO;
            }
            (*pos)++;
        }
    }
}

// Função para gerar o código de barras EAN-8
void gerarCodigoDeBarras(const char *codigo, ImagemPBM *imagem, int espessura) {
    int larguraTotal = 0;

    // Calcular a largura total ajustada pela espessura
    for (int i = 0; i < 4; i++) {
        larguraTotal += strlen(L_code[codigo[i] - '0']) * espessura;
    }
    for (int i = 4; i < 8; i++) {
        larguraTotal += strlen(R_code[codigo[i] - '0']) * espessura;
    }
    larguraTotal += 2 * strlen("101") * espessura + strlen("01010") * espessura;

    int margemEsquerda = (imagem->largura - larguraTotal) / 2;
    int margemDireita = imagem->largura - larguraTotal - margemEsquerda;
    int margemSuperior = (imagem->altura - 100) / 2;
    int margemInferior = imagem->altura - 100 - margemSuperior;

    // Preencher a imagem com zeros
    for (int i = 0; i < imagem->altura; i++) {
        for (int j = 0; j < imagem->largura; j++) {
            imagem->pixels[i][j] = PIXEL_BRANCO;
        }
    }

    int pos = margemEsquerda;

    // Marcador inicial
    desenharBarra(imagem, &pos, "101", espessura, margemSuperior, margemInferior);

    // Codificar os 4 primeiros dígitos
    for (int i = 0; i < 4; i++) {
        desenharBarra(imagem, &pos, L_code[codigo[i] - '0'], espessura, margemSuperior, margemInferior);
    }

    // Marcador central
    desenharBarra(imagem, &pos, "01010", espessura, margemSuperior, margemInferior);

    // Codificar os 4 últimos dígitos
    for (int i = 4; i < 8; i++) {
        desenharBarra(imagem, &pos, R_code[codigo[i] - '0'], espessura, margemSuperior, margemInferior);
    }

    // Marcador final
    desenharBarra(imagem, &pos, "101", espessura, margemSuperior, margemInferior);
}

// Função para lidar com os argumentos da linha de comando
void processarArgumentos(int argc, char *argv[], char *codigo, int *largura, int *altura, char *nomeArquivo, int *espessura) {
    if (argc < 2) {
        printf("Uso: %s <codigo-ean-8> [--largura <valor>] [--altura <valor (>= 100)>] [--arquivo <nome>] [--espessura <valor>]\n", argv[0]);
        exit(1);
    }

    // Validar o código EAN-8
    strcpy(codigo, argv[1]);
    if (!validarEAN8(codigo)) {
        printf("Código EAN-8 inválido.\n");
        exit(1);
    }

    // Valores padrão
    *largura = 200;
    *altura = 100;
    *espessura = 1; // Padrão: 1 pixel por bit
    strcpy(nomeArquivo, "codigo_barras.pbm");

    // Processar os argumentos opcionais
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--largura") == 0 && i + 1 < argc) {
            *largura = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--altura") == 0 && i + 1 < argc) {
            *altura = atoi(argv[++i]);
            if (*altura < 100) {
                printf("Altura mínima é 100. Ajustando para 100.\n");
                *altura = 100;
            }
        } else if (strcmp(argv[i], "--arquivo") == 0 && i + 1 < argc) {
            strcpy(nomeArquivo, argv[++i]);
        } else if (strcmp(argv[i], "--espessura") == 0 && i + 1 < argc) {
            *espessura = atoi(argv[++i]);
            if (*espessura < 1) {
                printf("Espessura mínima é 1. Ajustando para 1.\n");
                *espessura = 1;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    char codigo[9];
    int largura, altura, espessura;
    char nomeArquivo[100];

    processarArgumentos(argc, argv, codigo, &largura, &altura, nomeArquivo, &espessura);

    // Criar a imagem
    ImagemPBM *imagem = criarImagem(largura, altura);
    if (!imagem) {
        printf("Erro ao criar a imagem.\n");
        return 1;
    }

    // Gerar o código de barras
    gerarCodigoDeBarras(codigo, imagem, espessura);

    // Salvar a imagem PBM
    if (!salvarImagemPBM(nomeArquivo, imagem)) {
        printf("Erro ao salvar a imagem.\n");
        liberarImagem(imagem);
        return 1;
    }

    printf("Imagem gerada com sucesso!\n");

    liberarImagem(imagem);
    return 0;
}


