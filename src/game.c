// game.c

#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define ALTURA 25
#define LARGURA 50
#define MAX_OBSTACULOS 10
#define MAX_RECOMPENSAS 10

typedef struct {
    int x, y;
} Entidade;

typedef struct {
    char nome[20];
    int pontos;
} Jogador;

Entidade jogador;
Entidade obstaculos[MAX_OBSTACULOS];
Entidade recompensas[MAX_RECOMPENSAS];
Jogador player;

int jogador_x_ant = -1, jogador_y_ant = -1;

int game_over = 0;

void inicializar_jogador() {
    jogador.x = LARGURA / 2;
    jogador.y = 1; // fixo no final da tela
}

void gerar_obstaculo() {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (obstaculos[i].y < 0) {
            obstaculos[i].x = rand() % LARGURA;
            obstaculos[i].y = ALTURA - 2;
            break;
        }
    }
}

void gerar_recompensa() {
    for (int i = 0; i < MAX_RECOMPENSAS; i++) {
        if (recompensas[i].y < 0) {
            recompensas[i].x = rand() % LARGURA;
            recompensas[i].y = ALTURA - 2;
            break;
        }
    }
}

void atualizar_obstaculos() {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (obstaculos[i].y >= 0) {
            // APAGA posição antiga
            screenGotoxy(obstaculos[i].x + 1, obstaculos[i].y);
            printf(" ");

            if (obstaculos[i].y <= 1) {
                obstaculos[i].y = -1;
            } else {
                obstaculos[i].y--;
            }
        }
    }
}

void atualizar_recompensas() {
    for (int i = 0; i < MAX_RECOMPENSAS; i++) {
        if (recompensas[i].y >= 0) {
            // APAGA posição antiga
            screenGotoxy(recompensas[i].x + 1, recompensas[i].y);
            printf(" ");

            if (recompensas[i].y <= 1) {
                recompensas[i].y = -1;
            } else {
                recompensas[i].y--;
            }
        }
    }
}

void verificar_colisao() {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (abs(obstaculos[i].x - jogador.x) <= 0 &&
            obstaculos[i].y == jogador.y) {
            player.pontos--;
            obstaculos[i].y = -1;
        }
    }
    for (int i = 0; i < MAX_RECOMPENSAS; i++) {
        if (abs(recompensas[i].x - jogador.x) <= 0 &&
            recompensas[i].y == jogador.y) {
            player.pontos++;
            recompensas[i].y = -1;
        }
    }
}



void desenhar_mapa(int tempo) {
    // APAGA posição anterior do jogador (antes de desenhar novo)
    if (jogador_x_ant >= 0 && jogador_y_ant >= 0) {
        screenGotoxy(jogador_x_ant + 1, jogador_y_ant + 1);
        printf(" ");
    }

    // Obstáculos
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (obstaculos[i].y >= 0)
            screenGotoxy(obstaculos[i].x + 1, obstaculos[i].y), printf("X");
    }

    // Recompensas
    for (int i = 0; i < MAX_RECOMPENSAS; i++) {
        if (recompensas[i].y >= 0)
            screenGotoxy(recompensas[i].x + 1, recompensas[i].y), printf("$");
    }

    // Jogador (depois de apagar a posição antiga)
    screenGotoxy(jogador.x + 1, jogador.y + 1);
    printf("@");

    // Pontuação e tempo
    screenGotoxy(1, ALTURA + 2);
    printf("Pontos: %d   Tempo: %ds    Player: %s", player.pontos, tempo, player.nome);

    fflush(stdout);
}


void salvar_rank() {
    FILE* f = fopen("ranking.txt", "a");
    if (f) {
        fprintf(f, "%s %d\n", player.nome, player.pontos);
        fclose(f);
    }
}

void atualizar_jogador_anterior() {
    jogador_x_ant = jogador.x;
    jogador_y_ant = jogador.y;
}

void iniciar_jogo(char* nickname) {
    screenClear();
    screenDrawBorders();

    strncpy(player.nome, nickname, sizeof(player.nome));
    player.pontos = 0;

    inicializar_jogador();
    for (int i = 0; i < MAX_OBSTACULOS; i++) obstaculos[i].y = -1;
    for (int i = 0; i < MAX_RECOMPENSAS; i++) recompensas[i].y = -1;

    int frame = 0;
    game_over = 0;

    timerInit(1); // Timer 1 ativado

    long ultimo_frame = getTimeDiff(); // marca o tempo inicial em ms
    int intervalo_frame = 100;

    while (!game_over) {
         
        if (keyhit()) {
            char tecla = readch();
            if (tecla == 'a' && jogador.x > 0) jogador.x--;
            if (tecla == 'd' && jogador.x < LARGURA - 1) jogador.x++;
        }

        long agora = getTimeDiff();
        if (agora - ultimo_frame >= intervalo_frame) {
            if (frame % 5 == 0) gerar_obstaculo();
            if (frame % 7 == 0) gerar_recompensa();

            atualizar_obstaculos();
            atualizar_recompensas();

            verificar_colisao();

            int tempo_segundos = agora / 1000;
            desenhar_mapa(tempo_segundos);   // desenha o novo
            atualizar_jogador_anterior();    // só depois salva a nova posição como "anterior"

            frame++;
            ultimo_frame = agora;
        }


        usleep(10000); // curto intervalo para não travar a CPU

        if (player.pontos < -5) game_over = 1;
    }

    salvar_rank();

    screenClear();
    screenGotoxy(10, 10);
    printf("Game Over, %s! Pontos: %d\n", player.nome, player.pontos);
    printf("Pressione ENTER para voltar...");
    getchar(); getchar();
}