#include <stdio.h>     
#include <string.h>   
#include <stdlib.h>   
#include <locale.h>   

// Definição de macros para ANSI Escape Codes
#define TCXYEL "\033[93m" // Cor amarela
#define TCXWHT "\033[97m" // Cor branca
#define TCXRED "\033[91m" // Cor vermelha

// Declara protótipos das funções para poder chamá-las em qualquer local do código
int command_text ( char * ); 
void menu ( void );
void read_file ( char * );
void add_file ( char * );
void rename_file ( char *, char * );
void create_folder ( char * );
void remove_folder ( char * );

void main( void )
{
//	Permite caracteres da língua portuguesa do Brasil
	setlocale(LC_ALL, "ptb");	
	
//	Define as cores do console: fundo preto, texto branco
	system("color 0F");

//  Exibe mensagens de boas-vindas
	printf("Seja bem-vindo ao " TCXYEL "IMFM" TCXWHT "\n\n");
	
//	Exibe instrução do comando ajuda
    printf("Para visualizar os comandos,\ndigite:\n");
	printf(TCXYEL " > " TCXWHT "ajuda\n\n");
	
//	Exibe a lista de comandos e suas funcionalidades
	menu();

//	Laço infinito para receber os comandos
//	OBS.: Só é possível sair do laço digitando o comando "sair" ou "q"
	while (1)
	{
//  	Indica que o usuário deve entrar com um valor
		printf(TCXYEL "\n> " TCXWHT);
	
//		Variável para armazenar o valor de entrada
		char input[50];
		
//		Lê o valor digitado
		fgets(input, sizeof(input), stdin);

//		Remove o "\n" do ENTER
	 	input[strcspn(input, "\n")] = 0;
	 	
//		Verifica se a entrada é vazia
		if (strlen(input) == 0)
			continue; // Entrada vazia: repete o laço
		
//		Verifica se algum comando foi executado
//		OBS.: A função "command_text" retorna 1, se algum comando for executado, caso contrário, retorna 0
		if (command_text(input) == 1) 
			continue; // Comando executado: repete o laço
			
//		Exibe uma mensagem de erro informando que o comando digitado não existe
		printf(TCXRED "ERRO" TCXWHT ": O comando \"%s\" não existe.\n", input);
		
//		Exibe mensagem para lembrar o comando ajuda
		printf("Digite \"ajuda\" para visualizar a lista de comandos.\n");
	}
}

//	Exibe uma lista com todos os comandos e suas funcionalidades
void menu ( void )
{    
//  Cabeçalho
	printf(TCXWHT"COMANDO             ATALHO   FUNÇÃO\n");
//			Comando             Atalho   Descrição
    printf("criardiretorio      cd       Cria um diretório.\n");
    printf("criararquivo        ca       Cria um arquivo.\n");
    printf("adicionar           add      Adiciona conteúdo a um arquivo texto.\n");
    printf("ler                 -        Imprime o conteúdo de um arquivo\n");
    printf("listar              -        Lista os arquivos e diretórios locais.\n");
    printf("removerdiretorio    rd       Remove um diretório.\n");
    printf("removerarquivo      ra       Remove um arquivo.\n");
	printf("renomear            rm       Altera o nome de um arquivo ou diretório.\n");
	printf("limpar              ls       Limpa a tela.\n");
	printf("sair                q        Encerra o programa.\n");
}

// Interpreta e executa comandos, quebrando a string passada em tokens
// Retorna 1 se um comando for executado, caso contrário, retorna 0
int command_text( char *cmd_text )
{
//	Pega o primeiro token e passa para "cmd" (Primeiro argumento)
	char *cmd = strtok(cmd_text, " ");

//	Verifica se existe argumento
	if (!cmd)
        return 0; // Se não existir: retorna 0
     
//	OBS.: A lógica de comparação de comandos segue este padrão:
//  Verifica o retorno da função strcmp ao comparar o primeiro argumento com um comando do programa. Se o retorno for zero, então o argumento é idêntico ao comando do programa.
 	if (strcmp(cmd, "ajuda") == 0)	
    {
        menu(); 
        return 1; // Comando executado: retorna 1
 	} 

 	if (strcmp(cmd, "ler") == 0)
    {
//    	Pega o segundo token e já passa para a função como argumento
        read_file( strtok(NULL, " ") );
        return 1;
 	}

//	OBS.: Alguns comandos possuem atalhos, para evitar código duplicado, duas strings são comparadas como no código abaixo:
 	if (strcmp(cmd, "adicionar") == 0 || strcmp(cmd, "add") == 0)
    {
//    	Pega o segundo token e já passa para a função como argumento
        add_file( strtok(NULL, " ") );
        return 1;
	}

 	if (strcmp(cmd, "renomear") == 0 || strcmp(cmd, "rm") == 0)
 	{
// 		Pega o segundo token e passa para "old_name"
 		char *old_name = strtok(NULL, " ");
 		
// 		Pega o terceiro token e passa para "new_name"
 		char *new_name = strtok(NULL, " ");
 		
// 		Verifica se os tokens são válidos
 		if (old_name == NULL || new_name == NULL)
    	{
//    		Exibe uma mensagem de erro
        	printf(TCXRED "ERRO" TCXWHT ": Invalid argument\n");
        	return 1;
    	}
    	
// 		Chama a função passando os tokens como argumentos
 	    rename_file(old_name, new_name);
	 	return 1;
 	}

 	if (strcmp(cmd, "sair") == 0 || strcmp(cmd, "q") == 0)
 	{
 		printf("Programa encerrado.\n");
// 		Encerra o programa
 		exit(0); 
 	}

 	if (strcmp(cmd, "listar") == 0)
 	{
// 		Comando do sistema para listar
 		system("dir");
 		return 1;
 	}

 	if (strcmp(cmd, "limpar") == 0 || strcmp(cmd, "ls") == 0)
 	{
// 		Comando do sistema para limpar console
 		system("cls");
 		return 1;
 	}

	if (strcmp(cmd, "criararquivo") == 0 || strcmp(cmd, "ca") == 0)
    {
//    	Pega o segundo token e passa para "file_name"
		char *file_name = strtok(NULL, " ");

//      Abre um arquivo em modo escrita
//      OBS.: Se o arquivo não existir, ele será criado automaticamente, caso contrário, nada acontecerá.
		FILE *file = fopen(file_name, "a");

//		Verifica se o token é válido
    	if (file == NULL)
    	{
//    		Exibe uma mensagem de erro com base no problema ocorrido na função "fopen"
        	perror(TCXRED "ERRO" TCXWHT ":");
        	return 1;
    	}
    	
//    	Fecha o arquivo
    	fclose(file); 
    	return 1;
	}
    
	if (strcmp(cmd, "criardiretorio") == 0 || strcmp(cmd, "cd") == 0)
    {
// 		Pega o segundo token e passa para "folder_name"
    	char *folder_name = strtok(NULL, " ");
    	
// 		Verifica se o token é válido
 		if (folder_name == NULL)
    	{
//    		Exibe uma mensagem de erro
        	printf(TCXRED "ERRO" TCXWHT ": Invalid argument\n");
        	return 1;
    	}
    	
// 		Chama a função passando o token como argumento
    	create_folder(folder_name);
        return 1;
    }
    
    if (strcmp(cmd, "removerarquivo") == 0 || strcmp(cmd, "ra") == 0)
    {
// 		Pega o segundo token e passa para "file_name"
		char *filename = strtok(NULL, " ");
		
// 		Chama a função passando o token como argumento. Se a função retornar zero, ela executou sem erros, caso contrário, não.
		if (remove(filename) == 0)
		{
        	printf("Arquivo '%s' foi removido.\n", filename);
    	} 
		else
		{
//			Exibe uma mensagem de erro com base no problema ocorrido na função "remove"
        	perror(TCXRED "ERRO" TCXWHT ":");
    	}
    	return 1;
    }    

    if (strcmp(cmd, "removerdiretorio") == 0 || strcmp(cmd, "rd") == 0)
    {
//    	Pega o segundo token e passa para "folder_name"
		char *folder_name = strtok(NULL, " ");
		
// 		Verifica se o token é válido
 		if (folder_name == NULL)
    	{
    		// Exibe mensagem de erro
        	printf(TCXRED "ERRO" TCXWHT ": Invalid argument\n");
        	return 1;
    	}
    	
// 		Chama a função passando o token como argumento
    	remove_folder(folder_name);
        return 1;
    }

	return 0; //  Se nenhum comando for encontrado: retorna 0
}

// Abre e imprime o conteúdo de um arquivo
void read_file ( char *file_name)
{
//	Abre o arquivo em modo leitura
    FILE *file = fopen(file_name, "r");
    
//  Verifica se o arquivo é válido
    if (file == NULL)
    {
//    	Exibe uma mensagem de erro com base no problema ocorrido na função "fopen"
        perror(TCXRED "ERRO" TCXWHT);
        return;
    }

//  Imprime o nome do arquivo para indicar a exibição de seu conteúdo
    printf(TCXYEL "%s" TCXWHT ":\n", file_name);

//	Imprime o conteúdo do arquivo
    char ch;
    while ((ch = fgetc(file)) != EOF)
        putchar(ch);
        
//  Fecha o arquivo
    fclose(file); 
    
    printf("\n");
    
//  Informa que o arquivo foi fechado
    printf(TCXYEL "%s " TCXWHT "foi fechado\n", file_name);
}

// Abre, exibe e adiciona conteúdo a um arquivo
void add_file (char *file_name)
{
//	Abre arquivo em modo leitura e escrita
    FILE *file = fopen(file_name, "r+");

//  Verifica se o arquivo é válido
    if (file == NULL)
    {
//    	Exibe uma mensagem de erro com base no problema ocorrido na função "fopen"
        perror(TCXRED "ERRO" TCXWHT);
        return;
    }
    printf("\n");

//  Imprime o nome do arquivo para indicar a exibição de seu conteúdo
    printf(TCXYEL "%s" TCXWHT ":\n", file_name);

//	Imprime o conteúdo do arquivo
    char ch;
    while ((ch = fgetc(file)) != EOF)
        putchar(ch);
        
//  Indica que o usuário deve entrar com um valor
    printf(TCXYEL " > " TCXWHT);
    
//  Variável para armazenar o conteúdo a ser adicionado ao arquivo
	char file_content[200];
	
//	Lê o valor digitado
    fgets(file_content, sizeof(file_content), stdin);

//  Variável para armazenar a opção escolhida
 	char option;
//	Laço para confirmação de salvamento
    do
    {
        printf("\n");
        
//      Exibe mensagem de confirmação de salvamento
        printf("Você deseja salvar as alterações? (s/N) ");
        
//      Lê a opção escolhida
		option = getchar();
//		Limpa o buffer de entrada (Teclado)
		fflush(stdin);

    } while (option != 's' && option != 'N'); // Enquanto o primeiro caractere informado for diferente de 's' ou 'N', o laço continua

    if (option == 's')
    {
//    	Imprime o novo contéudo ao arquivo
    	fprintf(file, "%s", file_content);   
        
		printf("Alteração concluída.\n");
    }
    else
    {
    	printf("Alteração cancelada.\n");
	}
	
//	Fecha o arquivo
	fclose(file);
}

// Renomeia um diretório
void rename_file (char *old_name, char *new_name)
{
//	Chama a função passando "old_name" e "new_name" como argumentos. Se retornar zero, a função executou sem erros, caso contrário, não.
    if (rename(old_name, new_name) == 0)
    {
//    	Exibe uma mensagem confirmando a renomeação
        printf("%s renomeado para %s\n", old_name, new_name);
        return;
    }
	else
	{
//		Exibe uma mensagem de erro com base no problema ocorrido na função "rename"
		perror(TCXRED "ERRO" TCXWHT);
	}
}

// Cria um diretório
void create_folder ( char *folder_name )
{
//	Variável para armazenar a formatação do comando
    char format[50];

//  Formata o comando e seu argumento  
    sprintf(format, "mkdir %s", folder_name);
    
//  Verifica se a função foi executada sem problemas
    if (system(format) == 0)
    	printf("Diretório \"%s\" foi criado.\n", folder_name);
//  Se o retorno for diferente de zero, o próprio sistema exibirá uma mensagem de erro
}

//	Remove um diretório
void remove_folder ( char *folder_name )
{
//	Variável para armazenar a formatação do comando
    char format[50];
    
//  Formata o comando e seu argumento
    sprintf(format, "rmdir %s", folder_name);
    
//  Verifica se a função foi executada sem problemas
    if (system(format) == 0)
    	printf("Diretório \"%s\" foi removido.\n", folder_name);
//  Se o retorno for diferente de zero, o próprio sistema exibirá uma mensagem de erro
}
