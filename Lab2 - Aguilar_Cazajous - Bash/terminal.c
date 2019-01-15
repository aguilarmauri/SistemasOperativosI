#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

#include "info.h"
#include "cd.h"
#include "run.h"




int main(int argc, char *argv[]){
	
	//Definicion de variables
	char comando [50];
	char *directorios[50]; 
	char *dir=malloc(50*sizeof(char));
	char *diraux=malloc(50*sizeof(char));
	char *raiz=malloc(50*sizeof(char));
	
	printEncabezado();
			
	guardarPWDen(raiz);
	
	//Bucle de ingreso de comandos en el prompt
	while( 1 )
	{
		printPrompt();
		
		//Solicitar ingreso de comando
		fgets(comando,50,stdin);
		
		//Dividir comando que ingresa el usuario en palabras simples y guardar en argv
		argc = parse_command(argv, comando, " \n");
		
		//Decidir qué hacer de acuerdo a las palabras simples
		if(argc == 0)
			printAyuda();
		else{
			
			//1) Salir con exit-------
			
			if( !strcmp(comando,"exit") ){
				printf("--------------------Fin del programa----------------------------\n\n\n");
				return 0;
			}
			
			//2) Cambiar de directorios con cd-------
			
			else if(  !strcmp(argv[0],"cd") ){
				//Home
				if( argc<=1 )
					chdir("/home");
				else{
					//Raiz
					if( !strcmp(argv[1],"/") )
						chdir("/");
					//Nivel superior
					else if( !strcmp(argv[1],"..") ) {
						guardarPWDen(dir);
						subirNivelDeCarpeta(dir);
						chdir( dir );
					}
					//Absoluta
					else if( !strncmp(argv[1],"/",1) ) {
						free(dir);
						dir = malloc(50*sizeof(char));
						strcat( dir,argv[1] );
						chdir( dir );
					}
					//Relativa
					else if( strncmp(argv[1],"/",1) ) {
						guardarPWDen(dir); 
						strcat( dir,"/" );
						strcat( dir,argv[1] );
						chdir( dir );
					}
					else{
						printAyudaCd();
					}
				}
			}//end cd
			
			//3) Ejecutar un archivo indicado-------
			
			//Si es una ruta absoluta ejecutar desde raiz
			else if( !strncmp(argv[0],"/",1) )
				ejecutarDesdeRutaAbsoluta(argv, argc);
			//Si es una ruta relativa ejecutar desde el directorio actual
			else if( strlen(argv[0])>2 && !strncmp(argv[0],"./",2))
				ejecutarDesdeRutaRelativa(argc, argv);
			//Si es una ruta relativa ejecutar desde un directorio superior
			else if( strlen(argv[0])>3 && !strncmp(argv[0],"../",3)  ){		
				guardarPWDen(dir);			
				subirNivelDeCarpeta(diraux);
				ejecutarDesdeRutaRelativa(argc, argv);
				chdir(dir);
			}
			//Sino intentar ejecutar desde los distintos directorios de PATH
			else
				ejecutarDesdePATH(directorios, argv, argc);
			//end run
		 }
	
	}//end while
	

	
	return 0;
}
