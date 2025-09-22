#include <stdio.h>     
#include <string.h>   
#include <stdlib.h>   
#include <locale.h>   

// Defini��o de macros para ANSI Escape Codes
#define TCXYEL "\033[93m" // Cor amarela
#define TCXWHT "\033[97m" // Cor branca
#define TCXRED "\033[91m" // Cor vermelha

// Declara prot�tipos das fun��es para poder cham�-las em qualquer local do c�digo
int command_text ( char * ); 
void menu ( void );
void read_file ( char * );
void add_file ( char * );
void rename_file ( char *, char * );
void create_folder ( char * );
void remove_folder ( char * );

void main( void )
{
//	Permite caracteres da l�ngua portuguesa do Brasil
	setlocale(LC_ALL, "ptb");	
	
//	Define as cores do console: fundo preto, texto branco
	system("color 0F");

//  Exibe mensagens de boas-vindas
	printf("Seja bem-vindo ao " TCXYEL "IMFM" TCXWHT "\n\n");
	
//	Exibe instru��o do comando ajuda
    printf("Para visualizar os comandos,\ndigite:\n");
	printf(TCXYEL " > " TCXWHT "ajuda\n\n");
	
//	Exibe a lista de comandos e suas funcionalidades
	menu();

//	La�o infinito para receber os comandos
//	OBS.: S� � poss�vel sair do la�o digitando o comando "sair" ou "q"
	while (1)
	{
//  	Indica que o usu�rio deve entrar com um valor
		printf(TCXYEL "\n> " TCXWHT);
	
//		Vari�vel para armazenar o valor de entrada
		char input[50];
		
//		L� o valor digitado
		fgets(input, sizeof(input), stdin);

//		Remove o "\n" do ENTER
	 	input[strcspn(input, "\n")] = 0;
	 	
//		Verifica se a entrada � vazia
		if (strlen(input) == 0)
			continue; // Entrada vazia: repete o la�o
		
//		Verifica se algum comando foi executado
//		OBS.: A fun��o "command_text" retorna 1, se algum comando for executado, caso contr�rio, retorna 0
		if (command_text(input) == 1) 
			continue; // Comando executado: repete o la�o
			
//		Exibe uma mensagem de erro informando que o comando digitado n�o existe
		printf(TCXRED "ERRO" TCXWHT ": O comando \"%s\" n�o existe.\n", input);
		
//		Exibe mensagem para lembrar o comando ajuda
		printf("Digite \"ajuda\" para visualizar a lista de comandos.\n");
	}
}

//	Exibe uma lista com todos os comandos e suas funcionalidades
void menu ( void )
{    
//  Cabe�alho
	printf(TCXWHT"COMANDO             ATALHO   FUN��O\n");
//			Comando             Atalho   Descri��o
    printf("criardiretorio      cd       Cria um diret�rio.\n");
    printf("criararquivo        ca       Cria um arquivo.\n");
    printf("adicionar           add      Adiciona conte�do a um arquivo texto.\n");
    printf("ler                 -        Imprime o conte�do de um arquivo\n");
    printf("listar              -        Lista os arquivos e diret�rios locais.\n");
    printf("removerdiretorio    rd       Remove um diret�rio.\n");
    printf("removerarquivo      ra       Remove um arquivo.\n");
	printf("renomear            rm       Altera o nome de um arquivo ou diret�rio.\n");
	printf("limpar              ls       Limpa a tela.\n");
	printf("sair                q        Encerra o programa.\n");
}

// Interpreta e executa comandos, quebrando a string passada em tokens
// Retorna 1 se um comando for executado, caso contr�rio, retorna 0
int command_text( char *cmd_text )
{
//	Pega o primeiro token e passa para "cmd" (Primeiro argumento)
	char *cmd = strtok(cmd_text, " ");

//	Verifica se existe argumento
	if (!cmd)
        return 0; // Se n�o existir: retorna 0
     
//	OBS.: A l�gica de compara��o de comandos segue este padr�o:
//  Verifica o retorno da fun��o strcmp ao comparar o primeiro argumento com um comando do programa. Se o retorno for zero, ent�o o argumento � id�ntico ao comando do programa.
 	if (strcmp(cmd, "ajuda") == 0)	
    {
        menu(); 
        return 1; // Comando executado: retorna 1
 	} 

 	if (strcmp(cmd, "ler") == 0)
    {
//    	Pega o segundo token e j� passa para a fun��o como argumento
        read_file( strtok(NULL, " ") );
        return 1;
 	}

//	OBS.: Alguns comandos possuem atalhos, para evitar c�digo duplicado, duas strings s�o comparadas como no c�digo abaixo:
 	if (strcmp(cmd, "adicionar") == 0 || strcmp(cmd, "add") == 0)
    {
//    	Pega o segundo token e j� passa para a fun��o como argumento
        add_file( strtok(NULL, " ") );
        return 1;
	}

 	if (strcmp(cmd, "renomear") == 0 || strcmp(cmd, "rm") == 0)
 	{
// 		Pega o segundo token e passa para "old_name"
 		char *old_name = strtok(NULL, " ");
 		
// 		Pega o terceiro token e passa para "new_name"
 		char *new_name = strtok(NULL, " ");
 		
// 		Verifica se os tokens s�o v�lidos
 		if (old_name == NULL || new_name == NULL)
    	{
//    		Exibe uma mensagem de erro
        	printf(TCXRED "ERRO" TCXWHT ": Invalid argument\n");
        	return 1;
    	}
    	
// 		Chama a fun��o passando os tokens como argumentos
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
//      OBS.: Se o arquivo n�o existir, ele ser� criado automaticamente, caso contr�rio, nada acontecer�.
		FILE *file = fopen(file_name, "a");

//		Verifica se o token � v�lido
    	if (file == NULL)
    	{
//    		Exibe uma mensagem de erro com base no problema ocorrido na fun��o "fopen"
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
    	
// 		Verifica se o token � v�lido
 		if (folder_name == NULL)
    	{
//    		Exibe uma mensagem de erro
        	printf(TCXRED "ERRO" TCXWHT ": Invalid argument\n");
        	return 1;
    	}
    	
// 		Chama a fun��o passando o token como argumento
    	create_folder(folder_name);
        return 1;
    }
    
    if (strcmp(cmd, "removerarquivo") == 0 || strcmp(cmd, "ra") == 0)
    {
// 		Pega o segundo token e passa para "file_name"
		char *filename = strtok(NULL, " ");
		
// 		Chama a fun��o passando o token como argumento. Se a fun��o retornar zero, ela executou sem erros, caso contr�rio, n�o.
		if (remove(filename) == 0)
		{
        	printf("Arquivo '%s' foi removido.\n", filename);
    	} 
		else
		{
//			Exibe uma mensagem de erro com base no problema ocorrido na fun��o "remove"
        	perror(TCXRED "ERRO" TCXWHT ":");
    	}
    	return 1;
    }    

    if (strcmp(cmd, "removerdiretorio") == 0 || strcmp(cmd, "rd") == 0)
    {
//    	Pega o segundo token e passa para "folder_name"
		char *folder_name = strtok(NULL, " ");
		
// 		Verifica se o token � v�lido
 		if (folder_name == NULL)
    	{
    		// Exibe mensagem de erro
        	printf(TCXRED "ERRO" TCXWHT ": Invalid argument\n");
        	return 1;
    	}
    	
// 		Chama a fun��o passando o token como argumento
    	remove_folder(folder_name);
        return 1;
    }

	return 0; //  Se nenhum comando for encontrado: retorna 0
}

// Abre e imprime o conte�do de um arquivo
void read_file ( char *file_name)
{
//	Abre o arquivo em modo leitura
    FILE *file = fopen(file_name, "r");
    
//  Verifica se o arquivo � v�lido
    if (file == NULL)
    {
//    	Exibe uma mensagem de erro com base no problema ocorrido na fun��o "fopen"
        perror(TCXRED "ERRO" TCXWHT);
        return;
    }

//  Imprime o nome do arquivo para indicar a exibi��o de seu conte�do
    printf(TCXYEL "%s" TCXWHT ":\n", file_name);

//	Imprime o conte�do do arquivo
    char ch;
    while ((ch = fgetc(file)) != EOF)
        putchar(ch);
        
//  Fecha o arquivo
    fclose(file); 
    
    printf("\n");
    
//  Informa que o arquivo foi fechado
    printf(TCXYEL "%s " TCXWHT "foi fechado\n", file_name);
}

// Abre, exibe e adiciona conte�do a um arquivo
void add_file (char *file_name)
{
//	Abre arquivo em modo leitura e escrita
    FILE *file = fopen(file_name, "r+");

//  Verifica se o arquivo � v�lido
    if (file == NULL)
    {
//    	Exibe uma mensagem de erro com base no problema ocorrido na fun��o "fopen"
        perror(TCXRED "ERRO" TCXWHT);
        return;
    }
    printf("\n");

//  Imprime o nome do arquivo para indicar a exibi��o de seu conte�do
    printf(TCXYEL "%s" TCXWHT ":\n", file_name);

//	Imprime o conte�do do arquivo
    char ch;
    while ((ch = fgetc(file)) != EOF)
        putchar(ch);
        
//  Indica que o usu�rio deve entrar com um valor
    printf(TCXYEL " > " TCXWHT);
    
//  Vari�vel para armazenar o conte�do a ser adicionado ao arquivo
	char file_content[200];
	
//	L� o valor digitado
    fgets(file_content, sizeof(file_content), stdin);

//  Vari�vel para armazenar a op��o escolhida
 	char option;
//	La�o para confirma��o de salvamento
    do
    {
        printf("\n");
        
//      Exibe mensagem de confirma��o de salvamento
        printf("Voc� deseja salvar as altera��es? (s/N) ");
        
//      L� a op��o escolhida
		option = getchar();
//		Limpa o buffer de entrada (Teclado)
		fflush(stdin);

    } while (option != 's' && option != 'N'); // Enquanto o primeiro caractere informado for diferente de 's' ou 'N', o la�o continua

    if (option == 's')
    {
//    	Imprime o novo cont�udo ao arquivo
    	fprintf(file, "%s", file_content);   
        
		printf("Altera��o conclu�da.\n");
    }
    else
    {
    	printf("Altera��o cancelada.\n");
	}
	
//	Fecha o arquivo
	fclose(file);
}

// Renomeia um diret�rio
void rename_file (char *old_name, char *new_name)
{
//	Chama a fun��o passando "old_name" e "new_name" como argumentos. Se retornar zero, a fun��o executou sem erros, caso contr�rio, n�o.
    if (rename(old_name, new_name) == 0)
    {
//    	Exibe uma mensagem confirmando a renomea��o
        printf("%s renomeado para %s\n", old_name, new_name);
        return;
    }
	else
	{
//		Exibe uma mensagem de erro com base no problema ocorrido na fun��o "rename"
		perror(TCXRED "ERRO" TCXWHT);
	}
}

// Cria um diret�rio
void create_folder ( char *folder_name )
{
//	Vari�vel para armazenar a formata��o do comando
    char format[50];

//  Formata o comando e seu argumento  
    sprintf(format, "mkdir %s", folder_name);
    
//  Verifica se a fun��o foi executada sem problemas
    if (system(format) == 0)
    	printf("Diret�rio \"%s\" foi criado.\n", folder_name);
//  Se o retorno for diferente de zero, o pr�prio sistema exibir� uma mensagem de erro
}

//	Remove um diret�rio
void remove_folder ( char *folder_name )
{
//	Vari�vel para armazenar a formata��o do comando
    char format[50];
    
//  Formata o comando e seu argumento
    sprintf(format, "rmdir %s", folder_name);
    
//  Verifica se a fun��o foi executada sem problemas
    if (system(format) == 0)
    	printf("Diret�rio \"%s\" foi removido.\n", folder_name);
//  Se o retorno for diferente de zero, o pr�prio sistema exibir� uma mensagem de erro
}
