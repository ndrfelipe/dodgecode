# "Life'Stag"

Bem-vindo ao "Life'Stag"! Um jogo de arcade desafiador e divertido, executado diretamente no seu terminal. Prepare-se para testar seus reflexos, desviando de obstáculos e coletando recompensas para alcançar a maior pontuação possível.
<div align="center">
  <img src="https://github.com/user-attachments/assets/111b3d73-3b36-41bf-8dd6-5d70231ce0a0" alt="Texto alternativo da imagem" width="500">
</div>




## Índice

* [Sobre o Jogo](#sobre-o-jogo)
* [Como Jogar](#como-jogar)
    * [Visão Geral](#visão-geral)
    * [Controles](#controles)
    * [Objetivos](#objetivos)
    * [Dinâmica do Jogo](#dinâmica-do-jogo)
    * [Fim de Jogo](#fim-de-jogo)
* [Sistema de Pontuação](#sistema-de-pontuação)
* [Ranking](#ranking)
* [Tecnologias Utilizadas](#tecnologias-utilizadas)
* [Pré-requisitos e Dependências](#pré-requisitos-e-dependências)
* [Como Compilar e Executar](#como-compilar-e-executar)
* [Funções Principais do Jogo](#funções-principais-do-jogo)
* [Equipe do Projeto](#equipe-do-projeto)
* [Informações da Disciplina](#informações-da-disciplina)

## Sobre o Jogo

"Life'Stag" é um jogo desenvolvido em C para ser executado em uma interface de linha de comando (CLI). No papel de um Estagiário, o jogador deve se mover horizontalmente na parte superior da tela para desviar de Obstáculos e coletar Recompensas. Esses itens surgem da parte inferior da tela e se movem verticalmente em direção ao topo.

O objetivo é simples: acumular o máximo de pontos coletando recompensas, enquanto evita um número fatal de colisões com os obstáculos. A dificuldade aumenta progressivamente à medida que mais obstáculos aparecem, exigindo agilidade e concentração.

## Como Jogar

### Visão Geral
Você controla o personagem (o Estagiário), que inicia sua jornada posicionado no centro da parte superior da tela (linha 1). Sua missão é sobreviver o maior tempo possível, coletando bolsas de dinheiro e evitando os Monstros Sugadores de Tempo que sobem pela tela.

### Controles
O jogador pode se mover apenas horizontalmente:
* **`A`**: Move o jogador para a esquerda (limitado pela borda da tela).
* **`D`**: Move o jogador para a direita (limitado pela borda da tela).

### Objetivos
* **Coletar Recompensas:** Cada recompensa coletada adiciona `+1` ponto à sua pontuação.
* **Evitar Obstáculos:** O jogador deve evitar colisões com os obstáculos. Não há perda de pontos ao colidir, mas cada colisão é registrada. Após 5 colisões com obstáculos, o jogo termina.

### Dinâmica do Jogo
* **Tela:** A ação acontece em um ambiente virtual com 25 linhas de altura e 50 colunas de largura.
* **Geração de Itens:**
    * Um novo Obstáculo é gerado a cada 5 frames.
    * Uma nova Recompensa é gerada a cada 7 frames.
    * Pode haver um máximo de 10 obstáculos e 10 recompensas simultaneamente na tela.
    * Obstáculos e recompensas surgem da parte inferior da tela e sobem uma linha por ciclo. Se alcançarem a linha superior (ou a linha onde o jogador está) sem colisão, eles desaparecem do mapa.
* **Atualização por Frame:**
    * O jogo opera com um sistema de frames temporizados, com um novo frame a cada 100ms.
    * A cada frame, ocorrem as seguintes ações:
        1.  Geração de novos obstáculos e/ou recompensas, conforme a lógica de tempo.
        2.  Atualização das posições verticais dos obstáculos e recompensas existentes (movendo-os para cima).
        3.  Verificação de colisões entre o jogador e os itens.
        4.  Redesenho completo da tela, exibindo a posição atual do jogador, obstáculos e recompensas, além de informações como pontuação.
<div align="center">
  <img src="https://github.com/user-attachments/assets/1c8ea12d-a7c3-46f2-a864-a997882d5bef" alt="Texto alternativo da imagem" width="500">
</div>

### Fim de Jogo
O jogo é encerrado automaticamente quando o jogador colide com um obstáculo pela quinta vez.
Ao encerrar:
1.  A tela do terminal é limpa.
2.  O ranking atualizado é mostrado.
3.  O jogo aguarda que o usuário pressione a tecla `Q` para finalizar a execução.

<div align="center">
  <img src="https://github.com/user-attachments/assets/2a786e42-e115-4c89-830a-fb0dd51fd813" alt="Texto alternativo da imagem" width="500">
</div>

## Sistema de Pontuação

O sistema de pontuação foi desenhado para ser simples e recompensador, incentivando a coleta de bônus e a habilidade de desvio:

* **Coletar uma Recompensa:** O jogador ganha `+1` ponto. A recompensa coletada é removida da tela.
* **Colidir com um Obstáculo:** O jogador **não perde pontos**. No entanto, a colisão é contabilizada. Após 5 colisões, o jogo termina. O obstáculo colidido é removido da tela.

À medida que o tempo avança, a dificuldade aumenta gradualmente com mais obstáculos ativos na tela, exigindo reflexos mais rápidos para pontuar com segurança.

## Ranking

Ao final de cada partida, o nickname fornecido pelo jogador ao iniciar o jogo e sua pontuação final são registrados em um arquivo local chamado `ranking.txt`. Este arquivo armazena o histórico de pontuações, permitindo que os jogadores acompanhem seu progresso e compitam por recordes.

## Tecnologias Utilizadas

* **Linguagem de Programação:** C
* **Ambiente de Execução:** Interface de Linha de Comando (CLI)

## Pré-requisitos e Dependências

Para compilar e executar o "Life'Stag", você precisará de:
* Um compilador C (como GCC, Clang, etc.).
* As seguintes bibliotecas customizadas, que devem estar localizadas nos caminhos especificados pelos `#include` no código-fonte (presumivelmente em um diretório `../include/` em relação aos arquivos `.c` principais):
    * `screen.h`
    * `keyboard.h`
    * `timer.h`

Estas bibliotecas devem prover as seguintes funcionalidades essenciais:
* **`screen.h`**:
    * `screenClear()`: Para limpar a tela do terminal.
    * `screenGotoxy(int x, int y)`: Para posicionar o cursor em coordenadas específicas da tela.
    * `screenDrawBorders()`: Para desenhar as bordas da área de jogo.
* **`keyboard.h`**:
    * `keyhit()`: Para verificar se alguma tecla foi pressionada (não bloqueante).
    * `readch()`: Para ler o caractere da tecla pressionada (pode ser bloqueante ou não, dependendo da implementação).
* **`timer.h`**:
    * `timerInit()`: Para inicializar ou marcar um ponto de referência no tempo.
    * `getTimeDiff()`: Para obter a diferença de tempo (em milissegundos, por exemplo) desde a inicialização ou última marcação, crucial para o controle de frames.

## Como Compilar e Executar

O projeto utiliza um `Makefile` para simplificar o processo de compilação.

1.  **Clone o Repositório (se aplicável):**
    ```bash
    git clone <URL_DO_SEU_REPOSITORIO_GIT>
    cd NOME_DO_DIRETORIO_DO_PROJETO # Ex: cd DODGECODE
    ```

2.  **Compile o Projeto:**
    No diretório raiz do projeto (onde o `Makefile` está localizado), execute o comando:
    ```bash
    make
    ```
    Este comando irá:
    * Criar o diretório `build/` se ele não existir (conforme a regra `@mkdir -p $(BUILD_DIR)`).
    * Compilar todos os arquivos fonte (`.c`) localizados no diretório `src/`, utilizando os cabeçalhos (`.h`) do diretório `include/`.
    * Gerar o executável chamado `jogo` dentro do diretório `build/` (o caminho completo será `build/jogo`).
    * Tornar o arquivo `build/jogo` executável (devido ao `chmod +x $@`).

3.  **Execute o Jogo:**
    Após a compilação bem-sucedida, o executável estará em `build/jogo`. Para rodar o jogo, a partir do diretório raiz do projeto, execute:
    ```bash
    ./build/jogo 
    ```
    
4.  **Limpar os Arquivos Compilados:**
    Para remover o executável gerado (`build/jogo`), utilize o comando (a partir do diretório raiz):
    ```bash
    make clean
    ```
    Isso executará `rm -f $(TARGET)`, que é `rm -f build/jogo`.
    
## Funções Principais do Jogo

A lógica do jogo é organizada em torno das seguintes funções chave (conforme a documentação fornecida):

* `inicializar_jogador()`: Posiciona o jogador no centro da parte superior da tela no início do jogo.
* `gerar_obstaculo()`: Cria um novo obstáculo e o posiciona na parte inferior da tela para iniciar seu movimento ascendente.
* `gerar_recompensa()`: Cria uma nova recompensa e a posiciona na parte inferior da tela.
* `atualizar_obstaculos()`: Move todos os obstáculos ativos uma linha para cima a cada frame.
* `atualizar_recompensas()`: Move todas as recompensas ativas uma linha para cima a cada frame.
* `verificar_colisao()`: Detecta se a posição atual do jogador coincide com a de algum obstáculo ou recompensa.
* `desenhar_mapa()`: Limpa a tela e redesenha todos os elementos do jogo (jogador, obstáculos, recompensas) e informações de status (HUD), como a pontuação.
* `salvar_rank(char* nickname, int pontuacao)`: Registra o nickname e a pontuação final do jogador no arquivo `ranking.txt`.
* `iniciar_jogo(char* nickname)`: É a função principal que contém o loop do jogo, gerenciando a sequência de eventos, atualizações e renderizações a cada frame.

## Equipe do Projeto

* Manuele Macedo - [@ManueleMacedo](https://github.com/ManueleMacedo)
* Alan Vitor Sobral - [@alancodex](https://github.com/alancodex)
* Andre Felipe Braga - [@ndrfelipe](https://github.com/ndrfelipe)

## Informações da Disciplina

* **Disciplina:** Programação Imperativa e Funcional - 2025.1
* **Instituição de Ensino:** CESAR School
