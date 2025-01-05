#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagem.h"

#define PIXEL_BRANCO 0
#define PIXEL_PRETO 1

// Tabelas de codificação L e R para os dígitos

char* L_code[] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};
char* R_code[] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

//Função para impressão da matriz (utilizada somente para os testes do programa)
void imprimirMatriz(ImagemPBM *imagem) {
    for (int i = 0; i < imagem->altura; i++) {
        for (int j = 0; j < imagem->largura; j++) {
            printf("%d ", imagem->pixels[i][j]);
        }
        printf("\n");
    }
}

//Função para remoção das margens da imagem do codigo de barras
void removerMargens(char *codigo) {
    int i = 0;
    int j = strlen(codigo) - 1;
    while (codigo[i] == '0') i++;
    while (codigo[j] == '0') j--;
    memmove(codigo, &codigo[i], j - i + 1);
    codigo[j - i + 1] = '\0';
}

//Função para normalizar o código. Caso a espessura das barras sejam
//maiores que 1, formata para facilitar na leitura.

char* normalizarCodigo(char *codigo) {
    int max_size = 1000;
    char *novo_vetor = (char *)malloc(max_size * sizeof(char));

    if (!novo_vetor) {
        printf("Erro na alocação de memória.\n");
        return NULL;
    }

    int count = 0, i = 0, start_counting = 0, first_sequence_length = 0;

    while (codigo[i] != '\0') {
        if (codigo[i] == '1') {
            if (!start_counting) start_counting = 1;
            count++;
        } else if (start_counting && codigo[i] == '0') {
            first_sequence_length = count;
            break;
        }
        i++;
    }

    if (first_sequence_length == 0) {
        printf("Nenhuma sequência de 1s encontrada.\n");
        free(novo_vetor);
        return NULL;
    }

    int novo_vetor_index = 0;

    for (i = 0; codigo[i] != '\0'; i += first_sequence_length) {
        novo_vetor[novo_vetor_index++] = codigo[i];
    }

    novo_vetor[novo_vetor_index] = '\0';
    return novo_vetor;
}

//Converte o vetor retirado da matriz para uma string para facilitar na hora
//da leitura

void converterLinhaParaString(int *linha, int largura, char *codigo) {
    for (int i = 0; i < largura; i++) {
        codigo[i] = linha[i] == PIXEL_PRETO ? '1' : '0';
    }
    codigo[largura] = '\0';
}

//Função para converter a sequência binária para dígitos

void converterBarraParaDigitos(char *codigo, int *digitos) {
    char digito[8];
    int j = 0;
    for (int i = 3; i < 31; i += 7) {
        strncpy(digito, &codigo[i], 7);
        digito[7] = '\0';
        for (int k = 0; k < 10; k++) {
            if (strcmp(digito, L_code[k]) == 0) {
                digitos[j++] = k;
                break;
            }
        }
    }
    for (int i = 36; i < 64; i += 7) {
        strncpy(digito, &codigo[i], 7);
        digito[7] = '\0';
        for (int k = 0; k < 10; k++) {
            if (strcmp(digito, R_code[k]) == 0) {
                digitos[j++] = k;
                break;
            }
        }
    }
    printf("\n");
}

//Função para obter o dígito verificador após a conversão para dígitos

int obterDigitoVerificador(int *digitos) {
    int somaImpares = digitos[0] + digitos[2] + digitos[4] + digitos[6];
    int somaPares = digitos[1] + digitos[3] + digitos[5];
    int somaTotal = somaImpares * 3 + somaPares;
    int digitoVerificadorCalculado = 10 - (somaTotal % 10);

    if (digitoVerificadorCalculado == 10) {
        digitoVerificadorCalculado = 0;
    }

    return digitoVerificadorCalculado;
}

//Aplicação das funções de tratamento e conversão

void processarCodigoBarras(ImagemPBM *imagem) {
    int linhaDoMeio = imagem->altura / 2;
    char codigo[imagem->largura + 1];
    int digitos[8] = {0};

    converterLinhaParaString(imagem->pixels[linhaDoMeio], imagem->largura, codigo);

    removerMargens(codigo);

    char *codigoNormalizado = normalizarCodigo(codigo);
    if (!codigoNormalizado) {
        printf("Erro na normalização do código.\n");
        return;
    }

    converterBarraParaDigitos(codigoNormalizado, digitos);
    free(codigoNormalizado);

    if (strlen(codigo) < 67) {
        printf("Erro: Codigo de barras invalido ou malformado.\n");
        return;
    }

    int digitoVerificadorCalculado = obterDigitoVerificador(digitos);
    printf("Digito verificador calculado: %d\n", digitoVerificadorCalculado);
    printf("Digito verificador do codigo: %d\n", digitos[7]);

    if (digitoVerificadorCalculado == digitos[7]) {
        printf("O codigo de barras EAN-8 eh valido.\n");
    } else {
        printf("O codigo de barras EAN-8 eh invalido.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo PBM>\n", argv[0]);
        return 1;
    }

    ImagemPBM *imagem = carregarImagemPBM(argv[1]);
    if (!imagem) {
        return 1;
    }

    processarCodigoBarras(imagem);

    liberarImagem(imagem);
    return 0;
}

