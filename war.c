#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_TERRITORIOS 5
#define MAX_STRING 30
#define NUM_MISSOES 2

struct Territorio {
    char nome[MAX_STRING];
    char cor[MAX_STRING];
    int numTropas;
};

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

struct Territorio* alocarMapa() {
    struct Territorio* mapa = (struct Territorio*)calloc(NUM_TERRITORIOS, sizeof(struct Territorio));
    if (!mapa) {
        printf("Erro ao alocar memoria!\n");
        exit(1);
    }
    return mapa;
}

// Reads territory data from the user for each territory
void cadastrarTerritorios(struct Territorio* mapa) {
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, MAX_STRING, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0'; // Remove newline

        printf("Cor do Exercito: ");
        fgets(mapa[i].cor, MAX_STRING, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Numero de Tropas: ");
        scanf("%d", &mapa[i].numTropas);
        limparBufferEntrada();
    }
}


void liberarMemoria(struct Territorio* mapa) {
    free(mapa);
}

void exibirMapa(const struct Territorio* mapa) {
    printf("\n=== MAPA ATUAL ===\n");
    printf("%-2s | %-12s | %-10s | %-6s\n", "ID", "Nome", "Cor", "Tropas");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%2d | %-12s | %-10s | %6d\n", i+1, mapa[i].nome, mapa[i].cor, mapa[i].numTropas);
    }
}

void exibirMenuPrincipal() {
    printf("\nMenu:\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

int sortearMissao() {
    return rand() % NUM_MISSOES;
}

void exibirMissao(int idMissao) {
    printf("\n=== SUA MISSAO ===\n");
    if (idMissao == 0)
        printf("Destruir o exercito Verde.\n");
    else
        printf("Conquistar 3 territorios.\n");
}

void simularAtaque(struct Territorio* mapa, int atacante, int defensor) {
    if (atacante == defensor) {
        printf("Nao pode atacar o mesmo territorio!\n");
        return;
    }
    if (mapa[atacante].numTropas < 2) {
        printf("O territorio atacante precisa de pelo menos 2 tropas!\n");
        return;
    }
    if (mapa[defensor].numTropas < 1) {
        printf("O territorio defensor nao possui tropas!\n");
        return;
    }
    int dadoAtq = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtq, dadoDef);
    if (dadoAtq >= dadoDef) {
        mapa[defensor].numTropas--;
        printf("Atacante venceu! Defensor perde 1 tropa.\n");
        if (mapa[defensor].numTropas == 0) {
            printf("Territorio conquistado!\n");
            strncpy(mapa[defensor].cor, mapa[atacante].cor, MAX_STRING);
            mapa[defensor].numTropas = 1;
            mapa[atacante].numTropas--;
        }
    } else {
        mapa[atacante].numTropas--;
        printf("Defensor resistiu! Atacante perde 1 tropa.\n");
    }
}

void faseDeAtaque(struct Territorio* mapa) {
    int atq, def;
    exibirMapa(mapa);
    printf("Escolha o territorio atacante (1-%d): ", NUM_TERRITORIOS);
    scanf("%d", &atq); limparBufferEntrada();
    printf("Escolha o territorio defensor (1-%d): ", NUM_TERRITORIOS);
    scanf("%d", &def); limparBufferEntrada();
    if (atq < 1 || atq > NUM_TERRITORIOS || def < 1 || def > NUM_TERRITORIOS) {
        printf("IDs invalidos!\n");
        return;
    }
    simularAtaque(mapa, atq-1, def-1);
}

int verificarVitoria(const struct Territorio* mapa, int idMissao, const char* corJogador) {
    if (idMissao == 0) {
        // Missao: destruir exercito Verde
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0 && mapa[i].numTropas > 0)
                return 0;
        }
        return 1;
    } else {
        // Missao: conquistar 3 territorios
        int count = 0;
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                count++;
        }
        return count >= 3;
    }
}

int main() {
    srand((unsigned)time(NULL));
    struct Territorio* mapa = alocarMapa();
    cadastrarTerritorios(mapa);

    char corJogador[MAX_STRING] = "Azul";
    int idMissao = sortearMissao();
    int venceu = 0, opcao;

    printf("Bem-vindo ao WAR Estruturado!\nSua cor: %s\n", corJogador);
    exibirMissao(idMissao);

    do {
        exibirMapa(mapa);
        exibirMenuPrincipal();
        scanf("%d", &opcao); limparBufferEntrada();
        switch (opcao) {
            case 1:
                faseDeAtaque(mapa);
                break;
            case 2:
                if (verificarVitoria(mapa, idMissao, corJogador)) {
                    printf("Parabens! Voce cumpriu sua missao!\n");
                    venceu = 1;
                } else {
                    printf("Missao ainda nao cumprida.\n");
                }
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}
