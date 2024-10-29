#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>
#define ARQUIVO_CLIENTES "cliente.txt"
#define TAM 500
int menu();
int aprimoramentos();

void clearbuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

typedef struct
{
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
} Cliente;

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

int ficha_carro()
{

    carregarUsuarios();

    puts(":::: Ficha do Carro ::::\n");
    printf("Modelo: %s \n", cliente[usuario_log].NomeCarro);
    printf("Cavalos: %.3f \n", cliente[usuario_log].cv);
    printf("Torque: %.3f\n", cliente[usuario_log].torque);
    printf("Roda: %d \" \n", cliente[usuario_log].roda);
    printf("Cor: %s\n", cliente[usuario_log].cor);
    printf("Seu saldo: R$ %.2f\n", cliente[usuario_log].reais);

    return 0;
}

void converte_tempo(float tempo_horas)
{
    int minutos = (int)(tempo_horas * 60);
    int segundos = (int)((tempo_horas * 3600) - (minutos * 60));
    printf("%dm %ds\n", minutos, segundos);
}

float simula_corrida()
{
    carregarUsuarios();
    srand(time(NULL));

    float competidorA_velocidadeMedia; // km/h
    float competidorB_velocidadeMedia; // Jogador
    float competidorC_velocidadeMedia; // km/h
    float competidorA_tempo;           // tempo em horas
    float competidorB_tempo;           // tempo em horas
    float competidorC_tempo;           // tempo em horas

    int escolha;
    puts(":::: Circuitos disponiveis ::::\n");
    printf("1. Tiro de meta      (09.00 km)\n");
    printf("2. Circuito regional (30.00 km)\n");
    printf("3. Track Day         (75.00 km)\n");
    printf("4. Copa pistao       (100.0 km)\n");
    printf("Selecione um circuito: ");
    scanf("%d", &escolha);
    printf("\n");

    switch (escolha)
    {
    case 1:
    {
        float distancia = 9.0; // km

        competidorA_velocidadeMedia = (rand() % 45) + 50;
        competidorC_velocidadeMedia = (rand() % 45) + 50;

        competidorA_tempo = distancia / competidorA_velocidadeMedia;
        competidorC_tempo = distancia / competidorC_velocidadeMedia;

        float diamentroRodaCliente = cliente[usuario_log].roda * 0.0254; // polegadas -> metros
        float raioCliente = diamentroRodaCliente / 2;
        float forcaTracao = cliente[usuario_log].torque / raioCliente;
        competidorB_velocidadeMedia = cliente[usuario_log].cv * forcaTracao / 1000; // km/h

        competidorB_tempo = distancia / competidorB_velocidadeMedia;

        puts(":::: Tiro de meta 9km ::::");

        if ((competidorB_tempo < competidorA_tempo) && (competidorB_tempo < competidorC_tempo))
        {
            printf("Resultados da corrida...\n");
            ;
            printf("=> VITORIA!\n");
            if (competidorA_tempo < competidorC_tempo)
            {
                printf(". . . . . . A . . .\n");
            }
            else
            {
                printf(". . A . . . . . . .\n");
            }

            printf(". . . . . . . . B . (Voce)\n");

            if (competidorC_tempo < competidorA_tempo)
            {
                printf(". . . . . . C . . .\n");
            }
            else
            {
                printf(". . C . . . . . . .\n");
            };

            printf("Premiacao de R$ 850.00 depositada na sua conta\n");
            printf("\n");
            cliente[usuario_log].reais += 850;

            char registro_hist[200];
            sprintf(registro_hist, "/VITORIA: Tiro de Meta + R$ 850.00");

            if (strcmp(cliente[usuario_log].historico, "Nenhuma corrida registrada.") == 0)
            {
                strcpy(cliente[usuario_log].historico, registro_hist);
            }
            else
            {
                strcat(cliente[usuario_log].historico, registro_hist);
            }
        }
        else
        {
            printf("Resultados da corrida...\n");
            ;
            printf("=> DERROTA\n");
            if (competidorA_tempo < competidorC_tempo)
            {
                printf(". . . . . . . . A .\n");
            }
            else
            {
                printf(". A . . . . . . . .\n");
            }

            printf(". . . B . . . . . . (Voce)\n");

            if (competidorC_tempo < competidorA_tempo)
            {
                printf(". . . . . . . . C .\n");
            }
            else
            {
                printf(". C . . . . . . . .\n");
            }

            char registro_hist[200];
            sprintf(registro_hist, "/DERROTA: Tiro de Meta");

            if (strcmp(cliente[usuario_log].historico, "Nenhuma corrida registrada.") == 0)
            {
                strcpy(cliente[usuario_log].historico, registro_hist);
            }
            else
            {
                strcat(cliente[usuario_log].historico, registro_hist);
            }

            printf("\n");
        }

        printf("Tempo Corredor A: ");
        converte_tempo(competidorA_tempo);
        printf("Tempo Corredor B: ");
        converte_tempo(competidorB_tempo);
        printf("Tempo Corredor C: ");
        converte_tempo(competidorC_tempo);
        printf("\n");

        break;
    }
    case 2:
    {
        float distancia = 30.0;

        competidorA_velocidadeMedia = (rand() % 85) + 70;
        competidorC_velocidadeMedia = (rand() % 85) + 70;

        competidorA_tempo = distancia / competidorA_velocidadeMedia;
        competidorC_tempo = distancia / competidorC_velocidadeMedia;

        float diamentroRodaCliente = cliente[usuario_log].roda * 0.0254;
        float raioCliente = diamentroRodaCliente / 2;
        float forcaTracao = cliente[usuario_log].torque / raioCliente;
        competidorB_velocidadeMedia = cliente[usuario_log].cv * forcaTracao / 1000;

        competidorB_tempo = distancia / competidorB_velocidadeMedia;

        puts(":::: Circuito Regional 30km ::::");

        if ((competidorB_tempo < competidorA_tempo) && (competidorB_tempo < competidorC_tempo))
        {
            printf("Resultados da corrida...\n");
            ;
            printf("=> VITORIA!\n");
            if (competidorA_tempo < competidorC_tempo)
            {
                printf(". . . . . . A . . .\n");
            }
            else
            {
                printf(". . A . . . . . . .\n");
            }

            printf(". . . . . . . . B . (Voce)\n");

            if (competidorC_tempo < competidorA_tempo)
            {
                printf(". . . . . . C . . .\n");
            }
            else
            {
                printf(". . C . . . . . . .\n");
            };

            printf("Premiacao de R$ 1550.00 depositada na sua conta\n");
            printf("\n");
            cliente[usuario_log].reais += 1550;

            char registro_hist[200];
            sprintf(registro_hist, "/VITORIA: Circuito Regional + R$ 1550.00");

            if (strcmp(cliente[usuario_log].historico, "Nenhuma corrida registrada.") == 0)
            {
                strcpy(cliente[usuario_log].historico, registro_hist);
            }
            else
            {
                strcat(cliente[usuario_log].historico, registro_hist);
            }
        }
        else
        {
            printf("Resultados da corrida...\n");
            ;
            printf("=> DERROTA\n");
            if (competidorA_tempo < competidorC_tempo)
            {
                printf(". . . . . . . . A .\n");
            }
            else
            {
                printf(". A . . . . . . . .\n");
            }

            printf(". . . B . . . . . . (Voce)\n");

            if (competidorC_tempo < competidorA_tempo)
            {
                printf(". . . . . . . . C .\n");
            }
            else
            {
                printf(". C . . . . . . . .\n");
            }

            char registro_hist[200];
            sprintf(registro_hist, "/DERROTA: Circuito Regional");

            if (strcmp(cliente[usuario_log].historico, "Nenhuma corrida registrada.") == 0)
            {
                strcpy(cliente[usuario_log].historico, registro_hist);
            }
            else
            {
                strcat(cliente[usuario_log].historico, registro_hist);
            }

            printf("\n");
        }

        printf("Tempo Corredor A: ");
        converte_tempo(competidorA_tempo);
        printf("Tempo Corredor B: ");
        converte_tempo(competidorB_tempo);
        printf("Tempo Corredor C: ");
        converte_tempo(competidorC_tempo);

        break;
    }
    case 3:
    {
        float distancia = 75.0;

        competidorA_velocidadeMedia = (rand() % 95) + 80;
        competidorC_velocidadeMedia = (rand() % 95) + 80;

        competidorA_tempo = distancia / competidorA_velocidadeMedia;
        competidorC_tempo = distancia / competidorC_velocidadeMedia;

        float diamentroRodaCliente = cliente[usuario_log].roda * 0.0254;
        float raioCliente = diamentroRodaCliente / 2;
        float forcaTracao = cliente[usuario_log].torque / raioCliente;
        competidorB_velocidadeMedia = cliente[usuario_log].cv * forcaTracao / 1000;

        competidorB_tempo = distancia / competidorB_velocidadeMedia;

        puts(":::: Track Day 75km ::::");

        if ((competidorB_tempo < competidorA_tempo) && (competidorB_tempo < competidorC_tempo))
        {
            printf("Resultados da corrida...\n");
            ;
            printf("=> VITORIA!\n");
            if (competidorA_tempo < competidorC_tempo)
            {
                printf(". . . . . . A . . .\n");
            }
            else
            {
                printf(". . A . . . . . . .\n");
            }

            printf(". . . . . . . . B . (Voce)\n");

            if (competidorC_tempo < competidorA_tempo)
            {
                printf(". . . . . . C . . .\n");
            }
            else
            {
                printf(". . C . . . . . . .\n");
            };

            printf("Premiacao de R$ 2300.00 depositada na sua conta\n");
            printf("\n");
            cliente[usuario_log].reais += 2300;

            char registro_hist[200];
            sprintf(registro_hist, "/VITORIA: Track Day + R$ 2300.00");

            if (strcmp(cliente[usuario_log].historico, "Nenhuma corrida registrada.") == 0)
            {
                strcpy(cliente[usuario_log].historico, registro_hist);
            }
            else
            {
                strcat(cliente[usuario_log].historico, registro_hist);
            }
        }
        else
        {
            printf("Resultados da corrida...\n");
            ;
            printf("=> DERROTA\n");
            if (competidorA_tempo < competidorC_tempo)
            {
                printf(". . . . . . . . A .\n");
            }
            else
            {
                printf(". A . . . . . . . .\n");
            }

            printf(". . . B . . . . . . (Voce)\n");

            if (competidorC_tempo < competidorA_tempo)
            {
                printf(". . . . . . . . C .\n");
            }
            else
            {
                printf(". C . . . . . . . .\n");
            }

            char registro_hist[200];
            sprintf(registro_hist, "/DERROTA: Track Day");

            if (strcmp(cliente[usuario_log].historico, "Nenhuma corrida registrada.") == 0)
            {
                strcpy(cliente[usuario_log].historico, registro_hist);
            }
            else
            {
                strcat(cliente[usuario_log].historico, registro_hist);
            }

            printf("\n");
        }

        printf("Tempo Corredor A: ");
        converte_tempo(competidorA_tempo);
        printf("Tempo Corredor B: ");
        converte_tempo(competidorB_tempo);
        printf("Tempo Corredor C: ");
        converte_tempo(competidorC_tempo);

        break;
    }
    case 4:
    {
        float distancia = 100.0;

        competidorA_velocidadeMedia = (rand() % 90) + 90;
        competidorC_velocidadeMedia = (rand() % 90) + 90;

        competidorA_tempo = distancia / competidorA_velocidadeMedia;
        competidorC_tempo = distancia / competidorC_velocidadeMedia;

        float diamentroRodaCliente = cliente[usuario_log].roda * 0.0254;
        float raioCliente = diamentroRodaCliente / 2;
        float forcaTracao = cliente[usuario_log].torque / raioCliente;
        competidorB_velocidadeMedia = cliente[usuario_log].cv * forcaTracao / 1000;

        competidorB_tempo = distancia / competidorB_velocidadeMedia;

        puts(":::: Copa pistão 100km ::::");

        if ((competidorB_tempo < competidorA_tempo) && (competidorB_tempo < competidorC_tempo))
        {
            printf("Resultados da corrida...\n");
            ;
            printf("=> VITORIA!\n");
            if (competidorA_tempo < competidorC_tempo)
            {
                printf(". . . . . . A . . .\n");
            }
            else
            {
                printf(". . A . . . . . . .\n");
            }

            printf(". . . . . . . . B . (Voce)\n");

            if (competidorC_tempo < competidorA_tempo)
            {
                printf(". . . . . . C . . .\n");
            }
            else
            {
                printf(". . C . . . . . . .\n");
            };

            printf("Premiacao de R$ 5000.00 depositada na sua conta\n");
            printf("\n");
            cliente[usuario_log].reais += 5000;

            char registro_hist[200];
            sprintf(registro_hist, "/VITORIA: Copa pistao + R$ 5000.00");

            if (strcmp(cliente[usuario_log].historico, "Nenhuma corrida registrada.") == 0)
            {
                strcpy(cliente[usuario_log].historico, registro_hist);
            }
            else
            {
                strcat(cliente[usuario_log].historico, registro_hist);
            }
        }
        else
        {
            printf("Resultados da corrida...\n");
            ;
            printf("=> DERROTA\n");
            if (competidorA_tempo < competidorC_tempo)
            {
                printf(". . . . . . . . A .\n");
            }
            else
            {
                printf(". A . . . . . . . .\n");
            }

            printf(". . . B . . . . . . (Voce)\n");

            if (competidorC_tempo < competidorA_tempo)
            {
                printf(". . . . . . . . C .\n");
            }
            else
            {
                printf(". C . . . . . . . .\n");
            }

            char registro_hist[200];
            sprintf(registro_hist, "/DERROTA: Copa pistao");

            if (strcmp(cliente[usuario_log].historico, "Nenhuma corrida registrada.") == 0)
            {
                strcpy(cliente[usuario_log].historico, registro_hist);
            }
            else
            {
                strcat(cliente[usuario_log].historico, registro_hist);
            }

            printf("\n");
        }

        printf("Tempo Corredor A: ");
        converte_tempo(competidorA_tempo);
        printf("Tempo Corredor B: ");
        converte_tempo(competidorB_tempo);
        printf("Tempo Corredor C: ");
        converte_tempo(competidorC_tempo);

        break;
    }
    default:
        printf("=> Opcao Invalida!\n");
        break;
    }

    salvarUsuarios();
    return 0;
}

int historico()
{
    carregarUsuarios();
    puts("\n:::: Historico de Corridas ::::\n");

    char *corridas[300];
    int total = 0;
    char *historico_copia = strdup(cliente[usuario_log].historico);
    char *token = strtok(historico_copia, "/");

    while (token != NULL)
    {
        corridas[total] = token;
        total++;
        token = strtok(NULL, "/");
    }

    for (int i = 0; i < total; i++)
    {
        printf("%s\n", corridas[i]);
    }

    free(historico_copia);

    return 0;
}

int alteracoes_hist()
{

    carregarUsuarios();

    puts("\n::::: Historico de alteracoes :::::\n");

    char *alteracoes[300];
    int total = 0;
    char *alteracoes_hist_copia = strdup(cliente[usuario_log].alteracoes);
    char *token = strtok(alteracoes_hist_copia, "/");

    while (token != NULL)
    {
        alteracoes[total] = token;
        total++;
        token = strtok(NULL, "/");
    }

    for (int i = 0; i < total; i++)
    {
        printf("%s\n", alteracoes[i]);
    }

    free(alteracoes_hist_copia);

    return 0;
}

int stage01()
{
    carregarUsuarios();

    int decisao;
    float torqueInicial;
    float torqueFinal;
    float torque;
    float cvInicial;
    float cvFinal;
    float cavalo;

    if (cliente[usuario_log].stage01_realizado == 1)
    {
        printf("O stage 1 ja foi realizado. Nao e possivel realizar novamente\n");
        return 0;
    }

    printf("\n:::: Stage 1 ::::\n");
    printf("Aumenta potencia de torque e cavalos do veiculo em aproximadamente 15%%\n");
    printf("Valor: R$ 3500.00\n");
    printf("Seu saldo atual: R$ %.2f\n", cliente[usuario_log].reais);
    printf("Deseja realizar o aprimoramento?\n");
    printf("1. Sim\n");
    printf("2. Nao\n");
    printf("Digite: ");
    scanf("%d", &decisao);

    switch (decisao)
    {
    case 1:

        if (cliente[usuario_log].reais < 3500.00)
        {
            printf("Saldo insuficiente para realizar o aprimoramento.\n");
            return 0;
        } // para verificar o saldo do usuario

        torqueInicial = cliente[usuario_log].torque;
        cvInicial = cliente[usuario_log].cv;

        cliente[usuario_log].reais -= 3500.00;

        torque = torqueInicial * 0.15;
        torqueFinal = torque + torqueInicial;

        cavalo = cvInicial * 0.15;
        cvFinal = cavalo + cvInicial;

        cliente[usuario_log].torque = torqueFinal;
        cliente[usuario_log].cv = cvFinal;
        cliente[usuario_log].stage01_realizado = 1;
        // marca que já foi realizado o stage

        char registro_hist[200];
        sprintf(registro_hist, "/STAGE 1: Aumento de 15%% em Torque e Cavalos => Torque: %.2f | Cavalos: %.2f", torqueFinal, cvFinal);

        if (strcmp(cliente[usuario_log].alteracoes, "Nenhuma alteracao feita ate o momento.") == 0)
        {
            strcpy(cliente[usuario_log].alteracoes, registro_hist);
        }
        else
        {
            strcat(cliente[usuario_log].alteracoes, registro_hist);
        }

        salvarUsuarios();
        printf("Torque final: %.2f\n", torqueFinal);
        printf("Cavalos finais: %.2f\n", cvFinal);
        break;
    case 2:
        aprimoramentos();
        break;
    default:
        printf("Opcao Invalida");
        break;
    };

    return 0;
}

int stage02()
{
    carregarUsuarios();
    int decisao;
    float torqueInicial;
    float torqueFinal;
    float torque;
    float cvInicial;
    float cvFinal;
    float cavalo;

    if (cliente[usuario_log].stage02_realizado == 1)
    {
        printf("O stage 2 ja foi realizado. Nao e possivel realizar novamente\n"); // verifica se o stage 2 foi realizado
        return 0;
    }

    printf("\n:::: Stage 2 ::::\n");
    printf("Aumenta potencia de torque e cavalos do veiculo em aproximadamente 30%%\n");
    printf("Valor: R$ 6700.00\n");
    printf("Seu saldo atual: R$ %.2f\n", cliente[usuario_log].reais);
    printf("Deseja realizar o aprimoramento?\n");
    printf("1. Sim\n");
    printf("2. Nao\n");
    printf("Digite: ");
    scanf("%d", &decisao);

    switch (decisao)
    {
    case 1:

        if (cliente[usuario_log].reais < 6700.00)
        {
            printf("Saldo insuficiente para realizar o aprimoramento.\n");
            return 0;
        } // verifica o saldo do usuario
        torqueInicial = cliente[usuario_log].torque;
        cvInicial = cliente[usuario_log].cv;

        cliente[usuario_log].reais -= 6700.00;

        torque = torqueInicial * 0.3;
        torqueFinal = torque + torqueInicial;

        cavalo = cvInicial * 0.3;
        cvFinal = cavalo + cvInicial;

        cliente[usuario_log].torque = torqueFinal;
        cliente[usuario_log].cv = cvFinal;
        cliente[usuario_log].stage02_realizado = 1;
        // marca que já foi realizado o stage

        printf("Torque final: %.2f\n", torqueInicial);
        printf("Cavalos finais: %.2f\n", cvFinal);

        char registro_hist[200];
        sprintf(registro_hist, "/STAGE 2: Aumento de 30%% em Torque e Cavalos => Torque: %.2f | Cavalos: %.2f", torqueFinal, cvFinal);

        if (strcmp(cliente[usuario_log].alteracoes, "Nenhuma alteracao feita ate o momento.") == 0)
        {
            strcpy(cliente[usuario_log].alteracoes, registro_hist);
        }
        else
        {
            strcat(cliente[usuario_log].alteracoes, registro_hist);
        }

        break;
    case 2:
        aprimoramentos();
        break;
    default:
        printf("Opcao Invalida");
        break;
    }
    salvarUsuarios();
    return 0;
}

int stage03()
{
    carregarUsuarios();
    int decisao;
    float torqueInicial;
    float torqueFinal;
    float torque;
    float cvInicial;
    float cvFinal;
    float cavalo;

    if (cliente[usuario_log].stage03_realizado == 1)
    {
        printf("O stage 3 ja foi realizado. Nao e possivel realizar novamente\n");
        return 0;
    } // verifica se o stage 3 foi realizado

    printf("\n:::: Stage 3 ::::\n");
    printf("Aumenta potencia de torque e cavalos do veiculo em aproximadamente 35%%\n");
    printf("Valor: R$ 9000.00\n");
    printf("Seu saldo atual: R$ %.2f\n", cliente[usuario_log].reais);
    printf("Deseja realizar o aprimoramento?\n");
    printf("1. Sim\n");
    printf("2. Nao\n");
    printf("Digite: ");
    scanf("%d", &decisao);

    switch (decisao)
    {
    case 1:
        if (cliente[usuario_log].reais < 9000.00)
        {
            printf("Saldo insuficiente para realizar o aprimoramento.\n");
            return 0;
        } // verifica o saldo do usuario
        torqueInicial = cliente[usuario_log].torque;
        cvInicial = cliente[usuario_log].cv;

        cliente[usuario_log].reais -= 9000.00;

        torque = torqueInicial * 0.35;
        torqueFinal = torque + torqueInicial;

        cavalo = cvInicial * 0.35;
        cvFinal = cvInicial + cavalo;

        cliente[usuario_log].torque = torqueFinal;
        cliente[usuario_log].cv = cvFinal;
        cliente[usuario_log].stage03_realizado = 1;
        // marca que já foi realizado o stage

        printf("Torque final: %.2f\n", torqueFinal);
        printf("Cavalos finais: %.2f\n", cvFinal);

        char registro_hist[200];
        sprintf(registro_hist, "/STAGE 3: Aumento de 35%% em Torque e Cavalos => Torque: %.2f | Cavalos: %.2f", torqueFinal, cvFinal);

        if (strcmp(cliente[usuario_log].alteracoes, "Nenhuma alteracao feita ate o momento.") == 0)
        {
            strcpy(cliente[usuario_log].alteracoes, registro_hist);
        }
        else
        {
            strcat(cliente[usuario_log].alteracoes, registro_hist);
        }

        break;
    case 2:
        aprimoramentos();
        break;
    default:
        printf("opcao invalida");
        break;
    }

    salvarUsuarios();
    return 0;
}

int stage04()
{
    carregarUsuarios();
    int decisao;
    float torqueInicial;
    float torqueFinal;
    float torque;
    float cvInicial;
    float cvFinal;
    float cavalo;

    if (cliente[usuario_log].stage04_realizado == 1)
    {
        printf("O stage 4 ja foi realizado. Nao e possivel realizar novamente\n");
        return 0;
    } // verifica se o stage 4 ja foi realizado

    printf("\n:::: Stage 4 ::::\n");
    printf("Aumenta potencia de torque e cavalos do veiculo em aproximadamente 40%%\n");
    printf("Valor: R$ 13000.00\n");
    printf("Seu saldo atual: R$ %.2f\n", cliente[usuario_log].reais);
    printf("Deseja realizar o aprimoramento?\n");
    printf("1. Sim\n");
    printf("2. Nao\n");
    printf("Digite: ");
    scanf("%d", &decisao);

    switch (decisao)
    {
    case 1:
        if (cliente[usuario_log].reais < 13000.00)
        {
            printf("Saldo insuficiente para realizar o aprimoramento.\n");
            return 0;
        } // verifica o saldo do usuario
        torqueInicial = cliente[usuario_log].torque;
        cvInicial = cliente[usuario_log].cv;

        cliente[usuario_log].reais -= 13000.00;

        torque = torqueInicial * 0.4;
        torqueFinal = torque + torqueInicial;

        cavalo = cvInicial * 0.4;
        cvFinal = cavalo + cvInicial;

        cliente[usuario_log].torque = torqueFinal;
        cliente[usuario_log].cv = cvFinal;
        cliente[usuario_log].stage04_realizado = 1;
        // marca que já foi realizado o stage
        salvarUsuarios();
        printf("Torque final: %.2f\n", torqueFinal);
        printf("Cavalos finais: %.2f\n", cvFinal);

        char registro_hist[200];
        sprintf(registro_hist, "/STAGE 4: Aumento de 40%% em Torque e Cavalos => Torque: %.2f | Cavalos: %.2f", torqueFinal, cvFinal);

        if (strcmp(cliente[usuario_log].alteracoes, "Nenhuma alteracao feita ate o momento.") == 0)
        {
            strcpy(cliente[usuario_log].alteracoes, registro_hist);
        }
        else
        {
            strcat(cliente[usuario_log].alteracoes, registro_hist);
        }

        break;
    case 2:
        aprimoramentos();
        break;
    default:
        printf("opcao invalida");
        break;
    }

    salvarUsuarios();
    return 0;
}

int Cmp_rodas()
{
    carregarUsuarios();
    int decisao;
    char registro_hist[200];
    int pneu_antigo = cliente[usuario_log].roda;
    printf("\n:::: Trocar Pneus ::::\n");
    printf("1. 20\" (R$ 5200.00) \n");
    printf("2. 21\" (R$ 6400.00) \n");
    printf("3. 22\" (R$ 7500.00) \n");
    printf("4. 23\" (R$ 8600.00) \n");
    printf("0. Voltar\n");
    printf("Seu saldo atual: R$ %.2f\n", cliente[usuario_log].reais);
    printf("Escolha um conjunto de rodas: ");
    scanf("%d", &decisao);

    switch (decisao)
    {
    case 1:

        if (cliente[usuario_log].reais < 5200.00)
        {
            printf("Saldo insuficiente para realizar o aprimoramento.\n");
            return 0;
        }
        if (cliente[usuario_log].roda == 20)
        {
            printf("Essa são suas rodas atuais, escolha outra opcao\n");
            return 0;
        }

        cliente[usuario_log].reais -= 5200;
        cliente[usuario_log].roda = 20;
        printf("=> Seu novo pneu tem 20\"!");

        sprintf(registro_hist, "/PNEU: %d\" => Novo 20\" ", pneu_antigo);

        if (strcmp(cliente[usuario_log].alteracoes, "Nenhuma alteracao feita ate o momento.") == 0)
        {
            strcpy(cliente[usuario_log].alteracoes, registro_hist);
        }
        else
        {
            strcat(cliente[usuario_log].alteracoes, registro_hist);
        }

        break;
    case 2:

        if (cliente[usuario_log].reais < 6400.00)
        {
            printf("Saldo insuficiente para realizar o aprimoramento.\n");
            return 0;
        }
        if (cliente[usuario_log].roda == 21)
        {
            printf("Essa são suas rodas atuais, escolha outra opcao\n");
            return 0;
        }

        cliente[usuario_log].reais -= 6400.00;
        cliente[usuario_log].roda = 21;
        printf("=> Seu novo pneu tem 21\"!");

        sprintf(registro_hist, "/PNEU: %d\" => Novo 21\" ", pneu_antigo);

        if (strcmp(cliente[usuario_log].alteracoes, "Nenhuma alteracao feita ate o momento.") == 0)
        {
            strcpy(cliente[usuario_log].alteracoes, registro_hist);
        }
        else
        {
            strcat(cliente[usuario_log].alteracoes, registro_hist);
        }

        break;
    case 3:

        if (cliente[usuario_log].reais < 7500.00)
        {
            printf("Saldo insuficiente para realizar o aprimoramento.\n");
            return 0;
        }
        if (cliente[usuario_log].roda == 22)
        {
            printf("Essa são suas rodas atuais, escolha outra opcao\n");
            return 0;
        }

        cliente[usuario_log].reais -= 7500.00;
        cliente[usuario_log].roda = 22;
        printf("=> Seu novo pneu tem 22\"!");

        sprintf(registro_hist, "/PNEU: %d\" => Novo 22\" ", pneu_antigo);

        if (strcmp(cliente[usuario_log].alteracoes, "Nenhuma alteracao feita ate o momento.") == 0)
        {
            strcpy(cliente[usuario_log].alteracoes, registro_hist);
        }
        else
        {
            strcat(cliente[usuario_log].alteracoes, registro_hist);
        }

        break;
    case 4:

        if (cliente[usuario_log].reais < 8600.00)
        {
            printf("Saldo insuficiente para realizar o aprimoramento.\n");
            return 0;
        }
        if (cliente[usuario_log].roda == 23)
        {
            printf("Essa são suas rodas atuais, escolha outra opcao\n");
            return 0;
        }

        cliente[usuario_log].reais -= 8600.00;
        cliente[usuario_log].roda = 23;
        printf("=> Seu novo pneu tem 23\"!");

        sprintf(registro_hist, "/PNEU: %d\" => Novo 23\" ", pneu_antigo);

        if (strcmp(cliente[usuario_log].alteracoes, "Nenhuma alteracao feita ate o momento.") == 0)
        {
            strcpy(cliente[usuario_log].alteracoes, registro_hist);
        }
        else
        {
            strcat(cliente[usuario_log].alteracoes, registro_hist);
        }

        break;
    case 0:
        aprimoramentos();
        break;
    default:
        printf("=> Opcao invalida!");
        break;
    }
    salvarUsuarios();
    return 0;
}

int aprimoramentos()
{

    int opcao;
    printf(":::: Aprimoramentos ::::\n");
    printf("1. Stage 1\n");
    printf("2. Stage 2\n");
    printf("3. Stage 3\n");
    printf("4. Stage 4\n");
    printf("5. Rodas\n");
    printf("0. Voltar\n");

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
        stage01();
        break;
    case 2:
        stage02();
        break;
    case 3:
        stage03();
        break;
    case 4:
        stage04();
        break;
    case 5:
        Cmp_rodas();
        break;
    case 0:
        menu();
        break;
    default:
        break;
    }
    return 0;
}

int menu()
{

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
        aprimoramentos();
        break;
    case 3:
        simula_corrida();
        break;
    case 4:
        historico();
        break;
    case 5:
        alteracoes_hist();
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