#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "parteB.h"
#include "parteC.h"
#include "parteD.h"

void imprimirHOSTNAME(){
		//printf("\n");
		FILE *f1;
		char buffer1[BUFFSIZE+1];
		f1=fopen("/proc/sys/kernel/hostname","r");
		fgets(buffer1, BUFFSIZE+1, f1);
		printf("            %s",buffer1);
		fclose(f1);
} 

void imprimirFECHAYHORA(){
		printf("\n");
		FILE *f;
		char buffer[BUFFSIZE+1];
		int hora, min, seg, ano, mes, dia;
		
		f=fopen("/proc/driver/rtc","r");
		//rtc_date       : 2014-08-22
		fgets(buffer, BUFFSIZE+1, f);
		sscanf(buffer, "%*s       %*c %d%*c%d%*c%d",&hora,&min,&seg);
		fgets(buffer, BUFFSIZE+1, f);
		sscanf(buffer, "%*s       %*c %d%*c%d%*c%d",&ano,&mes,&dia);
		
		printf("   Hora: %d:%d:%d", hora, min, seg);
		printf("   Fecha: %d/%d/%d\n", dia, mes, ano);
		
		fclose(f);
}

void imprimirEncabezado(){
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
		printf("\n>Maquina: ");
		imprimirHOSTNAME();
		printf("\n>Fecha y Hora actuales: ");
		imprimirFECHAYHORA();
}

void parteB(){
			//PARTE B
			imprimirCPU();
			imprimirKernel();
			imprimirUptime();
			imprimirFileSystem();
}

void parteC(){
			//PARTE C
			printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
			imprimirTiempoCpu();
			imprimirCambiosDeContexto();
			imprimirFechayHoraInicio();
			imprimirNroProcesos();
} 

void parteD(){
			//PARTE D
			printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
			imprimirPeticiones();
			imprimirMemoria();
			imprimirPromedioCarga();
}

void imprimirComandos(){
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
	printf("\nLos comandos adecuados son: \n");
	
	printf("\n	-s para obtener la siguiente información adicional:\n ");
	printf("\n		Tiempo de CPU,"); 
	printf("\n		Nro. de Cambios de Contexto, ");
	printf("\n		Fecha y hora de inicio de la maquina, ");
	printf("\n		Nro de Procesos Creados,\n");
	
	printf("\n	-l a b para obtener la siguiente información adicional EDITAR:\n ");
	printf("\n		Nro. de Peticiones a Disco,"); 
	printf("\n		Memoria Disponible y Memoria Total, ");
	printf("\n		Promedio de Carga de 1 minuto VER, ");
	printf("\n		Esta información se actualizará cada a segundos durante b segundos\n");
}

int main(int argc, char *argv[]){
	
	printf("%d",argc);
	printf("\n\n\n\n\n-------------------Inicio del programa--------------------------");
	printf("\nAutores: Cazajous Miguel, Aguilar Mauricio");
	printf("\nMateria: Sistemas Operativos I");
	printf("\nTrabajo Practico Nº 1\n");
	int i, a, b;
	
	imprimirEncabezado();
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
	
	//Si hay demasiados parametros (>4) o si hay parametros pero el primero no es ni -s ni -l, imprimir comandos
	if(argc>4 || (argc>=2 && ((argv[1][0]!='-' && argv[1][1]!='s') || (argv[1][0]!='-' && argv[1][1]!='l'))))
		imprimirComandos();
	//Imprimir siempre parte B
	else{
		parteB();		
		if(argc>=2){
			//Si el primer parametro es -s, imprimir la parte C
			if(argv[1][0]=='-' && argv[1][1]=='s')
				parteC();
			//Si el primer parametro es -l, imprimir la parte D, cada a segundos durante b segundos
			else if(argc==4 && argv[1][0]=='-' && argv[1][1]=='l'){
				i = 0;
				a = atoi(argv[2]);
				b = atoi(argv[3]);
				b = b/a;
				if(a>0 && b>0){
					parteC();
					for(i; i<b; i++){
						sleep(a);
						parteD();
					}
				}

			}
		}
	}
								
	printf("--------------------Fin del programa----------------------------\n\n\n\n\n");

	return 0;
}
