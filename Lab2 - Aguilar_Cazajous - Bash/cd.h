#define BUFFSIZE 512

void guardarPWDen(char *dir){
	FILE *fp; 
	char buffer[BUFFSIZE+1];
	fp = popen( "pwd", "r");
	fgets(buffer, 512, fp);	
	sscanf(buffer, "%s", dir);
}

int parse_command(char *palabras[], char comandos[], char *separador/*, int cant*/){
	int i;
	
	for(i=0; i<8; i++)
		palabras[i]=malloc(100*sizeof(char));
	palabras[0] = strtok(comandos,separador);
	for(i=1; i<20; i++){
		palabras[i] = strtok(NULL,separador);
		if(palabras[i] == NULL)
			break;
	}
	
	if(palabras[0] == NULL)
		i=0;
	
	return i;
}

int updateDirectorios(char *directorios[]){
	FILE *fp; 
	char buffer[512];
	
	//Guardar Variable de Entorno PATH en buffer
	fp = popen( "echo $PATH", "r");
	fgets(buffer, 512, fp);
	//Dividir y retornar la cantidad de directorios
	return parse_command(directorios, buffer, ":");
}

void subirNivelDeCarpeta(char *dir){
	int i, posUltimaBarra;
	char *aux=malloc(50*sizeof(char));
	for(i=0; i<strlen(dir); i++)
		if(dir[i]=='/')
			posUltimaBarra = i;
	
	for(i=0; i<strlen(dir); i++)
		if(i<posUltimaBarra)
			aux[i] = dir[i];
		else
			break;
	sscanf(aux,"%s",dir);
}
