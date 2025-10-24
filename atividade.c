#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* a) Remover espaços no início e no fim (trim in-place) */
void removerEspacos(char *str) {
    if (!str) return;

    /* avança até o primeiro não espaço */
    char *inicio = str;
    while (*inicio && isspace((unsigned char)*inicio)) inicio++;

    /* se havia espaços à esquerda, move o conteúdo */
    if (inicio != str) memmove(str, inicio, strlen(inicio) + 1);

    /* agora remove espaços à direita */
    size_t len = strlen(str);
    if (len == 0) return;

    char *fim = str + len - 1;
    while (fim >= str && isspace((unsigned char)*fim)) fim--;
    *(fim + 1) = '\0';
}

/* b) Preencher vetor de salários usando aritmética de ponteiros */
void preencherSalarios(float *salarios, int tamanho) {
    float *p = salarios;
    for (int i = 0; i < tamanho; i++, p++) {
        float v;
        while (1) {
            printf("Digite o salario do funcionario %d: ", i + 1);
            if (scanf("%f", &v) == 1 && v > 0.0f) {
                *p = v;
                break;
            }
            printf("Valor invalido. Informe um salario positivo (> 0).\n");
            /* limpa entrada inválida */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { /* descarta */ }
        }
    }
}

/* c) Calcular reajustes (indexacao), atualizar vetor e devolver media por ref.
      Retorna o total de reajustes aplicados. */
float calcularReajuste(float *salarios, int tamanho, float *media) {
    float somaReajustes = 0.0f;
    float somaReajustados = 0.0f;

    for (int i = 0; i < tamanho; i++) {
        float s = salarios[i];
        float perc;

        if (s <= 2000.0f)           perc = 0.15f;
        else if (s <= 5000.0f)      perc = 0.10f;
        else if (s <= 10000.0f)     perc = 0.07f;
        else                        perc = 0.05f;

        float acrescimo = s * perc;
        float ajustado  = s + acrescimo;

        salarios[i] = ajustado;          /* atualiza o vetor */
        somaReajustes   += acrescimo;    /* acumula total de reajustes */
        somaReajustados += ajustado;     /* acumula para media */
    }

    if (media) *media = (tamanho > 0) ? (somaReajustados / tamanho) : 0.0f;
    return somaReajustes;
}

int main(void) {
    /* d.1) Parte de strings (a) */
    char linha[256];
    printf("Digite um nome com possiveis espacos nas extremidades:\n");
    /* lê a linha inteira (inclui espaços); remove \n se vier */
    if (fgets(linha, sizeof(linha), stdin)) {
        size_t n = strlen(linha);
        if (n > 0 && linha[n - 1] == '\n') linha[n - 1] = '\0';
        removerEspacos(linha);
        printf("String corrigida: \"%s\"\n\n", linha);
    }

    /* d.2) Parte de salarios (b) e (c) */
    int qtd;
    while (1) {
        printf("Quantidade de funcionarios para reajuste: ");
        if (scanf("%d", &qtd) == 1 && qtd > 0) break;
        printf("Quantidade invalida. Digite um inteiro positivo.\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { /* descarta */ }
    }

    float *salarios = (float *)malloc(qtd * sizeof(float));
    if (!salarios) {
        fprintf(stderr, "Falha ao alocar memoria.\n");
        return 1;
    }

    preencherSalarios(salarios, qtd);

    float mediaApos;
    float totalReajustes = calcularReajuste(salarios, qtd, &mediaApos);

    printf("\nMedia dos salarios apos reajuste: R$ %.2f\n", mediaApos);
    printf("Total gasto com reajustes: R$ %.2f\n", totalReajustes);

    /* exibe salarios ajustados usando aritmetica de ponteiros */
    printf("\nSalarios ajustados:\n");
    float *p = salarios;
    for (int i = 0; i < qtd; i++, p++) {
        printf("Funcionario %d: R$ %.2f\n", i + 1, *p);
    }

    /* libera memoria (ponteiro base ainda esta em 'salarios') */
    free(salarios);
    return 0;
}