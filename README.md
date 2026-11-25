# Pac-Man em C

Jogo de console inspirado no clássico Pac-Man, desenvolvido em linguagem C.
O jogador percorre um labirinto coletando pontos enquanto foge dos fantasmas.

## Funcionalidades
- **5 labirintos** lidos do arquivo `labirintos.txt`, com um sorteado a cada partida
- **Até 3 fantasmas** com comportamento híbrido: a cada passo, 50% de chance de
  perseguir o jogador (menor distância Manhattan) e 50% de andar aleatoriamente
- Fantasmas guardados em estrutura separada da matriz, então **não "comem" os pontos** do mapa
- Contador de **pontos coletados e restantes** em tempo real
- Condições de **vitória, derrota e saída** (tecla `N`)
- Validação de entrada, com mensagens para teclas inválidas e colisão com paredes
- Menu inicial com instruções dos comandos

## Controles
| Tecla | Ação |
|-------|------|
| W | Cima |
| S | Baixo |
| A | Esquerda |
| D | Direita |
| N | Sair |

## Legenda do mapa
| Símbolo | Significado |
|---------|-------------|
| `P` | Jogador |
| `G` | Fantasma |
| `.` | Ponto |
| `#` | Parede |

## Tecnologias
- Linguagem C
- Code::Blocks (IDE) com compilador GCC

## Como rodar

### Pelo Code::Blocks
1. Abra o arquivo `Pacman.c` (ou o projeto `.cbp`, se estiver no repositório)
2. Compile e execute com **F9** (Build and Run)
3. Mantenha o `labirintos.txt` na pasta de execução do programa

### Pelo GCC
```bash
gcc Pacman.c -o pacman
./pacman
```

> O jogo usa `system("cls")` para limpar a tela, então foi pensado para **Windows**.
> Em Linux/macOS, troque por `system("clear")`.

## Formato do `labirintos.txt`
Cada labirinto tem 11 linhas de 21 caracteres, e os 5 labirintos ficam um após o outro
no arquivo, usando os símbolos da legenda acima (e espaço para casas vazias).

## Conceitos praticados
- Matrizes bidimensionais e `struct`
- Passagem de parâmetros por referência (ponteiros)
- Leitura de arquivos (`fopen`, `fgets`)
- Números aleatórios (`rand`, `srand`)
- Modularização em funções e lógica de estados de jogo
