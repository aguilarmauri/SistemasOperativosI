#define BUFFSIZE 512

void printHOSTNAME(){
		FILE *f;
		char buffer[BUFFSIZE+1];
		f=fopen("/proc/sys/kernel/hostname","r");
		fgets(buffer, BUFFSIZE+1, f);
		int i;
		for(i=0; i<strlen(buffer); i++)
			if(buffer[i]!=' ' && buffer[i]!='\n')
				printf("%c",buffer[i]);
		//printf("%s",buffer);
		fclose(f);
}

void printFECHAYHORA(){
		FILE *f;
		char buffer[BUFFSIZE+1];
		int hora, min, seg, ano, mes, dia;
		
		f=fopen("/proc/driver/rtc","r");
		fgets(buffer, BUFFSIZE+1, f);
		sscanf(buffer, "%*s       %*c %d%*c%d%*c%d",&hora,&min,&seg);
		fgets(buffer, BUFFSIZE+1, f);
		sscanf(buffer, "%*s       %*c %d%*c%d%*c%d",&ano,&mes,&dia);
		
		printf("\n   >Fecha: %d/%d/%d", dia, mes, ano);
		printf("   >Hora: %d:%d:%d", hora, min, seg);
		
		printf("\n");
		fclose(f);
}

void printEncabezado(){
		printf("\n\n\n-------------------Inicio del programa--------------------------");
		printf("\nAutores: Aguilar Mauricio, Cazajous Miguel");
		printf("\nMateria: Sistemas Operativos I");
		printf("\nTrabajo Practico Nº 2");
		printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
		//printf("\n>Maquina: ");
		//printHOSTNAME();
		printFECHAYHORA();
		//printf("\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
}



void printAyuda(){
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
	printf("\nAyuda: Los comandos adecuados son cd, <ruta_de_archivo>, y exit\n");
	
	/*EDITAR
	printf("\n	cd para obtener la siguiente información adicional:\n ");
	printf("\n		Tiempo de CPU,"); 
	printf("\n		Nro. de Cambios de Contexto, ");
	printf("\n		Fecha y hora de inicio de la maquina, ");
	printf("\n		Nro de Procesos Creados,\n");
	
	printf("\n	exit a b para obtener la siguiente información adicional :\n ");
	printf("\n		Nro. de Peticiones a Disco,"); 
	printf("\n		Memoria Disponible y Memoria Total, ");
	printf("\n		Promedio de Carga de 1 minuto VER, ");
	printf("\n		Esta información se actualizará cada a segundos durante b segundos\n");
	*/
}

void printComandos(int argc, char *argv[]){
		int i;
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
		printf("Numero de Comandos: %d",argc);
		for(i=0; i<argc; i++){
			printf("Argumento %d: %s",argc,argv[i]);
		}
}

void printDirectorios(char *directorios[]){
	printf("\nImprimiendo Directorios: ");
	int i;
	for(i=0; i<20; i++){
		if(directorios[i] == NULL)
			break;	
		printf("\nDirectorio-%d:__%s__",i,directorios[i]);	
	}
}

void printPWD(){
	FILE *fp; 
	char buffer[BUFFSIZE+1];
	fp = popen( "pwd", "r");
	fgets(buffer, 512, fp);
	
	int i;
	for(i=0; i<strlen(buffer); i++)
		if(buffer[i]!=' ' && buffer[i]!='\n')
			printf("%c",buffer[i]);
}

void printUser(){
	FILE *fp; 
	char buffer[BUFFSIZE+1];
	fp = popen( "whoami", "r");
	fgets(buffer, 512, fp);
	
	int i;
	for(i=0; i<strlen(buffer); i++)
		if(buffer[i]!=' ' && buffer[i]!='\n')
			printf("%c",buffer[i]);
}

void printPrompt(){
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
	//system("color 07");
	printf("\n");
	printUser();
	printf("@");
	printHOSTNAME();
	printf(": ");
	printPWD();
	printf("> ");
}

void printAyudaCd(){
		printf("\nEl directorio no existe.");
		printf("\nAyuda del comando cd: ");
		printf("\n\t\tCambiar a Home: cd");
		printf("\n\t\tCambiar a Raíz: cd /");
		printf("\n\t\tCambiar a nivel superior: cd ..");
}
