#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
// Protótipo das funções para poder chamá-las em qualquer local do código
int command_text ( char cmd_text[], int tam );
char get_option ( void );
void menu ( void );
void read_file ( char * );
void add_file ( char * );
void rename_file ( char *, char * );
void create_folder ( char * );

int main( void )
{
//	Para permitir caracteres do idioma Portugues BR
	setlocale(LC_ALL, "ptb");	
	
//	Para permitir a saída de cores nos caracteres
	system("color 0F");

	printf("Seja bem-vindo ao \e[1;93mIMFM\e[1;97m\n");
	printf("\n");
    printf("Para visualizar os comandos,\ndigite:\n");
	printf("\e[1;93m > \e[97majuda\n\n");
	menu();

//	Cria um laço infinito, saindo apenas com o comando "sair"
	while (1)
	{
		printf("\n\e[1;93m > \e[1;97m");
		char input[50];
		fgets(input, sizeof(input), stdin);

//		Remove o "\n" do ENTER
	 	input[strcspn(input, "\n")] = 0;
//		Evita string vazia
		if (strlen(input) == 0)  {}
		else
		{
//			Verifica se um comando foi executado
			if (command_text(input, sizeof(input)) == 1) {}
			else
//			Se nenhum comando foi executado, exibe erro
			{
				printf("\e[31;1mERRO\e[97m: O comando \"%s\" não existe.\n", input);
				printf("Digite \"ajuda\" para visualizar a lista de comandos.\n");
			}
		}
	}
	return 0;
}
//	Exibe uma lista com todos os comandos e suas funcionalidades
void menu ( void )
{    
	printf("\e[1;97mCOMANDO\t\t\tATALHO\t\tFUNÇÃO\e[97m\n");
    printf("criardiretorio\t\tcd\t\tCria um diretório.\n");
    printf("criararquivo\t\tca\t\tCria um arquivo.\n");
    printf("adicionar\t\tadd\t\tAdiciona conteúdo a um arquivo texto.\n");
    printf("ler\t\t\t\t\tImprime o conteúdo de um arquivo\n");
    printf("listar\t\t\t\t\tLista os arquivos e diretórios locais.\n");
    printf("removerdiretorio\trd\t\tRemove um diretório.\n");
    printf("removerarquivo\t\tra\t\tRemove um arquivo.\n");
	printf("renomear\t\trm\t\tAltera o nome de um arquivo ou diretório.\n");
	printf("limpar\t\t\tls\t\tLimpa a tela.\n");
	printf("sair\t\t\tq\t\tEncerra o programa.\n");
}

// Compara um texto digitado com um comando
// Retorna 1 se um comando for executado, caso contrário: 0
int command_text( char cmd_text[], int tam )
{
//	Separa os argumentos.
	char *cmd = strtok(cmd_text, " ");

//	Caso não tenha digitado nada, retorna 0 (Sai da função)
	if (!cmd)
        return 0;
        
 	if (strcmp(cmd_text, "ajuda") == 0)
    {
        menu();
        return 1;
 	}

 	if (strcmp(cmd_text, "ler") == 0)
    {
        read_file( strtok(NULL, " ") );
        return 1;
 	}

 	if (strcmp(cmd_text, "adicionar") == 0 || strcmp(cmd_text, "add") == 0)
    {
        add_file( strtok(NULL, " ") );
        return 1;
	}

 	if (strcmp(cmd_text, "renomear") == 0 || strcmp(cmd_text, "rm") == 0)
 	{
 		char *old_name = strtok(NULL, " ");
 		char *new_name = strtok(NULL, " ");
// 		Verifica se falta argumentos
 		if (old_name == NULL || new_name == NULL)
    	{
        	printf("\e[31;1mERRO\e[97m: Invalid argument\n");
        	return 1;
    	}
 		
 	    rename_file(old_name, new_name);
	 	return 1;
 	}

 	if (strcmp(cmd_text, "sair") == 0 || strcmp(cmd_text, "q") == 0)
 	{
 		printf("Programa encerrado.\n");
 		exit(0);
 	}

 	if (strcmp(cmd_text, "listar") == 0)
 	{
 		system("dir");
 		return 1;
 	}

 	if (strcmp(cmd_text, "limpar") == 0 || strcmp(cmd_text, "ls") == 0)
 	{
 		system("cls");
 		return 1;
 	}

	if (strcmp(cmd_text, "criararquivo") == 0 || strcmp(cmd_text, "ca") == 0)
    {
		char *file_name = strtok(NULL, " ");

    	FILE *file = fopen(file_name, "ab");

    	if (file == NULL)
    	{
        	perror("\e[31;1mERRO\e[97m");
        	return 1;
    	}
    	fclose(file); 
    	return 1;
	}

	if (strcmp(cmd_text, "criardiretorio") == 0 || strcmp(cmd_text, "cd") == 0)
    {
    	char *folder_name = strtok(NULL, " ");
// 		Verifica se falta argumentos
 		if (folder_name == NULL)
    	{
        	printf("\e[31;1mERRO\e[97m: Invalid argument\n");
        	return 1;
    	}
    	create_folder(folder_name);
        return 1;
    }
    
    if (strcmp(cmd_text, "removerarquivo") == 0 || strcmp(cmd_text, "ra") == 0)
    {
		char *filename = strtok(NULL, " ");
 		
		if (remove(filename) == 0)
		{
        	printf("Arquivo '%s' foi removido.\n", filename);
    	} 
		else
		{
        	perror("\e[31;1mERRO\e[97m");
    	}
    	return 1;
    }    

    if (strcmp(cmd_text, "removerpasta") == 0 || strcmp(cmd_text, "rd") == 0)
    {
		char *folder_name = strtok(NULL, " ");
// 		Verifica se falta argumentos
 		if (folder_name == NULL)
    	{
        	printf("\e[31;1mERRO\e[97m: Invalid argument\n");
        	return 1;
    	}
    	remove_folder(folder_name);
        return 1;
    }
	return 0;
}

// Abre o arquivo e imprime seu conteúdo na tela
void read_file ( char *file_name)
{
    FILE *file = fopen(file_name, "r");
    
    if (file == NULL)
    {
        perror("\e[31;1mERRO\e[97m");
        return;
    }

    printf("\e[1;93m%s\e[97m:\n", file_name);

//	Captura e imprime caractere por caractere do arquivo informado
    char ch;
    while ((ch = fgetc(file)) != EOF)
        putchar(ch);
        
    fclose(file); 
    printf("\n");
    printf("\e[1;93m%s\e[97m foi fechado\n", file_name);
}

// Adiciona um texto ao final do arquivo
void add_file (char *file_name)
{
    FILE *file = fopen(file_name, "a");

    if (file == NULL)
    {
        perror("\e[31;1mERRO\e[97m");
        return;
    }
    printf("\n");

    printf("\e[1;93m%s\e[97m:\n", file_name);
    printf("\e[1;93m > \e[97m");
    
	char file_content[200];
    fgets(file_content, sizeof(file_content), stdin);
    fprintf(file, "%s", file_content);

    char option;
    do
    {
        printf("\n");
        printf("Você deseja salvar as alterações? (s/N) ");
        option = get_option();

    } while (option != 's' && option != 'N');

    if (option == 's')
    {
        fclose(file);
        printf("Alteração concluída.\n");
        return;
    }
    printf("Alteração cancelada.\n");
}

// Renomeia um diretório
void rename_file (char *old_name, char *new_name)
{
    if (rename(old_name, new_name) == 0)
    {
        printf("%s renomeado para %s\n", old_name, new_name);
        return;
    }
	else perror("\e[31;1mERRO\e[97m");
}

// Cria um diretório
void create_folder ( char *folder_name )
{
    char format[50];
    sprintf(format, "mkdir %s", folder_name);
    if (system(format) != 0)
    	return;
    printf("Diretório \"%s\" foi criado.\n", folder_name);
}

// Remove um diretório
int remove_folder ( char *folder_name )
{
    char format[50];
    sprintf(format, "rmdir %s", folder_name);
    if (system(format) != 0)
    	return;
    printf("Diretório \"%s\" foi removido.\n", folder_name);
}

// Utils
char get_option( void )
{
	char option[50];
    fgets(option, sizeof(option), stdin);
    return option[0];
}
