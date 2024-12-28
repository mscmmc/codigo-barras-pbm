/* Aqui podemos colocar:
 - declarações de funções para manipulação da imagem PBM
 - definições de structs que representam a imagem
 - constantes e macros úteis para os dois programas */

#ifndef IMAGEM_H
#define IMAGEM_H

#include <stdbool.h>

// Constantes para os valores dos pixels
#define PIXEL_BRANCO 0
#define PIXEL_PRETO 1

// Struct para representar uma imagem PBM
typedef struct {
    int largura;    // Largura da imagem em pixels
    int altura;     // Altura da imagem em pixels
    int **pixels;   // Matriz de pixels (0 ou 1)
} ImagemPBM;

// Funções para manipulação de imagens
ImagemPBM *criarImagem(int largura, int altura);
void liberarImagem(ImagemPBM *imagem);
bool salvarImagemPBM(const char *nomeArquivo, const ImagemPBM *imagem);
ImagemPBM *carregarImagemPBM(const char *nomeArquivo);

#endif // IMAGEM_H

