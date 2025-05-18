#include "../include/keyboard.h"
#include "../include/screen.h"
#include "../include/timer.h"
#include "../include/game.h"
#include "../include/ranking.h"
#include "../include/som.h"


#include <stdio.h>
#include <string.h>
#include <unistd.h>  // usleep()
#include <stdlib.h>  // system()

#define LARGURA 70

// === Códigos de Cor ANSI ===
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

// === ASCII Art ===
void desenhar_ascii_logo() {
    const char* logo[] = {
        " __    ____  ____  ____    ___  ____   __    ___ ",
        "(  )  (_  _)( ___)( ___)  / __)(_  _) /__\\  / __)",
        " )(__  _)(_  )__)  )__)   \\__ \\  )(  /(__)\\( (_-.",
        "(____)(____)(__)  (____)  (___/ (__)(__)(__)\\___/",
        NULL
    };

    int linha = 2;
    for (int i = 0; logo[i] != NULL; i++) {
        screenGotoxy((LARGURA / 2) - 20, linha++);
        printf(CYAN "%s" RESET, logo[i]);
    }
}



// === Animação de Carregamento ===
void animacao_inicial() {
    const char* frames[] = {
        "Carregando [     ]",
        "Carregando [=    ]",
        "Carregando [==   ]",
        "Carregando [===  ]",
        "Carregando [==== ]",
        "Carregando [=====]",
        NULL
    };

    for (int i = 0; frames[i] != NULL; i++) {
        screenClear();
        screenGotoxy((LARGURA / 2) - 8, 10);
        printf(GREEN "%s" RESET, frames[i]);
        fflush(stdout);
        usleep(200000);
    }
}

// === Mensagem informativa antes do jogo ===
void exibir_informacoes_do_jogo() {
    screenClear();
    screenDrawBorders();

    screenGotoxy(3, 3);
    printf(YELLOW "Seja bem-vindo ao " CYAN "Life's Stag" YELLOW "." RESET);
    screenGotoxy(3, 5);
    printf("Nesse mundo você precisa desviar dos Monstros");
    screenGotoxy(3, 6);
    printf("Roubadores de Tempo para sobreviver e ao mesmo");
    screenGotoxy(3, 7);
    printf("tempo coletar Dinheiro para ganhar pontos.");

    screenGotoxy(3, 9);
    printf("Se você colidir com 5 Monstros Roubadores de Tempo, você perde.");

    screenGotoxy(3, 11);
    printf("Quanto mais tempo sobreviver e mais pontos coletar,");
    screenGotoxy(3, 12);
    printf("mais alto você fica no ranking!");

    screenGotoxy(3, 14);
    printf(GREEN "Boa sorte, querido(a) Stag." RESET);

    screenGotoxy(3, 16);
    printf("Pressione ENTER para iniciar...");
    while (getchar() != '\n');
}

int iniciar_menu() {
    char nickname[20];

    printf("Olá! Digite o seu apelido para iniciarmos o Jogo :P : ");
    if (fgets(nickname, sizeof(nickname), stdin)) {
        nickname[strcspn(nickname, "\n")] = 0;
    }

    screenInit(1);
    keyboardInit();

    animacao_inicial();

    while (1) {
        screenClear();
        screenDrawBorders();
        tocar_som("./assets/menu.wav");
        desenhar_ascii_logo();

        screenGotoxy((LARGURA / 2) - 5, 10);
        printf(YELLOW "Olá, %s!" RESET, nickname);

        screenGotoxy((LARGURA / 2) - 5, 12);
        printf(GREEN "1 - Começar" RESET);
        screenGotoxy((LARGURA / 2) - 5, 13);
        printf(CYAN "2 - Ranking" RESET);
        screenGotoxy((LARGURA / 2) - 5, 14);
        printf(RED "3 - Sair" RESET);

        int c = getchar();

        if (c == '1') {
            exibir_informacoes_do_jogo();  // Mostra explicação
            screenClear();
            iniciar_jogo(nickname);
        }
        else if (c == '2') {
            screenClear();
            screenDrawBorders();
            mostrar_rank();

            screenGotoxy((LARGURA / 2) - 10, 20);
            printf("Pressione ENTER para voltar ao menu...");
            while (getchar() != '\n');
        }
        else if (c == '3') {
            break;
        }
    }

    keyboardDestroy();
    screenDestroy();
    return 0;
}