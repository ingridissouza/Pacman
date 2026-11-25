#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define LINHAS 11
#define COLUNAS 21
#define QTD_LABIRINTOS 5
#define MAX_FANTASMAS 3

//guarda a posicao de todos os fantasmas (eles ficam fora da matriz,
//assim nunca alteram os pontos do labirinto)
typedef struct {
    int l[MAX_FANTASMAS]; // são dois vetores de 3 posições, já que MAX_FANTASMAS vale 3.
    int c[MAX_FANTASMAS];
    int qtd;
} Fantasmas;

int contar_pontos(char labirinto[][COLUNAS]) {
    int l, c, total = 0;

    for (l = 0; l < LINHAS; l++) {
        for (c = 0; c < COLUNAS; c++) {
            if (labirinto[l][c] == '.') {
                total++;
            }
        }
    }

    return total;
}

//sorteia um labirinto do arquivo e guarda na matriz
int carregar_labirinto(char labirinto[][COLUNAS], int *JogL, int *JogC, Fantasmas *fan) {
    char linha[COLUNAS + 3];
    FILE *lab;
    int l, c, i;
    int sorteado = rand() % QTD_LABIRINTOS;

    fan->qtd = 0;

    lab = fopen("labirintos.txt", "r");
    if (lab == NULL) {
        return 0;
    }

    //pula os labirintos anteriores ao sorteado
    for (i = 0; i < sorteado * LINHAS; i++) {
        fgets(linha, sizeof(linha), lab);
    }

    //le o labirinto sorteado
    for (l = 0; l < LINHAS; l++) {
        fgets(linha, sizeof(linha), lab);

        for (c = 0; c < COLUNAS; c++) {
            labirinto[l][c] = linha[c];

            //guarda onde o jogador comeca e deixa o espaco livre
            if (linha[c] == 'P') {
                *JogL = l;
                *JogC = c;
                labirinto[l][c] = ' ';
            }
            //guarda onde cada fantasma comeca e deixa o espaco livre
            else if (linha[c] == 'G') {
                //so os 3 primeiros G viram fantasmas; os demais viram espaco livre
                if (fan->qtd < MAX_FANTASMAS) { // só impede de gravar além do tamanho da lista
                    fan->l[fan->qtd] = l;
                    fan->c[fan->qtd] = c;
                    fan->qtd++;
                }
                labirinto[l][c] = ' ';
            }
        }
    }

    fclose(lab);
    return 1;
}

//retorna o indice do fantasma na posicao (l, c) ou -1 se nao houver
int fantasma_em(Fantasmas *fan, int l, int c) {
    int i;

    for (i = 0; i < fan->qtd; i++) {
        if (fan->l[i] == l && fan->c[i] == c) {
            return i;
        }
    }

    return -1;
}

void matriz_visor(char labirinto[][COLUNAS], int JogL, int JogC, Fantasmas *fan, int PC, int PR, char MsgErro[]) {
    int l, c;

    system("cls");

    for (l = 0; l < LINHAS; l++) {
        for (c = 0; c < COLUNAS; c++) {
            if (l == JogL && c == JogC) {
                printf("P");
            } else if (fantasma_em(fan, l, c) != -1) {
                printf("G");   //o fantasma so e desenhado por cima, o ponto continua la
            } else {
                printf("%c", labirinto[l][c]);
            }
        }
        printf("\n");
    }

    printf("\nPontos Coletados: %d | Pontos Restantes: %d\n", PC, PR);

    if (MsgErro[0] != '\0') {
        printf("\n%s\n", MsgErro);
    }
}

//move cada fantasma 1 casa. Retorna 1 se algum fantasma alcancou o jogador
int mover_fantasmas(char labirinto[][COLUNAS], Fantasmas *fan, int JogL, int JogC) {
    int dl[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    int i, d;

    for (i = 0; i < fan->qtd; i++) {
        int candL[4], candC[4], nCand = 0;
        int escolhido;

        //lista os movimentos possiveis (sem parede, sem sair do mapa, sem outro fantasma)
        for (d = 0; d < 4; d++) {
            int nl = fan->l[i] + dl[d];
            int nc = fan->c[i] + dc[d];

            if (nl < 0 || nl >= LINHAS || nc < 0 || nc >= COLUNAS) continue;
            if (labirinto[nl][nc] == '#') continue;
            if (fantasma_em(fan, nl, nc) != -1) continue;

            candL[nCand] = nl;
            candC[nCand] = nc;
            nCand++;
        }

        if (nCand == 0) continue;   //fantasma preso: fica parado

        if (rand() % 2 == 0) {
            //50%: persegue o jogador (escolhe a casa mais proxima dele)
            int melhor = 0, melhorDist = 1000;
            for (d = 0; d < nCand; d++) {
                int dist = abs(candL[d] - JogL) + abs(candC[d] - JogC);
                if (dist < melhorDist) {
                    melhorDist = dist;
                    melhor = d;
                }
            }
            escolhido = melhor;
        } else {
            //50%: anda aleatoriamente
            escolhido = rand() % nCand;
        }

        //o fantasma apenas troca de posicao: NAO mexe na matriz, entao nao come pontos
        fan->l[i] = candL[escolhido];
        fan->c[i] = candC[escolhido];

        if (fan->l[i] == JogL && fan->c[i] == JogC) {
            return 1;
        }
    }

    return 0;
}

void movimento(char labirinto[][COLUNAS], int *JogL, int *JogC, Fantasmas *fan, int *PC, int *PR, int *fim, char MsgErro[]) {
    char Mov;
    int NovaL = *JogL, NovaC = *JogC;

    MsgErro[0] = '\0';

    printf("\nPróxima jogada (W-cima, S-baixo, A-esquerda, D-direita, N-sair): ");
    scanf(" %c", &Mov);

    switch (Mov) {
        case 'W': case 'w': NovaL--; break;
        case 'S': case 's': NovaL++; break;
        case 'A': case 'a': NovaC--; break;
        case 'D': case 'd': NovaC++; break;
        case 'N': case 'n': *fim = 3; return;
        default:  strcpy(MsgErro, "QUE TECLA E ESSA? O Pac-Man só entende W, A, S e D!");
    }

    if (MsgErro[0] != '\0') {
        return;
    }

    if (NovaL < 0 || NovaL >= LINHAS || NovaC < 0 || NovaC >= COLUNAS || labirinto[NovaL][NovaC] == '#') {
        strcpy(MsgErro, "TEM UMA PAREDE AI!! O Pac-Man não atravessa paredes (ainda).");
        return;
    }

    *JogL = NovaL;
    *JogC = NovaC;

    //somente o jogador coleta pontos
    if (labirinto[NovaL][NovaC] == '.') {
        labirinto[NovaL][NovaC] = ' ';
        (*PC)++;
        (*PR)--;
    }

    //jogador andou para cima de um fantasma
    if (fantasma_em(fan, NovaL, NovaC) != -1) {
        *fim = 2;
        return;
    }

    //venceu antes dos fantasmas se mexerem
    if (*PR == 0) {
        *fim = 1;
        return;
    }

    //vez dos fantasmas
    if (mover_fantasmas(labirinto, fan, *JogL, *JogC)) {
        *fim = 2;
    }
}

void iniciar_jogo() {
    char labirinto[LINHAS][COLUNAS];
    char MsgErro[100];
    Fantasmas fan;
    int JogL, JogC;      //posicao do jogador
    int PC = 0, PR, PT;  //pontos coletados, restantes e total do labirinto
    int fim = 0;         //0 = jogando, 1 = venceu, 2 = perdeu, 3 = saiu

    MsgErro[0] = '\0';

    if (!carregar_labirinto(labirinto, &JogL, &JogC, &fan)) {
        printf("\nErro: não foi possivel abrir o arquivo labirintos.txt!\n");
        return;
    }
    PR = contar_pontos(labirinto);
    PT = PR;

    do {
        matriz_visor(labirinto, JogL, JogC, &fan, PC, PR, MsgErro);
        movimento(labirinto, &JogL, &JogC, &fan, &PC, &PR, &fim, MsgErro);
    } while (fim == 0);

    matriz_visor(labirinto, JogL, JogC, &fan, PC, PR, MsgErro);

    if (fim == 1) {
        printf("\nARRASOU! VOCÊ ZEROU O LABIRINTO E HUMILHOU OS FANTASMAS!\n");
    } else if (fim == 2) {
        printf("\nGAME OVER!!! O RH dos fantasmas agradece a sua colaboração!\n");
    } else {
        printf("\nJOGO ENCERRADO!\nQue pena! A coragem ficou para a próxima partida.\n\n");
    }

    printf("\n\n========================================\n");
    printf("PONTUAÇÃO TOTAL: %d de %d pontos\n", PC, PT);
    printf("========================================\n\n");
}

int exibir_menu() {
    char iniciar;

    printf("\n========================================\n");
    printf("         BEM-VINDO(A) AO PAC-MAN\n");
    printf("========================================\n");
    printf("COMANDOS:\n");
    printf("[W] Cima     | [S] Baixo\n");
    printf("[A] Esquerda | [D] Direita\n");
    printf("[N] Sair do jogo\n");
    printf("----------------------------------------\n");
    printf("OBJETIVO:\n");
    printf("Coletar todos os pontos sem ser alcancado\n");
    printf("pelos fantasmas.\n");
    printf("\nVocê é o P e deve fugir dos fantasmas (G).\n");
    printf("========================================\n");

    do {
        printf("\nDeseja iniciar o jogo? [S/N]: ");
        scanf(" %c", &iniciar);

        if (iniciar == 'S' || iniciar == 's') {
            return 1;
        } else if (iniciar == 'N' || iniciar == 'n') {
            printf("\nJOGO ENCERRADO!\nQue pena! A coragem ficou para a próxima partida.\n");
            return 0;
        } else {
            printf("Opção inválida! Digite S ou N.\n");
        }

    } while (1);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    if (exibir_menu() == 1) {
        iniciar_jogo();
    }

    return 0;
}
