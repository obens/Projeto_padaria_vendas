#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///ESTRUTURAS
typedef struct _produto{
    char nome[30], data_fab[12], data_val[12];
    double val_venda;
}Produto;

typedef struct _compra{
    Produto produtos[20];
    //Adicionei a variável quantidade para cada produto
    int quantidade[20];
    //Adicionei a variável itens, que controla quantos itens tem em cada compra
    //  Serve para percorrer apenas os itens da compra, e não todas as 20 posições do vetor
    int itens;
    double total;
}Compra;

typedef struct _cartao{
    int numero;
    Compra compras[20];
    //Adicionei variável quantidade de compras, para melhor controle do vetor
    int qntd_compras;
    double total_compras;
}Cartao;

typedef struct _cliente{
    char nome[30], cpf[16], telefone[17];
    Cartao cartao;
}Cliente;

typedef struct _estoque{
    Produto produto;
    int qtd_estoque;
    double val_aquisicao;
    //Adicionei campo fornecedor
    char fornecedor[30];
}Estoque;

//Relatorio de compras
typedef struct _relatorio{
    Cliente cliente[50];
    Compra compra[50];
    double troco[50];
    int numero_cartao[50];
    int opcao[50];
    int controle_relatorio;
}Relatorio;


///FUNÇÕES
void cadastrar_cliente(Cliente cliente[], int numero_cliente){
    printf("Escreva o nome do cliente:\n");
    fgets(cliente[numero_cliente].nome,30,stdin);
    printf("Escreva o numero do cpf:\n");
    fgets(cliente[numero_cliente].cpf,16,stdin);
    printf("Escreva o numero de telefone do cliente:\n");
    fgets(cliente[numero_cliente].telefone,17,stdin);

    //Após cadastrar um cliente, cadastramos um cartão para esse cliente
    printf("Informe número do cartao do cliente:\n");
    scanf(" %d",&cliente[numero_cliente].cartao.numero);
    getchar();
    //Iniciar cartão zerado
    cliente[numero_cliente].cartao.qntd_compras = 0;
    cliente[numero_cliente].cartao.total_compras = 0;
    printf("\nCliente cadastrado com sucesso!\n\n");
 }

void cadastrar_produto(Estoque estoque[], int numero_produto){
    double val;
    //Sera um vetor de estoque, cada estoque tem um produto
    printf("Escreva o nome do produto:\n");
    fgets(estoque[numero_produto].produto.nome,30,stdin);
    printf("Escreva a data de fabricacao do produto:\n");
    fgets(estoque[numero_produto].produto.data_fab,12,stdin);
    printf("Escreva a data de validade do produto:\n");
    fgets(estoque[numero_produto].produto.data_val,12,stdin);
    printf("Escreva o preco do produto:\n");
    scanf(" %lf", &val);
    getchar();
    estoque[numero_produto].produto.val_venda = val;

    //Iniciar quantidade em estoque como 0
    estoque[numero_produto].qtd_estoque = 0;
}

void cadastrar_venda(Cliente cliente[], Estoque estoque[], Compra compra[], Relatorio relatorio[], int numero_cliente, int numero_produto, int numero_compra, int numero_relatorio){
    /*
    1º - É informado ao sistema quais produtos e quantidade compõem a nova compra;
    2º - Informamos ao sistema o método de pagamento, se a pessoas pagar em dinheiro não precisa
        identificar o cliente, agora pagamento em cartão ou cheque precisamo identificar o cliente
    3º - Registrar no vetor de compras a compra realizada, para ter um controle do sistema... Se for pago
        com cartão, registrar a compra no cartão do cliente e incrementar valor total_compras do cartão.
    */
    int i, j, controle_compra=0, card_num, tentativas;
    char nome_prod[30], op;
    double money;

    printf("\nNOVA COMPRA\n------------\n");

    //Inciar subtotal com 0
    compra[numero_compra].total = 0;

    //Laço com condição de parada interna
    while(1){
        printf("\nDigite o nome do produto:\n");
        fgets(nome_prod,30,stdin);
        //Buscar por produto com nome digitado, se encontrar para o laço. i armazena o indice do produto
        for(i=0; i<numero_produto; i++){
            if(strcmp(nome_prod, estoque[i].produto.nome) == 0) break;
        }
        //Verifica se não encontrou produto, se achou pede quantidade
        if(i == numero_produto){
            printf("\nProduto não encontrado no cadastro!\n");
            //Setamos op para 's', assim ela roda denovo pedindo nome do produto
            op = 's';
        }
        else{
            //Adiciona o produto encontrado na lista de compra
            compra[numero_compra].produtos[controle_compra] = estoque[i].produto;

            do{
                printf("\nDigite a quantidade:\n");
                scanf(" %d", &compra[numero_compra].quantidade[controle_compra]);
                getchar();
            }while(compra[numero_compra].quantidade[controle_compra] > estoque[i].qtd_estoque);

            //Incrementar valor do subtotal multiplicando preço do produto com a quantidade
            compra[numero_compra].total +=  (compra[numero_compra].produtos[controle_compra].val_venda * compra[numero_compra].quantidade[controle_compra]);

            //Se alcancar o maximo do carrinho quebra laço automaticamente
            if(controle_compra == 19) break;

            printf("\nAdicionar outro produto? s/n\n");
            do{
                op = getchar();
            }while(op != 's' && op != 'n');
            controle_compra++;
        }
        //CONDIÇÂO DE PARADA INTERNA do laço da lista de compras
        if(op == 'n') break;
    }

    //verificar pagamento se controle_compra for > 0
    if(controle_compra>0){
        do{
            printf("\tMetodo de Pagamento\n");
            printf("1 - Cartao Supermercado:\n");
            printf("2 - Dinheiro:\n");
            printf("3 - Cheque:\n");
            printf("4 - Cartao debito:\n");
            printf("5 - Cancelar compra:\n");
            printf("\n--> ");
            scanf(" %c", &op);
            getchar();
        }while(op != '1' && op != '2' && op != '3' && op != '4' && op != '5');

        //Usaremos j como uma flag agora... Será 1 se pagamento deu certo, ou 0 se pagamento foi cancelado
        switch(op){
            case '1':
                //Tem 3 tentativas pra digitar numero de cartao certo, sen cancela compra
                tentativas = 0;
                do{
                    printf("\nDigite o numero do cartao:\n");
                    scanf(" %d", &card_num);
                    getchar();
                    //Procurar pelo cliente com o numero do cartao informado
                    for(i=0; i<numero_cliente; i++){
                        if(cliente[i].cartao.numero == card_num) break;
                    }
                    //Se não encontrar, pede numero denovo
                    if(i == numero_cliente){
                        if(tentativas == 2){
                            //Cancela compra
                            j = 0;
                            i = -1;
                        }
                        tentativas++;
                        continue;
                    }

                    //Se encontrar, continua o codigo aqui
                    printf("\n\tConfirmar dados do cliente:\n");
                    printf("Nome: %s\n", cliente[i].nome);
                    printf("CPF: %s\n", cliente[i].cpf);
                    printf("Telefone: %s\n", cliente[i].telefone);

                    printf("\n\nProsseguir com a compra? s/n");
                    do{
                        op = getchar();
                    }while(op != 's' && op != 'n');
                    //Se nao continuar, seta flag j para 0
                    if(op == 'n') j = 0;
                    else{
                        //Continuamos, seta flag j pra 1
                        j = 1;
                        //Registramos a compra no cartão do cliente
                        cliente[i].cartao.compras[cliente[i].cartao.qntd_compras] = compra[numero_compra];
                        cliente[i].cartao.total_compras += compra[numero_compra].total;
                        cliente[i].cartao.qntd_compras += 1;

                        //Registrar no relatorio
                        relatorio[numero_relatorio].cliente[relatorio[numero_relatorio].controle_relatorio] = cliente[i];
                        relatorio[numero_relatorio].compra[relatorio[numero_relatorio].controle_relatorio] = compra[numero_compra];
                        relatorio[numero_relatorio].troco[relatorio[numero_relatorio].controle_relatorio] = 0.0;
                        relatorio[numero_relatorio].opcao[relatorio[numero_relatorio].controle_relatorio] = 1;
                        relatorio[numero_relatorio].controle_relatorio += 1;
                    }
                }while(i==numero_cliente);
                break;
            case '2':
                printf("\nDigite o valor pago em dinheiro:\n");
                scanf(" %lf", &money);
                getchar();
                //Contabilizar troco
                printf("\nTroco: %.2lf reais\nPressione qualquer tecla para continuar...", money-compra[numero_compra].total);
                getchar();

                //Registrar no relatorio
                relatorio[numero_relatorio].compra[relatorio[numero_relatorio].controle_relatorio] = compra[numero_compra];
                relatorio[numero_relatorio].troco[relatorio[numero_relatorio].controle_relatorio] = money-compra[numero_compra].total;
                relatorio[numero_relatorio].opcao[relatorio[numero_relatorio].controle_relatorio] = 2;
                relatorio[numero_relatorio].controle_relatorio += 1;
                j = 1;
                break;
            case '3':
                printf("\nDigite o numero do cheque:\n");
                scanf(" %d", &card_num);
                getchar();

                relatorio[numero_relatorio].compra[relatorio[numero_relatorio].controle_relatorio] = compra[numero_compra];
                relatorio[numero_relatorio].opcao[relatorio[numero_relatorio].controle_relatorio] = 3;
                relatorio[numero_relatorio].numero_cartao[relatorio[numero_relatorio].controle_relatorio] = card_num;
                relatorio[numero_relatorio].controle_relatorio += 1;
                j = 1;
                break;
            case '4':
                printf("\nDigite o numero do cartao de debito:\n");
                scanf(" %d", &card_num);
                getchar();

                //Registrar no relatorio
                relatorio[numero_relatorio].compra[relatorio[numero_relatorio].controle_relatorio] = compra[numero_compra];
                relatorio[numero_relatorio].opcao[relatorio[numero_relatorio].controle_relatorio] = 4;
                relatorio[numero_relatorio].numero_cartao[relatorio[numero_relatorio].controle_relatorio] = card_num;
                relatorio[numero_relatorio].controle_relatorio += 1;
                j = 1;
                break;
            case '5':
                j = 0;
                break;
        }

        if(j==0) printf("\nCompra CANCELADA!\n");
        else{
            /*FINALMENTE, finalizar a compra...
                Precisamos descontar do estoque todos os produtos utilizados na compra, para isso precorremos os produtos da lista de compra
                e no cadastro de produtos. Aproveitamos e ja imprimimos o comprovante/nota enfim...
            */
            printf("\n\tPRODUTOS COMPRADOS\n"
                    "--------------------\n");
            for(j=0; j<controle_compra; j++){
                for(i=0; i<numero_produto; i++){
                    if(strcmp(compra[numero_compra].produtos[j].nome, estoque[i].produto.nome) == 0){
                        estoque[i].qtd_estoque -= compra[numero_compra].quantidade[j];
                        printf("%s", estoque[i].produto.nome);
                        printf("\t Preco unit: %.2lf reais", estoque[i].produto.val_venda);
                        printf("\t Qntd: %d\n\n", compra[numero_compra].quantidade[j]);
                    }
                }
            }
            printf("\nValor total: %.2lf reais\n", compra[numero_compra].total);
            printf("\nCompra finalizada com sucesso!\n");
        }
    }
}

void buscar_cliente(Cliente cliente[], int numero_cliente){
    char cpfdocliente[20];
    int i, cliente_cadastro;

    printf("Escreva o CPF do cliente que deseja encontrar:\n");
    fgets(cpfdocliente,20,stdin);
    for(i=0;i<numero_cliente;i++){
        if(strcmp(cpfdocliente,cliente[i].cpf)==0){
            cliente_cadastro=i;
            break;
            }
    }
    if(i==numero_cliente)
        printf("Este cliente nao existe!\n");
    else{
        printf("Nome do cliente: %s\n",cliente[cliente_cadastro].nome);
        printf("Telefone do cliente: %s\n",cliente[cliente_cadastro].telefone);
        printf("CPF do cliente: %s\n",cliente[cliente_cadastro].cpf);
    }
}
void buscar_produto(Estoque estoque[], int numero_produto){
    int i;
    char nome_prod[30];

    printf("\nDigite o nome do produto:\n");
    fgets(nome_prod,30,stdin);

    //Buscar por produto com nome digitado, se encontrar para o laço. i armazena o indice do produto
    for(i=0; i<numero_produto; i++){
        if(strcmp(nome_prod, estoque[i].produto.nome) == 0) break;
    }

    //Se não encontrar, retorna para o menu
    if(i == numero_produto){
        printf("\nProduto não encontrado no cadastro!\n");
    }
    else{
        printf("Nome: %s\n", estoque[i].produto.nome);
        printf("Data Fab: %s\n", estoque[i].produto.data_fab);
        printf("Data Val: %s\n", estoque[i].produto.data_val);
        printf("Preco: %lf\n", estoque[i].produto.val_venda);
        printf("Quantidade em estoque: %d\n\n\n", estoque[i].qtd_estoque);
    }
}

void adicionar_prod_estoque(Estoque estoque[], int numero_produto){
    int i;
    char nome_prod[30];

    printf("\nDigite o nome do produto:\n");
    fgets(nome_prod,30,stdin);

    //Buscar por produto com nome digitado, se encontrar para o laço. i armazena o indice do produto
    for(i=0; i<numero_produto; i++){
        if(strcmp(nome_prod, estoque[i].produto.nome) == 0) break;
    }

    //Se não encontrar, retorna para o menu
    if(i == numero_produto){
        printf("\nProduto não encontrado no cadastro!\n");
    }
    else{
        printf("\nDigite o nome do fornecedor:\n");
        fgets(estoque[i].fornecedor,30,stdin);
        printf("\nDigite o valor de aquisicao:\n");
        scanf(" %lf", &estoque[i].val_aquisicao);
        getchar();
        printf("\nDigite a quantidade:\n");
        scanf(" %d", &estoque[i].qtd_estoque);
        getchar();

        printf("\nEstoque de %s abastecido com sucesso!\n", nome_prod);
    }
}

void listar_prod_fornecedor(Estoque estoque[], int numero_produto){
    int i, qntd=0;
    char nome_forn[30];

    printf("\nDigite o nome do fornecedor:\n");
    fgets(nome_forn,30,stdin);

    for(i=0; i<numero_produto; i++){
        if(strcmp(nome_forn, estoque[i].fornecedor) == 0){
            qntd++;
            printf("Produto %d: %s\n", qntd, estoque[i].produto.nome);
        }
    }
    if(qntd == 0){
        printf("\nFornecedor não encontrado!\n\n");
    }
}

void listar_prod_baixa(Estoque estoque[], int numero_produto){
    int i;

    for(i=0; i<numero_produto; i++){
        //Considerar em baixa, produtos com menos de 10 unidades
        if(estoque[i].qtd_estoque < 10){
            printf("Nome: %s\n", estoque[i].produto.nome);
            printf("Quantidade em estoque: %d\n\n\n", estoque[i].qtd_estoque);
        }
    }
}

void listar_clientes(Cliente cliente[], int numero_cliente){
    int i;
    for(i=0; i<numero_cliente; i++){
        printf("Nome: %s\n", cliente[i].nome);
        printf("CPF: %s\n", cliente[i].cpf);
        printf("Telefone: %s\n", cliente[i].telefone);
        printf("Cartao: %d\n\n\n", cliente[i].cartao.numero);
    }
}


///FUNÇÃO PRINCIPAL
int main(){
    //Alocar estruturas e variáveis de controle
    int numero_cliente=0, numero_produto=0, numero_compra=0, numero_relatorio=0;
    //Cliente cliente[100];
    Cliente *cliente;
    Estoque *estoque;
    Compra *compra;
    Relatorio *relatorio;

    cliente = (Cliente *)malloc(100*sizeof(Cliente));
    estoque = (Estoque *)malloc(100*sizeof(Estoque));
    compra = (Compra *)malloc(100*sizeof(Compra));
    relatorio = (Relatorio *)malloc(100*sizeof(Relatorio));

    int opcao;
    do{
        printf("\tMenu de Opções:\n");
        printf("1 - Cadastrar clientes, produtos ou venda:\n");
        printf("2 - Buscar cliente:\n");
        printf("3 - Buscar produto:\n");
        printf("4 - Adicionar produto no estoque:\n");
        printf("5 - Listar produtos:\n");
        printf("6 - Listar clientes:\n");
        printf("0 - Sair do sistema:\n");
        printf("\n--> ");
        scanf("%d", &opcao);
        getchar();
        printf("\n");
        switch(opcao){
            case 1:
                do{
                    printf("\tSubmenu de cadastro\n");
                    printf("1 - Cadastrar cliente:\n");
                    printf("2 - Cadastrar produto:\n");
                    printf("3 - Cadastrar venda:\n");
                    printf("0 - Voltar ao menu anterior\n");
                    printf("\n--> ");
                    scanf("%d", &opcao);
                    getchar();
                    printf("\n");
                    switch(opcao){
                        case 1:
                            cadastrar_cliente(cliente, numero_cliente);
                            numero_cliente++;
                            opcao = 0;
                            break;
                        case 2:
                            cadastrar_produto(estoque, numero_produto);
                            numero_produto++;
                            opcao = 0;
                            break;
                        case 3:
                            cadastrar_venda(cliente, estoque, compra, relatorio, numero_cliente, numero_produto, numero_compra, numero_relatorio);
                            numero_relatorio++;
                            numero_compra++;
                            opcao = 0;
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opcao inválida\n\n");
                            break;
                    }
                }while(opcao != 0);
                opcao = 1;
                break;
            case 2:
                buscar_cliente(cliente, numero_cliente);
                break;
            case 3:
                buscar_produto(estoque, numero_produto);
                break;
            case 4:
                adicionar_prod_estoque(estoque, numero_produto);
                break;
            case 5:
                do{
                    printf("\tSubmenu de produtos\n");
                    printf("1 - Listar produtos por fornecedor:\n");
                    printf("2 - Listar produtos em baixa no estoque:\n");
                    printf("0 - Voltar ao menu anterior\n");
                    printf("\n--> ");
                    scanf("%d", &opcao);
                    getchar();
                    printf("\n");
                    switch(opcao){
                        case 1:
                            listar_prod_fornecedor(estoque, numero_produto);
                            opcao = 0;
                            break;
                        case 2:
                            listar_prod_baixa(estoque, numero_produto);
                            opcao = 0;
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opcao inválida\n\n");
                            break;
                    }

                }while(opcao != 0);
                opcao = 5;
                break;
            case 6:
                listar_clientes(cliente, numero_cliente);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida\n\n");
        }
    }while(opcao != 0);

    free(cliente);
    free(estoque);
    free(compra);
    free(relatorio);

    return 0;
}
