# Documentação do Simulador de Corridas

**Aluno**: Daniel Barroso de Oliveira  
**R.A**: 22.124.054-2  

**Aluno**: Hendrick Gonçalves Camporese Burielo  
**R.A**: 22.124.038-5  

**Aluno**: Vinicius Alves Gimenez  
**R.A**: 22.124.088-0  

## 1. Objetivo 🎯

O projeto desenvolvido faz a simulação de corridas entre carros dentro de circuitos específicos, gerando o resultado da partida em console, apontando se o cliente perdeu ou ganhou. Para aumentar suas chances de vitória, o cliente poderá realizar aprimoramentos em seu carro na **Los Santos Customs**, com o dinheiro proveniente de corridas anteriores. 

## 2. Forma de Compilação 👨‍💻

O comando `gcc -o main main.c` é usado para compilar um programa em C com o GCC (GNU Compiler Collection), que é o compilador em si. Ao utilizar o comando `gcc`, você está chamando o compilador para processar arquivos de código-fonte C e transformá-los em um programa executável. O argumento `-o main` especifica o nome do arquivo de saída; `-o` significa "output", indicando que o nome do arquivo gerado deve ser o que segue o argumento, neste caso, `main`. Sem o argumento `-o`, o GCC geraria um arquivo executável com o nome padrão `a.out` no Linux/macOS ou `a.exe` no Windows. Com o `-o main`, o arquivo executável é nomeado `main` (ou `main.exe` no Windows). O arquivo `main.c` é o código-fonte escrito em C que está sendo compilado. O GCC processa esse arquivo e, após a compilação, cria o executável. Portanto, o GCC lê o arquivo `main.c`, compila o código contido nele e gera um executável com o nome `main` em vez de usar o nome padrão. Depois disso, é possível executar o arquivo diretamente no terminal, como `.\main.exe` no Windows ou `./main` no Linux/macOS.

## 3. Funcionamento 🧩

### `salvarUsuarios()`

Essa função manipula o arquivo binário para armazenar as informações fornecidas pelo usuário, garantindo que os dados persistam entre as execuções do programa. Ela começa abrindo o arquivo `“usuarios.txt”` em modo de escrita binária `(wb)`, onde escreve a quantidade atual de usuários, seguida pelos dados de cada um, armazenados na estrutura Cliente. Essa função é acionada sempre que um novo usuário é cadastrado ou que há atualização nas informações de um usuário existente.

### `carregarUsuarios()`

Enquanto salvarUsuarios() grava os dados, carregarUsuarios() é responsável por carregá-los. Ela abre o arquivo `“cliente.txt”` em modo de leitura binária `(rb)`. Caso o arquivo exista, a função lê o número total de usuários e, em seguida, seus dados, para que possam ser consultados ou manipulados por outras funções.

### `cadastro()`

Essa função é chamada toda vez que o usuário deseja abrir uma nova conta. O cadastro pede um nome e uma senha de comprimento superior a 6 letras ou caracteres, sendo que o nome deve ser único, caso contrário, não é possível cadastrar esse cliente. Aqui são atribuídos valores iniciais do carro do usuário, essas informações são armazenadas em um Struct chamado ”Cliente”, que então tem suas informações salvas no arquivo binário.

### `login()`

O login valida o Nome e a senha do usuário, permitindo que ele acesse suas informações e realize corridas e aprimoramentos em seu veículo. Para isso, um loop percorre o struct Cliente, verificando se os dados informados são iguais aos cadastrados anteriormente. Caso as credenciais correspondam às de um usuário registrado, a função exibe uma mensagem de sucesso e salva o índice do usuário logado na variável `usuario_log`, essencial para consultas futuras. A comparação só é possível graças ao carregamento dos clientes no início da função. A função também exibe um pequeno menu, onde é possível escolher entre realizar o login, cadastrar um usuário (chamando a função `cadastro()` ) ou sair.

### `ficha_carro()`

A função `ficha_carro()` exibe um resumo com os dados do carro do cliente atualmente logado no sistema. Ela apresenta informações como o modelo, a potência em cavalos, o torque, o tamanho das rodas (em polegadas) e a cor do veículo. A leitura desses dados é possível através do carregamento de usuários, onde buscamos informações do cliente logado.

### `simula_corrida()`

Nessa função, são apresentadas ao usuário quatro opções de corrida das quais ele pode optar participar, onde cada corrida possui uma distância diferente a ser percorrida, oferecendo premiação em dinheiro, que pode ser investido para melhor performance do carro. O usuário compete contra outros dois carros que tem sua velocidade média gerada aleatoriamente dentro de um limite, sendo eles:

| Corrida         | Velocidade Mínima | Velocidade Máxima |
|-----------------|-------------------|-------------------|
| Tiro de Meta| 50 km/h           | 95 km/h          |
| Circuito Regional | 70 km/h           | 155 km/h         |
| Track Day| 80 km/h           | 175 km/h         |
| Copa pistão  | 90 km/h           | 180 km/h         |

Por outro lado, a velocidade média do cliente é gerada em função dos atributos de seu carro, como o cavalo-vapor, torque e polegadas da roda, gerando assim um valor constante. O jogador vence a corrida se seu tempo para concluir a corrida é menor do que a dos outros participantes, caso contrário, ele perde. Ao simular a corrida é mostrada uma visualização simbólica da corrida, exibindo os tempos de finalização do trajeto pelos competidores. O resultado da corrida é registrado em um histórico de partidas do usuário. 

### `historico()`

Esta função exibe o histórico de corridas do cliente logado no sistema. Após carregar os dados dos usuários, ela imprime um cabeçalho e, em seguida, utiliza a função `strdup()` para criar uma cópia da string de histórico do cliente, evitando que alterações sejam feitas no histórico do cliente, mas sim, na duplicata de seus dados. Todas as vezes que o cliente participa de uma corrida, é concatenada uma string em seu histórico, formando uma longa string que é desmembrada aqui. A função `strtok()` é utilizada para dividir essa string em substrings, usando "/" como delimitador, e armazena cada corrida em um array. Após armazenar todas as corridas, a função exibe cada uma delas e, ao final, libera a memória alocada para a cópia do histórico. 

### `alteracoes_hist()`

Possui o mesmo princípio de funcionamento que o histórico, entretanto, esta função mostra os últimos aprimoramentos feitos pelo cliente. Todo momento que um aprimoramento é feito, é concatenado uma string a esse histórico de alterações, onde fazemos nossa consulta.

### `Os Stages`

Os *“stages”* são aprimoramentos no carro que aumentam seus atributos de cavalos-vapor e torque. Portanto, essas quatro funções possuem este objetivo. Primeiramente, é carregado os dados dos usuários com a função `carregarUsuarios()` e verifica se o stage escolhido já foi realizado anteriormente para evitar duplicação. Cada *“stage”* possui um custo em reais, logo, é feita uma verificação do saldo do cliente, caso este não tenha dinheiro suficiente, o aprimoramento não é feito. Os custos são:

| Stage   | Custo       | Aumento de Potência e Torque |
|---------|-------------|------------------------------|
| `Stage 01( )`| R$ 3.500,00 | 15%                          |
| `Stage 02( )`| R$ 6.700,00 | 30%                          |
| `Stage 03( )`| R$ 9.000,00 | 35%                          |
| `Stage 04( )`| R$ 13.000,00| 40%                          |

Caso o cliente escolha realizar o *stage*, seu saldo, histórico de alterações e os novos valores de torque e potência são salvos com `salvarUsuarios()`.

### `Cmp_roda()`

A função exibe uma lista de tamanhos de rodas e seus respectivos preços, para a escolha do usuário, e então verifica se o usuário possui saldo suficiente antes de aplicar a mudança. Além disso, evita a escolha do tamanho de roda já instalado. No momento da seleção, o saldo do usuário é atualizado, o tamanho da roda é alterado no struct do Cliente logado e uma entrada é registrada no histórico de alterações, indicando a troca. O histórico e atributos são salvos chamando a função `salvarUsuarios()`.

### `aprimoramentos()`

Esta função exibe os aprimoramentos disponíveis para o veículo, como os diferentes estágios de melhoria de desempenho (*Stage 1* a *Stage 4*) e a troca de rodas. Após exibir as opções, a função solicita ao usuário que escolha uma ação. Ela verifica se a entrada é válida e, caso contrário, solicita uma nova entrada até que um valor permitido seja inserido. Com base na escolha do usuário, a função chama a função específica para o aprimoramento selecionado, como `stage01()`, `stage02()`, `Cmp_rodas()`, entre outras. 

### `menu()`
A função menu é o ponto central de navegação do sistema, oferecendo ao usuário uma série de opções para interagir com as diferentes funcionalidades, incluindo visualização da ficha do carro, aprimoramentos, corridas, histórico e alterações feitas. Cada opção é apresentada em um menu, e o usuário digita o número correspondente à sua escolha. A função valida a entrada e solicita uma nova entrada se for inválida. Dependendo da escolha, o sistema chama funções específicas, como `ficha_carro()`, `simula_corrida()` ou `alteracoes_hist()`, para executar a operação selecionada. Se o usuário optar por "Sair", o sistema retorna à tela de login. 

## 4. Funções Complementares 🧩

### `clearbuffer()`

Aplicando esta função após a leitura dos dados, principalmente aqueles que usam `scanf()`, garantimos que o buffer de entrada estará limpo e não haverá interferência com leituras subsequentes. A função percorre e descarta caracteres residuais, até encontrar o fim do arquivo (EOF).

### `converte_tempo()`
Essa função complementa a simulação de corridas, visto que, ao calcular o tempo de finalização das corridas, obtemos o resultado em horas, que por sua vez, pode apresentar valores decimais. Portanto, utilizamos esta função para converter o tempo para uma medida mais conveniente. O conversor de tempo recebe horas e exibe os minutos e segundos do corredor