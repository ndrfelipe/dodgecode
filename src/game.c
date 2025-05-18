#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/timer.h"
#include "../include/ranking.h"
#include "../include/menu.h"
#include "../include/som.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ATENÇÃO: Se MAXX (ex: 180) for maior que a largura REAL do seu terminal,
// ainda podem ocorrer problemas visuais. Ajuste MAXX e MAXY conforme necessário.
#define MAXX 80 // Reduzido para um valor mais comum. Ajuste conforme seu terminal!
#define MAXY 24 // Reduzido para um valor mais comum.
#define MAX_OBSTACULOS 100
#define MAX_RECOMPENSAS 10

#define PLAYER_W 3
#define PLAYER_H 3
#define OBST_W 7
#define OBST_H 4
#define REWARD_W 5
#define REWARD_H 3

#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RED    "\x1b[31m"
#define COLOR_GREEN  "\x1b[32m"
#define COLOR_RESET  "\x1b[0m"

static const char* player_sprite[PLAYER_H] = {
    " o ",
    "/|\\",
    "/ \\",
};
static const char* obst_sprite[OBST_H] = {
    "\\|||/ ",
    " (x_x) ",
    " / | \\",
    "  / \\  ",
};
static const char* reward_sprite[REWARD_H] = {
    " ___ ",
    "/$$$\\",
    "\\___/",
};

typedef struct { int x, y; } Entidade;
typedef struct { char nome[20]; int pontos; } Jogador;

Entidade jogador;
Entidade obstaculos[MAX_OBSTACULOS];
Entidade recompensas[MAX_RECOMPENSAS];
Jogador player;

// Variáveis para guardar a posição anterior do jogador para limpeza
int prev_player_x, prev_player_y;

int game_over = 0;
int max_obstaculos_ativos = 10;
int colisoes_obstaculo = 0;

// Função para limpar a área de um sprite (essencial para evitar rastros)
void clear_sprite_area(int x, int y, int w, int h) {
    for (int i = 0; i < h; i++) {
        // Verifica se a linha atual y+i está dentro dos limites verticais do jogo (0 a MAXY-1)
        if (y + i >= 0 && y + i < MAXY) {
            screenGotoxy(x + 1, y + i + 1);
            for (int j = 0; j < w; j++) {
                // Verifica se a coluna atual x+j está dentro dos limites horizontais (0 a MAXX-1)
                if (x + j >= 0 && x + j < MAXX) {
                    printf(" ");
                }
            }
        }
    }
}

void draw_sprite(const char** sprite, int w, int h, int x, int y, const char* color) {
    if (y < 0) return; // Não desenha se estiver inativo/fora da tela por cima

    printf("%s", color);
    for (int i = 0; i < h; i++) {
        if (y + i >= 0 && y + i < MAXY) { // Desenha apenas dentro dos limites Y do jogo
            screenGotoxy(x + 1, y + i + 1);
            int print_w = w;
            // Se o sprite ultrapassar MAXX, truncar a largura de impressão
            if (x + w > MAXX) {
                print_w = MAXX - x;
            }
            if (print_w > 0) { // Só imprime se houver largura útil
                 printf("%.*s", print_w, sprite[i]);
            }
        }
    }
    printf("%s", COLOR_RESET);
}


void inicializar_jogador() {
    jogador.x = MAXX/2 - PLAYER_W/2;
    jogador.y = 1;
    // Define as posições anteriores iniciais
    prev_player_x = jogador.x;
    prev_player_y = jogador.y;
}

void gerar_obstaculo() {
    int active_count = 0;
    for(int k=0; k<MAX_OBSTACULOS; ++k) if(obstaculos[k].y >=0) active_count++;
    if (active_count >= max_obstaculos_ativos) return;

    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (obstaculos[i].y < 0) {
            obstaculos[i].x = rand() % (MAXX - OBST_W);
            obstaculos[i].y = MAXY - OBST_H - 1;
            break;
        }
    }
}

void gerar_recompensa() {
    for (int i = 0; i < MAX_RECOMPENSAS; i++) {
        if (recompensas[i].y < 0) {
            recompensas[i].x = rand() % (MAXX - REWARD_W);
            recompensas[i].y = MAXY - REWARD_H - 1;
            break;
        }
    }
}

int colisao_area(Entidade a, Entidade b, int aw, int ah, int bw, int bh) {
    // Lógica de colisão AABB (Axis-Aligned Bounding Box)
    int a_left = a.x;
    int a_right = a.x + aw;
    int a_top = a.y;
    int a_bottom = a.y + ah;

    int b_left = b.x;
    int b_right = b.x + bw;
    int b_top = b.y;
    int b_bottom = b.y + bh;

    // Verifica se não há colisão
    if (a_right <= b_left || a_left >= b_right || a_bottom <= b_top || a_top >= b_bottom) {
        return 0; // Sem colisão
    }
    return 1; // Colisão
}

void verificar_colisao() {
    for (int i = 0; i < MAX_OBSTACULOS; i++) {
        if (obstaculos[i].y >= 0 && colisao_area(jogador, obstaculos[i], PLAYER_W, PLAYER_H, OBST_W, OBST_H)) {
            colisoes_obstaculo++;
            obstaculos[i].y = -1;
        }
    }
    for (int i = 0; i < MAX_RECOMPENSAS; i++) {
        if (recompensas[i].y >= 0 && colisao_area(jogador, recompensas[i], PLAYER_W, PLAYER_H, REWARD_W, REWARD_H)) {
            player.pontos++;
            recompensas[i].y = -1;
        }
    }
}

void desenhar_hud(int tempo) {
    screenGotoxy(1, MAXY + 1);
    // Limpa a linha do HUD antes de reescrever para evitar lixo de texto anterior mais longo
    for (int i = 0; i < MAXX; ++i) {
        printf(" ");
    }
    screenGotoxy(1, MAXY + 1); // Volta ao início da linha do HUD

    char hud_buffer[MAXX + 50]; // Buffer grande o suficiente
    snprintf(hud_buffer, sizeof(hud_buffer), "Pontos: %d  Colisões: %d/5  Tempo: %ds  Jogador: %s", player.pontos, colisoes_obstaculo, tempo, player.nome);
    printf("%.*s", MAXX, hud_buffer); // Imprime o HUD, truncando se exceder MAXX
}

void salvar_rank() {
    FILE* f = fopen("ranking.txt", "a");
    if (f) {
        fprintf(f, "%s %d\n", player.nome, player.pontos);
        fclose(f);
    }
}



void iniciar_jogo(char* nickname) {
    printf("\a"); 
    usleep(200000);
    strncpy(player.nome, nickname, sizeof(player.nome)-1);
    player.nome[sizeof(player.nome)-1] = '\0';

    int jogar_novamente = 1;
    while (jogar_novamente) {
        player.pontos = 0; colisoes_obstaculo = 0; max_obstaculos_ativos = 10;
        for (int i=0;i<MAX_OBSTACULOS;i++) obstaculos[i].y=-1;
        for (int i=0;i<MAX_RECOMPENSAS;i++) recompensas[i].y=-1;

        inicializar_jogador(); // Isso também define prev_player_x/y

        int frame=0; game_over=0;
        timerInit(1);
        long last_time_update=getTimeDiff();
        int interval=80; // ~12.5 FPS. Pode diminuir para ~60ms (16FPS) ou ~40ms (25FPS) para mais suavidade, se o terminal aguentar

        screenClear();      // Limpa a tela UMA VEZ no início do jogo
        screenDrawBorders(); // Desenha bordas UMA VEZ

        while (!game_over) {
            // --- CAPTURA DE INPUT ---
            if (keyhit()) {
                char t = readch();
                if (t == 'a' && jogador.x > 0) jogador.x--;
                if (t == 'd' && jogador.x < MAXX - PLAYER_W) jogador.x++;
            }

            long current_time = getTimeDiff();
            if (current_time - last_time_update >= interval) {
                // --- FASE DE LIMPEZA (CLEAR) ---
                // Apaga o jogador da posição anterior
                clear_sprite_area(prev_player_x, prev_player_y, PLAYER_W, PLAYER_H);

                // Apaga obstáculos e recompensas de suas posições anteriores (antes de mover)
                for (int i = 0; i < MAX_OBSTACULOS; i++) {
                    if (obstaculos[i].y >= 0) { // Se estava ativo no quadro anterior
                        clear_sprite_area(obstaculos[i].x, obstaculos[i].y, OBST_W, OBST_H);
                    }
                }
                for (int i = 0; i < MAX_RECOMPENSAS; i++) {
                    if (recompensas[i].y >= 0) { // Se estava ativo no quadro anterior
                        clear_sprite_area(recompensas[i].x, recompensas[i].y, REWARD_W, REWARD_H);
                    }
                }

                // --- FASE DE ATUALIZAÇÃO (UPDATE) ---
                // Posição X do jogador já foi atualizada pelo keyhit()
                // Posição Y do jogador é estática

                // Mover obstáculos
                for (int i=0; i<MAX_OBSTACULOS; i++) {
                    if (obstaculos[i].y>=0) {
                        obstaculos[i].y--; // Move para cima
                        if (obstaculos[i].y < 0 - OBST_H) { // Completamente fora da tela por cima
                            obstaculos[i].y = -1; // Marca como inativo para reutilização
                        }
                    }
                }
                // Mover recompensas
                for (int i=0; i<MAX_RECOMPENSAS; i++) {
                    if (recompensas[i].y>=0) {
                        recompensas[i].y--; // Move para cima
                         if (recompensas[i].y < 0 - REWARD_H) { // Completamente fora da tela por cima
                            recompensas[i].y = -1; // Marca como inativo para reutilização
                        }
                    }
                }

                if (frame % 5 == 0) gerar_obstaculo();
                if (frame % 15 == 0) gerar_recompensa();

                verificar_colisao(); // Pode marcar entidades como y = -1

                // --- FASE DE DESENHO (DRAW) ---
                // Desenha obstáculos e recompensas (apenas os ativos)
                for (int i=0; i<MAX_OBSTACULOS; i++) {
                    if (obstaculos[i].y>=0) {
                        draw_sprite((const char**)obst_sprite, OBST_W, OBST_H, obstaculos[i].x, obstaculos[i].y, COLOR_RED);
                    }
                }
                for (int i=0; i<MAX_RECOMPENSAS; i++) {
                    if (recompensas[i].y>=0) {
                        draw_sprite((const char**)reward_sprite, REWARD_W, REWARD_H, recompensas[i].x, recompensas[i].y, COLOR_GREEN);
                    }
                }
                // Desenha o jogador na nova posição
                draw_sprite((const char**)player_sprite, PLAYER_W, PLAYER_H, jogador.x, jogador.y, COLOR_YELLOW);

                // Atualiza as posições "anteriores" do jogador para o próximo quadro
                prev_player_x = jogador.x;
                prev_player_y = jogador.y;

                // Desenha o HUD
                int tempo_segundos = current_time/1000;
                desenhar_hud(tempo_segundos);

                // Lógica de aumento de dificuldade
                static int last_difficulty_increase_time = -10;
                if (tempo_segundos > 0 && tempo_segundos % 10 == 0 && tempo_segundos != last_difficulty_increase_time) {
                    if (max_obstaculos_ativos < MAX_OBSTACULOS) {
                         max_obstaculos_ativos = (max_obstaculos_ativos + 2 <= MAX_OBSTACULOS) ? max_obstaculos_ativos + 2 : MAX_OBSTACULOS;
                    }
                    last_difficulty_increase_time = tempo_segundos;
                }

                if (colisoes_obstaculo>=5) game_over=1;

                frame++;
                last_time_update = current_time;
                fflush(stdout); // Importante! Envia tudo para o terminal.
            }
            usleep(10000); // Reduzido para maior responsividade do input e loop mais rápido. Teste valores como 1000-10000.
                        // Se o jogo parecer lento, aumente `interval`. Se parecer "pesado", aumente usleep.
        }

        // --- FIM DE JOGO ---
        salvar_rank();
        screenClear();
        screenDrawBorders();
        screenGotoxy(MAXX/2 - 15, 19);
        printf("Game Over, %s! Pontos: %d", player.nome, player.pontos);
        mostrar_rank();
        screenGotoxy(MAXX/2 - 15, 20);
        printf("Pressione 'q' para sair.");
        fflush(stdout);

        int c;
        while(1){
            c = getchar();
            if (c == 'q' || c == 'Q') {
                jogar_novamente = 0;
                break;
            }
        }
    }
}
