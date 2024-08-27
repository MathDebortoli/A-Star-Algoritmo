
#include "cabecalho.h"

// ===========================================================================
// Funcao para liberar a memoria da lista de nos
void freeLista(Node* lista[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free(lista[i]);
    }
}

// ===========================================================================
// Funcao para criar um no, ou seja alocar uma memoria e inicializa-lo
Node* criarNo(Node* parent, int position[2]) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->parent = parent;
    novo->position[0] = position[0];
    novo->position[1] = position[1];
    novo->g = 0;
    novo->h = 0;
    novo->f = 0;
    return novo;
}

// ===========================================================================
// Funcao principal do algoritmo A*
int** algoritmoAstar(int** maze, int linhas, int colunas, int start[2], int end[2], int * pathSize) {
    Node* startNode = criarNo(NULL, start);
    Node* endNode = criarNo(NULL, end);

    int mov[4][2] = { //defino movimentos possiveis
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };

    int maxNos = linhas * colunas;
    Node* openList[maxNos];
    Node* closedList[maxNos];
    int openSize = 0;
    int closedSize = 0;

    openList[openSize++] = startNode;

    while (openSize > 0) {
        Node* currentNode = openList[0];
        int currentIndex = 0;

        for (int i = 1; i < openSize; i++) {
            if (openList[i]->f < currentNode->f) {
                currentNode = openList[i];
                currentIndex = i;
            }
        }

        // Retira o no atual da lista aberta
        for (int i = currentIndex; i < openSize - 1; i++) {
            openList[i] = openList[i + 1];
        }
        openSize--;

        // Adiciona o no atual a lista fechada
        closedList[closedSize++] = currentNode;

        if (currentNode->position[0] == endNode->position[0] && currentNode->position[1] == endNode->position[1]) {
            // Encontrou o destino, reconstroi o caminho
            int** path = (int**)malloc(maxNos * sizeof(int*));
            for (int i = 0; i < maxNos; i++) {
                path[i] = (int*)malloc(2 * sizeof(int));
            }

            *pathSize = 0;

            while (currentNode != NULL) {
                path[*pathSize][0] = currentNode->position[0];
                path[*pathSize][1] = currentNode->position[1];
                *pathSize= *pathSize +1;

                currentNode = currentNode->parent;
            }

            // Inverte o caminho utilizado
            int** reversedPath = (int**)malloc((*pathSize) * sizeof(int*));
            for (int i = 0; i < *pathSize; i++) {
                reversedPath[i] = (int*)malloc(2 * sizeof(int));
                reversedPath[i][0] = path[*pathSize - i - 1][0];
                reversedPath[i][1] = path[*pathSize - i - 1][1];
            }

            // Libera a memoria alocada das nossas duas lisitnhas
            freeLista(openList, openSize);
            freeLista(closedList, closedSize);

            for (int i = 0; i < maxNos; i++) {
                free(path[i]);
            }
            free(path);

            return reversedPath;
        }

        // Gera os nos filhos
        Node* children[4];
        int numChildren = 0;

        for (int k = 0; k < 4; k++) {
            int nodeX = currentNode->position[0] + mov[k][0];
            int nodeY = currentNode->position[1] + mov[k][1];

            if (nodeX >= 0 && nodeX < linhas && nodeY >= 0 && nodeY < colunas && maze[nodeX][nodeY] == 0) {
                Node* newChild = criarNo(currentNode, (int[2]){nodeX, nodeY});
                children[numChildren++] = newChild;
            }
        }

        // Loop pelos filhos
        for (int i = 0; i < numChildren; i++) {
            // Filho ja esta na lista fechada
            int estaNaListaFechada = 0;
            for (int j = 0; j < closedSize; j++) {
                if (children[i]->position[0] == closedList[j]->position[0] && children[i]->position[1] == closedList[j]->position[1]) {
                    estaNaListaFechada = 1;
                    break;
                }
            }
            if (estaNaListaFechada) {
                continue;
            }

            // Cria os valores f, g e h
            children[i]->g = currentNode->g + 1;
            children[i]->h = pow((children[i]->position[0] - endNode->position[0]), 2) + pow((children[i]->position[1] - endNode->position[1]), 2);
            children[i]->f = children[i]->g + children[i]->h;

            // Filho ja esta na lista aberta com custo menor
            int estaNaListaAberta = 0;
            for (int j = 0; j < openSize; j++) {
                if (children[i]->position[0] == openList[j]->position[0] && children[i]->position[1] == openList[j]->position[1] && children[i]->g > openList[j]->g) {
                    estaNaListaAberta = 1;
                    break;
                }
            }
            if (estaNaListaAberta) {
                continue;
            }

            // Adiciona o filho a lista aberta
            openList[openSize++] = children[i];
        }
    }

    // Libera a memoria alocada
    freeLista(openList, openSize);
    freeLista(closedList, closedSize);

    return NULL;
}
// ===========================================================================
// Funcao responsavel por fazer a leitura do grid do arquivo txt e retornar-lo
int **lerMapa(int *linhas, int *colunas) {
    int num = 0, i, j,colum,lim=0;
    char linha[100];
    FILE *arquivo = fopen("mapa.txt", "r");

    //CASO ERRO NA LEITURA DO ARQUIVO
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo");
        return NULL;
    }

    while(fgets(linha,sizeof(linha),arquivo) != NULL) {
        colum = strlen(linha); //SALVAMOS QUANTOS CARACTERES TEM UMA LINHA
        lim++; //CONTAMOS QUANTAS LINHAS ELE LEU
    }

    *linhas = lim; //SALVAMOS AS LINHAS
    *colunas = (colum/2)+1; //DIVIDIMOS POR QUANTOS ESPACOS MAIS 1

    //ALOCAMOS MEMORIA PARA AS LINHAS
    int **mtz = (int **)malloc((*linhas) * sizeof(int *));
    //ALOCAMOS MEMORIA PARA AS COLUNAS
    for (i = 0; i < *linhas; i++) {
        mtz[i] = (int *)malloc((*colunas) * sizeof(int));
    }

    rewind(arquivo); //VOLTAMOS O PONTEIRO PRO INC
    for (i = 0; i < *linhas; i++) {
        for (j = 0; j < *colunas; j++) {
            fscanf(arquivo, "%d", &mtz[i][j]);
            //LEMOS DO ARQUIVO UM INT
        }
    }
    fclose(arquivo); //FECHAMOS O ARQUIVO
    return mtz; //RETORNAMOS A MATRIZ
}

// ===========================================================================
// Função para imprimir o caminho
void imprimirCaminho(int** path, int pathSize) {
    if (path == NULL) {
        printf("Caminho não encontrado.\n");
        return;
    }

    printf("\nCaminho encontrado:\n");
    for (int i = 0; i < pathSize; i++) {
        printf("[%d, %d] ", path[i][0], path[i][1]);
    }
    printf("\n");
}

// ===========================================================================
// Funcao para liberar a memoria alocada para o caminho
void freeCaminho(int** path, int pathSize) {
    if (path == NULL) {
        return;
    }

    for (int i = 0; i < pathSize; i++) {
        free(path[i]);
    }
    free(path);
}

// ===========================================================================
// Função para validar as coordenadas
int coordenadasValidas(int x, int y, int colunas, int linhas, int **grid) {
    return (x >= 0 && x < linhas && y >= 0 && y < colunas && grid[x][y] != 1);
}

// ===========================================================================
// Leitura do ponto de inicio e ponto de fim
void lerPontos(int *start, int *end, int **grid, int colunas, int linhas) {
    // Lemos os dois pontos
    do {
        printf("\nDigite as coordenadas de Inicio (formato: x y): ");
        scanf("%d %d", &start[0], &start[1]);

        printf("Digite as coordenadas de Fim (formato: x y): ");
        scanf("%d %d", &end[0], &end[1]);

        if (!coordenadasValidas(start[0], start[1], colunas, linhas, grid)) {
            printf("\nCoordenadas de Inicio invalidas. Tente novamente.\n");
        }

        if (!coordenadasValidas(end[0], end[1], colunas, linhas, grid)) {
            printf("\nCoordenadas de Fim invalidas. Tente novamente.\n");
        }

    } while (!coordenadasValidas(start[0], start[1], colunas, linhas, grid) || (!coordenadasValidas(end[0], end[1], colunas, linhas, grid)));
}

// ===========================================================================
// Codigo Principal
int main() {
    int i, j,linhas, colunas, pathSize;
    int start[2], end[2];
    
    //Fazemos a leitura do grid pelo txt
    int** grid = lerMapa(&linhas, &colunas);

    // Entrada do usuario para as posicoes de início e fim
    lerPontos(start,end, grid, colunas, linhas);
    //Chamada do algoritimo
    int** path = algoritmoAstar(grid, linhas, colunas, start, end, &pathSize);

    // Imprime o caminho encontrado
    imprimirCaminho(path, pathSize);

    // Libera a memoria alocada
    for (i = 0; i < linhas; i++) {
        free(grid[i]);
    }
    free(grid);
    freeCaminho(path, pathSize);

    return 0;
}