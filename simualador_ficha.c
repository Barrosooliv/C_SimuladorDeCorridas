#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>
#define ARQUIVO_CLIENTES "cliente.txt"
#define TAM 500
int menu();

void clearbuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

typedef struct {
    char Nome[100];
    char NomeCarro[100];
    char Senha[100];
    float cv;
    float torque;
    char cor[100];
    int roda;
    float reais;
    int stage01_realizado;
    int stage02_realizado; 
    int stage03_realizado; 
    int stage04_realizado;
    char historico[5000]; 
    char alteracoes[5000]; 
}Cliente;

Cliente *cliente = NULL;
int num_usuarios;
int usuario_log;

int salvarUsuarios()
{
    FILE *arquivo = fopen(ARQUIVO_CLIENTES, "wb");
    if (arquivo == NULL)
    {
        return -1;
    }

    fwrite(&num_usuarios, sizeof(int), 1, arquivo);

    for (int i = 0; i < num_usuarios; i++)
    {
        fwrite(&cliente[i], sizeof(Cliente), 1, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int carregarUsuarios()
{
    FILE *arquivo = fopen(ARQUIVO_CLIENTES, "rb");
    if (arquivo == NULL)
    {
        return -1;
    }

    fread(&num_usuarios, sizeof(int), 1, arquivo);

    cliente = malloc(num_usuarios * sizeof(Cliente));

    for (int i = 0; i < num_usuarios; i++)
    {
        fread(&cliente[i], sizeof(Cliente), 1, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int cadastro()      
{
    int Nomejaexiste = 0;
    cliente = realloc(cliente, (num_usuarios + 1) * sizeof(Cliente));
    printf("\n:::: Cadastro ::::\n");
    printf("Digite seu Nome: ");
    fgets(cliente[num_usuarios].Nome, TAM, stdin);
    cliente[num_usuarios].Nome[strcspn(cliente[num_usuarios].Nome, "\n")] = '\0';

    for (int i = 0; i < num_usuarios; i++)
    {
        if (strcmp(cliente[i].Nome, cliente[num_usuarios].Nome) == 0)
        {
            Nomejaexiste = 1;
            break;
        }
    }

    while (Nomejaexiste == 1)
    {
        printf("=> Este Nome ja esta cadastrado!\n");
        printf("Digite novamente seu Nome: ");
        fgets(cliente[num_usuarios].Nome, TAM, stdin);
        cliente[num_usuarios].Nome[strcspn(cliente[num_usuarios].Nome, "\n")] = '\0';

        for (int i = 0; i < num_usuarios; i++)
        {
            if (strcmp(cliente[i].Nome, cliente[num_usuarios].Nome) == 0)
            {
                Nomejaexiste = 1;
                break;
            }
            else
            {
                Nomejaexiste = 0;
            }
        }
    }

    printf("Digite a senha: ");
    fgets(cliente[num_usuarios].Senha, TAM, stdin);
    while (strlen(cliente[num_usuarios].Senha) < 6)
    {
        printf("Digite uma senha superior a 6 digitos ou letras!\n");
        printf("Digite novamente sua senha: ");
        fgets(cliente[num_usuarios].Senha, TAM, stdin);
    }
    cliente[num_usuarios].Senha[strcspn(cliente[num_usuarios].Senha, "\n")] = '\0';

    strcpy(cliente[num_usuarios].NomeCarro, "civic type R");
    cliente[num_usuarios].cv = 400;
    cliente[num_usuarios].torque = 45;
    strcpy(cliente[num_usuarios].cor, "Cinza");
    cliente[num_usuarios].roda = 19;
    cliente[num_usuarios].reais = 0;
    strcpy(cliente[num_usuarios].historico, "Nenhuma corrida registrada.");
    strcpy(cliente[num_usuarios].alteracoes, "Nenhuma alteracao feita ate o momento.");

    num_usuarios++;
    printf("=> Usuario cadastrado com sucesso!\n");

    salvarUsuarios();
    return 0;
}

int login()
{
    carregarUsuarios();
    char Nome[100];
    char senha[100];
    int opcao;
    int encontrado = 0;

    while (encontrado == 0 || opcao == 0)
    {
        printf("\n::::: Bem vindo(a) a Los Santos Customs :::::\n");
        printf("Gostaria de realizar login ou cadastro?\n");
        printf("1. Cadastrar Usuario\n");
        printf("2. Login\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        clearbuffer();

        switch (opcao)
        {
        case 1:
            cadastro();
            break;
        case 2:
            printf("Digite seu Nome: ");
            fgets(Nome, TAM, stdin);
            Nome[strcspn(Nome, "\n")] = '\0';

            printf("Digite a senha: ");
            fgets(senha, TAM, stdin);
            senha[strcspn(senha, "\n")] = '\0';

            for (int i = 0; i < num_usuarios; i++)
            {
                if (strcmp(cliente[i].Nome, Nome) == 0 && strcmp(cliente[i].Senha, senha) == 0)
                {
                    printf("\nLogin realizado com sucesso! Bem-vindo(a), %s!\n", cliente[i].Nome);
                    encontrado = 1;
                    usuario_log = i;
                    break;
                }
            }

            if (!encontrado)
            {
                printf("\nNome ou senha incorretos.\n");
            }
            break;

        case 0:
            printf("Encerrando o programa...\n");
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Opcao invalida.\n");
            break;
        }
    }

    return 0;
}

int ficha_carro(){

    carregarUsuarios();

    puts(":::: Ficha do Carro ::::\n");
    printf("Modelo: %s \n", cliente[usuario_log].NomeCarro);
    printf("Cavalos: %.3f \n",cliente[usuario_log].cv);
    printf("Torque: %.3f\n", cliente[usuario_log].torque);
    printf("Roda: %d \" \n", cliente[usuario_log].roda);
    printf("Cor: %s\n",cliente[usuario_log].cor);
    printf("Seu saldo: R$ %.2f\n",cliente[usuario_log].reais);

    return 0;
}

int menu(){

    int opcao;
    printf(":::: Los Santos Customs ::::\n");
    printf("1. Ficha do Carro \n");
    printf("2. Aprimoramentos \n");
    printf("3. Corridas \n");
    printf("4. Historico \n");
    printf("5. Alteracoes Feitas\n");
    printf("0. Sair\n");

    printf("Eu desejo... ");
    scanf("%d", &opcao);
    clearbuffer();
    
    while (opcao != 1 && opcao != 2 && opcao != 3 && opcao != 4 && opcao != 5 && opcao != 0)
    {
        printf("Valor invalido, insira novamente: ");
        scanf("%d", &opcao);
        clearbuffer();
    }

    switch (opcao)
    {
        case 1:
            ficha_carro(); 
            break;
        case 2:
            //aprimoramentos(); Implementar
            break;
        case 3:
            //simula_corrida(); Implementar
            break;
        case 4:
            //historico(); Implementar
            break;
        case 5:
            //alteracoes_hist(); Implementar
            break;
        case 0:
            login();
            break;
        default:
            printf("Opcao Invalida\n");
            break;
    }
    return 0;
}

int main(void)
{
    carregarUsuarios();

    login();

    while (1)
    {
        menu();
    }

    free(cliente);
    return 0;
}