#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/screen.h"

#define MAX_RANKS 100
#define LARGURA_TELA 70

typedef struct {
    char nome[20];
    int pontos;
} Rank;

void mostrar_rank() {
    FILE* f = fopen("ranking.txt", "r");
    Rank ranks[MAX_RANKS];
    int count = 0;

    screenClear();
    screenDrawBorders();

    if (!f) {
        screenGotoxy((LARGURA_TELA / 2) - 10, 10);
        printf("\033[1;31mNenhum ranking salvo ainda.\033[0m");
        return;
    }

    // Lê os dados do arquivo
    while (fscanf(f, "%19s %d", ranks[count].nome, &ranks[count].pontos) == 2 && count < MAX_RANKS) {
        count++;
    }
    fclose(f);

    // Ordena do maior para o menor
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (ranks[j].pontos > ranks[i].pontos) {
                Rank tmp = ranks[i];
                ranks[i] = ranks[j];
                ranks[j] = tmp;
            }
        }
    }

    // ASCII art para o topo do ranking
    screenGotoxy((LARGURA_TELA / 2) - 6, 3);
    printf("\033[1;33m╔════════════════════╗\033[0m");
    screenGotoxy((LARGURA_TELA / 2) - 6, 4);
    printf("\033[1;33m║   RANKING STAGS    ║\033[0m");
    screenGotoxy((LARGURA_TELA / 2) - 6, 5);
    printf("\033[1;33m╚════════════════════╝\033[0m");

    int linha = 7;
    for (int i = 0; i < count && i < 10 && linha < MAXY - 3; i++){
        screenGotoxy((LARGURA_TELA / 2) - 10, linha++);
        printf("\033[1;36m%2d. %-15s \033[1;32m%3d pts\033[0m", i + 1, ranks[i].nome, ranks[i].pontos);
    }
}
