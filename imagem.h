/* Aqui podemos colocar:
 - declarações de funções para manipulação da imagem PBM
 - definições de structs que representam a imagem
 - constantes e macros úteis para os dois programas */

#ifndef IMAGEM_H
#define IMAGEM_H

#include <stdbool.h>

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

