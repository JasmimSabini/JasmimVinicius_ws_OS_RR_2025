#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Matrizes de exemplo
int sudokus[6][SIZE][SIZE];

// Resultados das validações
int linha_valida[SIZE], coluna_valida[SIZE], bloco_valida[SIZE];

// Struct para passar múltiplos parâmetros para threads
typedef struct {
    int index;
    int sudoku_id;
} parametro;

// Função para imprimir o sudoku
void imprime_sudoku(int sudoku_id) {
    printf("\n--- Sudoku %d ---\n", sudoku_id + 1);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", sudokus[sudoku_id][i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Função para encontrar célula vazia
int encontrar_vazio(int sudoku_id, int *linha, int *coluna) {
    for (*linha = 0; *linha < SIZE; (*linha)++) {
        for (*coluna = 0; *coluna < SIZE; (*coluna)++) {
            if (sudokus[sudoku_id][*linha][*coluna] == 0)
                return 1;
        }
    }
    return 0;
}

// Função para checar se é seguro colocar um número
int seguro(int sudoku_id, int linha, int coluna, int num) {
    // Verificar linha e coluna
    for (int x = 0; x < SIZE; x++) {
        if (sudokus[sudoku_id][linha][x] == num) return 0;
        if (sudokus[sudoku_id][x][coluna] == num) return 0;
    }

    // Verificar bloco 3x3
    int linha_inicio = (linha / 3) * 3;
    int coluna_inicio = (coluna / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudokus[sudoku_id][linha_inicio + i][coluna_inicio + j] == num)
                return 0;
        }
    }

    return 1;
}

// -----------Resolução com Backtracking---------------
int resolver_sudoku(int sudoku_id) {
    int linha, coluna;
    if (!encontrar_vazio(sudoku_id, &linha, &coluna))
        return 1; // Sudoku resolvido

    for (int num = 1; num <= 9; num++) {
        if (seguro(sudoku_id, linha, coluna, num)) {
            sudokus[sudoku_id][linha][coluna] = num;

            if (resolver_sudoku(sudoku_id))
                return 1;

            sudokus[sudoku_id][linha][coluna] = 0; // Backtrack
        }
    }
    return 0;
}

// -----------Funções das threads-----------------
void *verifica_linha(void *param) {
    parametro *p = (parametro *) param;
    int linha = p->index;
    int sudoku_id = p->sudoku_id;
    int check[SIZE] = {0};

    for (int j = 0; j < SIZE; j++) {
        int num = sudokus[sudoku_id][linha][j];
        if (num < 1 || num > 9 || check[num - 1] == 1) {
            printf(" Thread Linha %d do Sudoku %d encontrou erro!\n", linha + 1, sudoku_id + 1);
            pthread_exit(NULL);
        }
        check[num - 1] = 1;
    }

    linha_valida[linha] = 1;
    printf(" Thread Linha %d do Sudoku %d finalizada com sucesso!\n", linha + 1, sudoku_id + 1);
    pthread_exit(NULL);
}

void *verifica_coluna(void *param) {
    parametro *p = (parametro *) param;
    int coluna = p->index;
    int sudoku_id = p->sudoku_id;
    int check[SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        int num = sudokus[sudoku_id][i][coluna];
        if (num < 1 || num > 9 || check[num - 1] == 1) {
            printf(" Thread Coluna %d do Sudoku %d encontrou erro!\n", coluna + 1, sudoku_id + 1);
            pthread_exit(NULL);
        }
        check[num - 1] = 1;
    }

    coluna_valida[coluna] = 1;
    printf(" Thread Coluna %d do Sudoku %d finalizada com sucesso!\n", coluna + 1, sudoku_id + 1);
    pthread_exit(NULL);
}

void *verifica_bloco(void *param) {
    parametro *p = (parametro *) param;
    int bloco = p->index;
    int sudoku_id = p->sudoku_id;
    int check[SIZE] = {0};

    int linha_inicio = (bloco / 3) * 3;
    int coluna_inicio = (bloco % 3) * 3;

    for (int i = linha_inicio; i < linha_inicio + 3; i++) {
        for (int j = coluna_inicio; j < coluna_inicio + 3; j++) {
            int num = sudokus[sudoku_id][i][j];
            if (num < 1 || num > 9 || check[num - 1] == 1) {
                printf(" Thread Bloco (%d,%d) do Sudoku %d encontrou erro!\n",
                       linha_inicio / 3 + 1, coluna_inicio / 3 + 1, sudoku_id + 1);
                pthread_exit(NULL);
            }
            check[num - 1] = 1;
        }
    }

    bloco_valida[bloco] = 1;
    printf(" Thread Bloco (%d,%d) do Sudoku %d finalizada com sucesso!\n",
           linha_inicio / 3 + 1, coluna_inicio / 3 + 1, sudoku_id + 1);
    pthread_exit(NULL);
}

// -------------------- Validação --------------------------
void validar_sudoku(int sudoku_id) {
    pthread_t threads[27];
    parametro params[27];
    int t = 0;

    // Resetando vetores de validação
    for (int i = 0; i < SIZE; i++) {
        linha_valida[i] = 0;
        coluna_valida[i] = 0;
        bloco_valida[i] = 0;
    }

    // Criando threads para linhas
    for (int i = 0; i < SIZE; i++) {
        params[t] = (parametro){i, sudoku_id};
        pthread_create(&threads[t], NULL, verifica_linha, &params[t]);
        t++;
    }

    // Criando threads para colunas
    for (int i = 0; i < SIZE; i++) {
        params[t] = (parametro){i, sudoku_id};
        pthread_create(&threads[t], NULL, verifica_coluna, &params[t]);
        t++;
    }

    // Criando threads para blocos
    for (int i = 0; i < SIZE; i++) {
        params[t] = (parametro){i, sudoku_id};
        pthread_create(&threads[t], NULL, verifica_bloco, &params[t]);
        t++;
    }

    // Esperando todas as threads terminarem
    for (int i = 0; i < 27; i++) {
        pthread_join(threads[i], NULL);
    }

    // Verificando resultado final
    int valido = 1;
    for (int i = 0; i < SIZE; i++) {
        if (linha_valida[i] == 0 || coluna_valida[i] == 0 || bloco_valida[i] == 0) {
            valido = 0;
            break;
        }
    }

    if (valido) {
        printf(" Sudoku %d é VÁLIDO!\n\n", sudoku_id + 1);
    } else {
        printf(" Sudoku %d é INVÁLIDO!\n\n", sudoku_id + 1);
    }
}

// -------------------- Carregar Sudokus ---------------------
void carregar_sudokus() {
    // Sudokus parciais
    int sudoku1[SIZE][SIZE] = {
        {5,3,0, 0,7,0, 0,0,0},
        {6,0,0, 1,9,5, 0,0,0},
        {0,9,8, 0,0,0, 0,6,0},
        {8,0,0, 0,6,0, 0,0,3},
        {4,0,0, 8,0,3, 0,0,1},
        {7,0,0, 0,2,0, 0,0,6},
        {0,6,0, 0,0,0, 2,8,0},
        {0,0,0, 4,1,9, 0,0,5},
        {0,0,0, 0,8,0, 0,7,9}
    };

    int sudoku2[SIZE][SIZE] = {
        {0,0,0, 0,0,0, 0,1,2},
        {0,0,0, 0,0,0, 0,3,4},
        {0,0,0, 0,0,0, 0,5,6},
        {0,0,0, 0,0,0, 0,7,8},
        {0,0,0, 0,0,0, 0,9,1},
        {0,0,0, 0,0,0, 0,2,3},
        {0,0,0, 0,0,0, 0,4,5},
        {0,0,0, 0,0,0, 0,6,7},
        {0,0,0, 0,0,0, 0,8,9}
    };

    int sudoku3[SIZE][SIZE] = {
        {0,2,0, 6,0,8, 0,0,0},
        {5,8,0, 0,0,9, 7,0,0},
        {0,0,0, 0,4,0, 0,0,0},
        {3,7,0, 0,0,0, 5,0,0},
        {6,0,0, 0,0,0, 0,0,4},
        {0,0,8, 0,0,0, 0,1,3},
        {0,0,0, 0,2,0, 0,0,0},
        {0,0,9, 8,0,0, 0,3,6},
        {0,0,0, 3,0,6, 0,9,0}
    };

    // Copiar para os 3 primeiros sudokus
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            sudokus[0][i][j] = sudoku1[i][j];
            sudokus[1][i][j] = sudoku2[i][j];
            sudokus[2][i][j] = sudoku3[i][j];
        }
    }

    // Sudoku completo correto (para gerar erros)
    int base[SIZE][SIZE] = {
        {5,3,4,6,7,8,9,1,2},
        {6,7,2,1,9,5,3,4,8},
        {1,9,8,3,4,2,5,6,7},
        {8,5,9,7,6,1,4,2,3},
        {4,2,6,8,5,3,7,9,1},
        {7,1,3,9,2,4,8,5,6},
        {9,6,1,5,3,7,2,8,4},
        {2,8,7,4,1,9,6,3,5},
        {3,4,5,2,8,6,1,7,9}
    };

    // Inserindo 5 na Linha 1 Coluna 2
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            sudokus[3][i][j] = base[i][j];
    sudokus[3][0][1] = 5;

}

// -------------------- Main ---------------------
int main() {
    carregar_sudokus();

    printf("\n===== PARTE 1: RESOLVER + VALIDAR =====\n");
    for (int i = 0; i < 3; i++) {
        imprime_sudoku(i);
        printf(" Resolvendo Sudoku %d...\n", i + 1);
        if (resolver_sudoku(i)) {
            printf(" Sudoku %d resolvido com sucesso!\n", i + 1);
        } else {
            printf(" Sudoku %d não tem solução!\n", i + 1);
        }
        imprime_sudoku(i);
        printf(" Iniciando validação do Sudoku %d...\n", i + 1);
        validar_sudoku(i);
    }

    printf("\n===== PARTE 2: VALIDAR COM ERROS =====\n");
    for (int i = 3; i < 4; i++) {
        imprime_sudoku(i);
        printf(" Iniciando validação do Sudoku %d...\n", i + 1);
        validar_sudoku(i);
    }

    return 0;
}
