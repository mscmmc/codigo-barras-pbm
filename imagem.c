// Para a implementação das funções declaradas em imagem.h 

#include <stdio.h>
#include <stdlib.h>
#include "imagem.h"

// Função para criar uma imagem PBM
ImagemPBM *criarImagem(int largura, int altura) {
    // Aloca memória para a estrutura ImagemPBM
    ImagemPBM *imagem = (ImagemPBM *)malloc(sizeof(ImagemPBM));
    if (!imagem) {
        fprintf(stderr, "Erro: Falha ao alocar memória para a imagem.\n");
        return NULL;
    }
	// define largura e altura a partir da entrada do usuário
    imagem->largura = largura;
    imagem->altura = altura;

    // Aloca memória para a matriz de pixels
    imagem->pixels = (int **)malloc(altura * sizeof(int *));
    if (!imagem->pixels) {
        fprintf(stderr, "Erro: Falha ao alocar memória para os pixels.\n");
        free(imagem);
        return NULL;
    }

    for (int i = 0; i < altura; i++) {
        imagem->pixels[i] = (int *)malloc(largura * sizeof(int));
        if (!imagem->pixels[i]) {
            fprintf(stderr, "Erro: Falha ao alocar memória para a linha de pixels.\n");
            for (int j = 0; j < i; j++) {
                free(imagem->pixels[j]);
            }
            free(imagem->pixels);
            free(imagem);
            return NULL;
        }

        // Inicializa os pixels como brancos
        for (int j = 0; j < largura; j++) {
            imagem->pixels[i][j] = PIXEL_BRANCO;
        }
    }

    return imagem;
}
// Função para liberar a memória de uma imagem PBM
void liberarImagem(ImagemPBM *imagem) {
    for (int i = 0; i < imagem->altura; i++) {
        free(imagem->pixels[i]);
    }
    free(imagem->pixels);
    free(imagem);
}

// Função para salvar uma imagem PBM em um arquivo
bool salvarImagemPBM(const char *nomeArquivo, const ImagemPBM *imagem) {
    if (!imagem) {
        fprintf(stderr, "Erro: Imagem inválida.\n");
        return false;
    }

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s para escrita.\n", nomeArquivo);
        return false;
    }

    // Escreve o cabeçalho do arquivo PBM
    fprintf(arquivo, "P1\n");
    fprintf(arquivo, "%d %d\n", imagem->largura, imagem->altura);

    // Escreve os pixels da imagem
    for (int i = 0; i < imagem->altura; i++) {
        for (int j = 0; j < imagem->largura; j++) {
            fprintf(arquivo, "%d ", imagem->pixels[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    return true;
}

// Função para carregar uma imagem PBM de um arquivo
ImagemPBM *carregarImagemPBM(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s para leitura.\n", nomeArquivo);
        return NULL;
    }

    char formato[3];
    if (!fscanf(arquivo, "%2s", formato) || formato[0] != 'P' || formato[1] != '1') {
        fprintf(stderr, "Erro: Formato de arquivo inválido.\n");
        fclose(arquivo);
        return NULL;
    }

    int largura, altura;
    if (fscanf(arquivo, "%d %d", &largura, &altura) != 2) {
        fprintf(stderr, "Erro: Dimensões da imagem inválidas.\n");
        fclose(arquivo);
        return NULL;
    }

    ImagemPBM *imagem = criarImagem(largura, altura);
    if (!imagem) {
        fclose(arquivo);
        return NULL;
    }

    // Lê os pixels da imagem
    for (int i = 0; i < altura; i++) {
        for (int j = 0; j < largura; j++) {
            if (fscanf(arquivo, "%d", &imagem->pixels[i][j]) != 1) {
                fprintf(stderr, "Erro: Dados de pixel inválidos.\n");
                liberarImagem(imagem);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    return imagem;
}
