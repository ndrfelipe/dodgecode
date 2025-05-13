#include "../include/keyboard.h"
#include "../include/screen.h"
#include "../include/timer.h"
#include "../include/game.h"
#include <stdio.h>
#include <string.h>

#define LARGURA 50
int main() {
    char nickname[20];

    // === 1) Tela de entrada do nick ===
    // Terminal normal, mostra o que digita
    printf("Digite o seu nick: ");
    if (fgets(nickname, sizeof(nickname), stdin)) {
        nickname[strcspn(nickname, "\n")] = 0;
    }

    // Aguarda ENTER para ir ao menu
    printf("Pressione ENTER para ir ao menu...");
    getchar();

    // === 2) Tela de menu (modo cli-lib) ===
    screenInit(1);       // inicializa com bordas
    keyboardInit();      // esconde cursor, etc.
    screenClear();
    screenDrawBorders();

    // Nome do jogo
    screenGotoxy( (LARGURA/2) - 10, 5 );
    printf("=== O JOGO DO ESTAGIÁRIO ===");

    // Saudação personalizada
    screenGotoxy(2, 8);
    printf("Olá, %s!", nickname);

    // Instrução de início
    screenGotoxy(2, 10);
    printf("Pressione ENTER para começar...");

    // Espera ENTER
    while (getchar() != '\n');

    // Limpa a tela e inicia o jogo
    screenClear();
    iniciar_jogo(nickname);

    keyboardDestroy();
    screenDestroy();
    return 0;
}