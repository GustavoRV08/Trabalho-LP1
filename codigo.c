#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ESTOQUE "estoque.txt"

typedef struct {
    int codigo;
    char nome[50];
    int qtd;
    float preco;
} Produto;

//vai abrir o arquivo, ler os campos do tad e adicionar ao arq
int cadastro_produto() {
    Produto p;
    FILE *file = fopen(ESTOQUE, "ab");

    if (file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return -1;
    }

    //lê os dados do produto
    printf("Código do produto: ");
    scanf("%d", &p.codigo);
    getchar();
    printf("Nome do produto: ");
    scanf("%[^\n]", p.nome);
    printf("Quantidade: ");
    scanf("%d", &p.qtd);
    printf("Preço do produto: ");
    scanf("%f", &p.preco);

    //inserção do produto no arquivo
    fwrite(&p, sizeof(Produto), 1, file);
    printf("Produto adicionado!\n");

    fclose(file);
    return 0;
}

//vai ler o cod a qnt e adicionar ao arq
int cadastro_compras() {
    Produto p;
    FILE *file = fopen(ESTOQUE, "r+b"); //abre o arq em leitura e escrita

    if(file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return -1;
    }

    int cod; //insere o cod do produto
    int count; //quantidade comprada

    printf("Código do produto comprado: ");
    scanf("%d", &cod);
    printf("Quantidade de produtos comprados: ");
    scanf("%d", &count);

    while(fread(&p, sizeof(Produto), 1, file) == 1) {  //lendo um arq por vez
        if(p.codigo == cod) {
            p.qtd += count; //atualiza a qtd

            fseek(file, -sizeof(Produto), SEEK_CUR);  //retorna ao inicio do produto para sobrescrever a quantidade
            fwrite(&p, sizeof(Produto), 1, file);  //grava os novos dados
            printf("Produto atualizado!\n");
            break;
        }
    }

    fclose(file);
    return 0;
}

//vai ler o cod e qnt de itens que irão sair
int cadastro_vendas() {
    Produto p;
    FILE *file = fopen(ESTOQUE, "r+b"); //abre em leitura e escrita

    if(file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return -1;
    }

    int cod;
    int count;
    float total=0;

    while(cod!=-1){
        printf("Código do produto vendido (Digite -1 para finalizar): ");
        scanf("%d", &cod);
        if(cod!=-1){
            printf("Quantidade do produto a ser vendido: ");
            scanf("%d", &count);
            while (fread(&p, sizeof(Produto), 1, file) == 1) {
                if(p.codigo == cod) {
                    if(p.qtd >= count) { //verifica se tem produtos suficientes
                        p.qtd -= count; //atualiza a qtd
        
                        fseek(file, -sizeof(Produto), SEEK_CUR);
                        fwrite(&p, sizeof(Produto), 1, file);
                        printf("Produto atualizado!\n");
                        total+=(p.preco*count);
                        break;
                    }
                }
            }
        }
    }
    printf("Total da venda: R$ %.2f\n", total);
    fclose(file);
    return 0;
}

void listar_produtos() {
    Produto p;
    FILE *file = fopen(ESTOQUE, "rb");

    if(file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    printf("\n-------------------------ESTOQUE-------------------------\n");
    printf("Código | Nome | Quantidade\n");
    while(fread(&p, sizeof(Produto), 1, file)) {
        printf("%d | %s | %d | R$ %.2lf\n", p.codigo, p.nome, p.qtd, p.preco);
    }
    printf("---------------------------------------------------------\n");

    fclose(file);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    do {
        printf("\n______CONTROLE DE ESTOQUE____\n");
        printf("1. Adicionar produto.\n");
        printf("2. Listar produtos.\n");
        printf("3. Cadastrar venda de produtos.\n");
        printf("4. Cadastrar reposicao de produtos.\n");
        printf("5. Apagar estoque.\n");
        printf("6. Sair do programa.\n");
        printf("Digite o numero da operação desejada: ");
        scanf("%d", &opcao);
        printf("________________\n\n");
        

        switch (opcao) {
            case 1:
                cadastro_produto();
                break;
            case 2:
                listar_produtos();
                break;
            case 3:
                cadastro_vendas();
                break;
            case 4:
                cadastro_compras();
                break;
            case 5:
                remove(ESTOQUE);
                break;
            default:
                break;
            }
    } while (opcao != 6);

    return 0;
}