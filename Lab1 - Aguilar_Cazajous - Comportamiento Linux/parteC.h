void imprimirTiempoCpu(){
	printf("\nTiempo de CPU: ");
	FILE *f;	
	int tiempo1, tiempo2, tiempo3;
	char buffer[BUFFSIZE+1];
	f=fopen("/proc/stat","r");	
	fgets(buffer, BUFFSIZE+1,f);
	//cpu  75980 630 17055 650452 6480 0 270 0 0 0
	/* %*d %*d %*d %*d %*d %*d*/
	sscanf(buffer, "%*s  %d %*d %d %d", &tiempo1, &tiempo2, &tiempo3);
	printf("\n\tTiempo de usuario: %d\n\tTiempo de sistema: %d\n\tTiempo inactivo: %d", tiempo1, tiempo2, tiempo3);
}

void imprimirCambiosDeContexto(){
		printf("\n");
		char cambios[50];
		int i;
		
		FILE *f; 
		char buffer[BUFFSIZE+1]; 
		char c;
		int n=0;
		f = fopen("/proc/stat","r");
		
		fgets(buffer, BUFFSIZE+1, f);
		while(buffer[0]!='c' || buffer[1]!='t' || buffer[2]!='x' || buffer[3]!='t')
			fgets(buffer, BUFFSIZE+1, f);

		i=5;
		while(buffer[i]!=' ' && i<30){
			cambios[i-5] = buffer[i];
			i++;
		}
		for(i; i<50; i++)
			cambios[i++] = ' ';
		
		printf("\nCambios de Contexto: %s",cambios);
		fclose(f);
}

void imprimirFechayHoraInicio(){
	printf("\nFecha y hora de inicio del sistema: ");
	FILE *fp;
	char buffer[BUFFSIZE+1]; 
	fp = popen( "who -b", "r");
	fgets(buffer, BUFFSIZE+1, fp);
	
	int i;
	i=30;
	//"         arranque del sistema 2014" <<30caracteres
	for(i; i<46; i++){
		buffer[i-30] = buffer[i];
		printf("%c",buffer[i]);
	}
	
	//Opcion: system("who -b");
}

void imprimirNroProcesos(){
		printf("\n");
		char procesos[50];
		int i;
		
		FILE *f; 
		char buffer[BUFFSIZE+1]; 
		char c;
		int n=0;
		f = fopen("/proc/stat","r");
		
		fgets(buffer, BUFFSIZE+1, f);
		while(buffer[0]!='p' || buffer[1]!='r' || buffer[2]!='o' || buffer[3]!='c' || buffer[4]!='e')
			fgets(buffer, BUFFSIZE+1, f);
		
		//processes 2949
		i=10;
		while(buffer[i]!=' ' && i<30){
			procesos[i-10] = buffer[i];
			i++;
		}
		for(i; i<50; i++)
			procesos[i++] = ' ';
		
		printf("\nProcesos: %s",procesos);
		fclose(f);
}
