// para a implementação das funções declaradas em imagem.h 

#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"

// Cria uma nova imagem PBM com todos os pixels inicializados em 0
ImagemPBM *criarImagem(int largura, int altura) {
    ImagemPBM *imagem = malloc(sizeof(ImagemPBM));
    imagem->largura = largura;
    imagem->altura = altura;

    // Aloca a matriz de pixels
    imagem->pixels = malloc(altura * sizeof(int *));
    for (int i = 0; i < altura; i++) {
        imagem->pixels[i] = calloc(largura, sizeof(int));
    }

    return imagem;
}

// Libera a memória de uma imagem PBM
void liberarImagem(ImagemPBM *imagem) {
    for (int i = 0; i < imagem->altura; i++) {
        free(imagem->pixels[i]);
    }
    free(imagem->pixels);
    free(imagem);
}

// Salva uma imagem PBM em um arquivo
bool salvarImagemPBM(const char *nomeArquivo, const ImagemPBM *imagem) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        return false;
    }

    fprintf(arquivo, "P1\n");
    fprintf(arquivo, "%d %d\n", imagem->largura, imagem->altura);

    for (int i = 0; i < imagem->altura; i++) {
        for (int j = 0; j < imagem->largura; j++) {
            fprintf(arquivo, "%d ", imagem->pixels[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    return true;
}

// Carrega uma imagem PBM de um arquivo
ImagemPBM *carregarImagemPBM(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        return NULL;
    }

    char formato[3];
    fscanf(arquivo, "%s", formato);
    if (strcmp(formato, "P1") != 0) {
        fclose(arquivo);
        return NULL;
    }

    int largura, altura;
    fscanf(arquivo, "%d %d", &largura, &altura);

    ImagemPBM *imagem = criarImagem(largura, altura);

    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            fscanf(arquivo, "%d", &imagem->pixels[i][j]);
        }
    }

    fclose(arquivo);
    return imagem;
}

