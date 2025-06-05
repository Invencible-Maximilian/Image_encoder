// Sávio Khendi 10295727
// Maximilian Wachter 10265078

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LARG 1024
#define MAX_ALT 768

char verificar(int** img, int lin_ini, int lin_fim, int col_ini, int col_fim) {
    int base = img[lin_ini][col_ini];
    int i, j;
    for (i = lin_ini; i < lin_fim; i++) {
        for (j = col_ini; j < col_fim; j++) {
            if (img[i][j] != base) {
                return 'X';
            }
        }
    }
    return base == 0 ? 'B' : 'P';
}

void codifica(int** img, int lin_ini, int lin_fim, int col_ini, int col_fim, char* cod) {
    char tipo = verificar(img, lin_ini, lin_fim, col_ini, col_fim);
    int tam = strlen(cod);
    cod[tam] = tipo;
    cod[tam + 1] = '\0';

    if (tipo != 'X') return;

    int meio_lin = (lin_ini + lin_fim) / 2;
    int meio_col = (col_ini + col_fim) / 2;

    int lin_inf = ((lin_fim - lin_ini) % 2 == 0) ? meio_lin : meio_lin + 1;
    int col_dir = ((col_fim - col_ini) % 2 == 0) ? meio_col : meio_col + 1;

    codifica(img, lin_ini, lin_inf, col_ini, col_dir, cod);     
    codifica(img, lin_ini, lin_inf, col_dir, col_fim, cod);     
    codifica(img, lin_inf, lin_fim, col_ini, col_dir, cod);     
    codifica(img, lin_inf, lin_fim, col_dir, col_fim, cod);     
}

int** criar(int alt, int larg) {
    int i;
    int** m = (int**)malloc(alt * sizeof(int*));
    for (i = 0; i < alt; i++) {
        m[i] = (int*)malloc(larg * sizeof(int));
    }
    return m;
}

void destruir(int** m, int alt) {
    int i;
    for (i = 0; i < alt; i++) {
        free(m[i]);
    }
    free(m);
}

int** ler_manual(int* alt, int* larg) {
    int i, j;
    printf("Largura: ");
    scanf("%d", larg);
    printf("Altura: ");
    scanf("%d", alt);

    int** img = criar(*alt, *larg);

    printf("Pixels (0 = branco, 1 = preto):\n");
    for (i = 0; i < *alt; i++) {
        for (j = 0; j < *larg; j++) {
            scanf("%d", &img[i][j]);
        }
    }
    return img;
}

int** ler_pbm(const char* nome, int* alt, int* larg) {
    FILE* arq = fopen(nome, "r");
    if (arq == NULL) {
        printf("Erro ao abrir arquivo.\n");
        exit(1);
    }

    char linha[128];
    fgets(linha, sizeof(linha), arq);

    while (fgets(linha, sizeof(linha), arq)) {
        if (linha[0] != '#') {
            sscanf(linha, "%d %d", larg, alt);
            break;
        }
    }

    int** img = criar(*alt, *larg);
    int i = 0, j = 0, v;
    while (fscanf(arq, "%d", &v) == 1) {
        img[i][j++] = v;
        if (j == *larg) {
            j = 0;
            i++;
        }
    }

    fclose(arq);
    return img;
}

void ajuda() {
    printf("Uso: programa [-? | -m | -f ARQ]\n");
    printf("  -?      Ajuda\n");
    printf("  -m      Entrada manual\n");
    printf("  -f ARQ  Arquivo PBM\n");
}

int main(int argc, char* argv[]) {
    int** img = NULL;
    int alt = 0, larg = 0;

    if (argc < 2) {
        ajuda();
        return 0;
    }

    if (strcmp(argv[1], "-?") == 0) {
        ajuda();
        return 0;
    } else if (strcmp(argv[1], "-m") == 0) {
        img = ler_manual(&alt, &larg);
    } else if (strcmp(argv[1], "-f") == 0 && argc >= 3) {
        img = ler_pbm(argv[2], &alt, &larg);
    } else {
        printf("Argumento inválido.\n");
        ajuda();
        return 1;
    }

    char cod[100000];
    cod[0] = '\0';

    codifica(img, 0, alt, 0, larg, cod);
    printf("Resultado: %s\n", cod);

    destruir(img, alt);
    return 0;
}

