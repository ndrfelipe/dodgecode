#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>  // getcwd

void tocar_som(const char* caminho_linux) {
    char caminho_convertido[512];
    char comando[1024];
    char cwd[256];

    getcwd(cwd, sizeof(cwd)); // pega diretório atual

    snprintf(caminho_convertido, sizeof(caminho_convertido), "%s/%s", cwd, caminho_linux);

    char caminho_windows[512] = {0};
    if (strncmp(caminho_convertido, "/mnt/c/", 7) == 0) {
        snprintf(caminho_windows, sizeof(caminho_windows), "C:%s", caminho_convertido + 7);
        for (int i = 0; caminho_windows[i]; i++) {
            if (caminho_windows[i] == '/') caminho_windows[i] = '\\';
        }
    } else {
        printf("Erro: som só funciona com arquivos em /mnt/c/ (Windows).\n");
        return;
    }

    // DEBUG: mostre caminho convertido
    printf("Caminho Windows convertido: %s\n", caminho_windows);

    // Tocar com app padrão do Windows
    snprintf(comando, sizeof(comando),
             "powershell.exe Start-Process -FilePath '%s'", caminho_windows);

    system(comando);
}
