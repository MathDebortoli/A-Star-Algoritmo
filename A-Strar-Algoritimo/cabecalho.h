//Matheus Francisco Debortoli Silva e Leandro de Souza Amorim

// ===========================================================================
// Bibliotecas Utilizadas
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// ===========================================================================
// Struct de no para o algoritmo A*
typedef struct Node {
    struct Node* parent;
    int position[2];
    int g, h, f;
} Node;

// ===========================================================================
// Assinaturas das Funcoes
int **lerMapa(int *linhas, int *colunas);
void imprimirMatriz(int **mtz, int linhas, int colunas);
void freeMatriz(int **mtz,int linhas, int colunas);
int** algoritmoAstar(int** maze, int linhas, int colunas, int start[2], int end[2], int * pathSize);
void freeLista(Node* lista[], int tamanho);
void imprimirCaminho(int** path, int pathSize);
void freeCaminho(int** path, int pathSize);
void lerPontos(int *start, int *end, int **grid, int colunas, int linhas);
int **lerMapa(int *linhas, int *colunas);
int coordenadasValidas(int x, int y, int colunas, int linhas, int **grid);