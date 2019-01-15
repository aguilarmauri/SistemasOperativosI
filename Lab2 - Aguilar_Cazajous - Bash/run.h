#include <sys/wait.h>
#include <sys/types.h>
#define TRUE 1
#define FALSE 0

void salida(char nombreArchivo[]){
	int fid;
	int banderas,permiso;
	banderas = O_WRONLY|O_CREAT|O_TRUNC;
	permiso = S_IWUSR|S_IRUSR;
	
	fid = open(nombreArchivo, banderas, permiso);	
	if (fid<0) {
		perror("open");
		exit(1);
	}
	close(STDOUT_FILENO);
	if (dup(fid)<0) {
		perror("dup");
		exit(1);
	}
	close(fid);
}

int checkRedir(char* argv[], char nombreArchivo[]){
	int i;
	for (i = 0; i < 20; i++){
			
		if(argv[i] == NULL){
			nombreArchivo = NULL;
			return 0;
		}
		else if (!strcmp(argv[i], "<")){
			strcpy(nombreArchivo, argv[i+1]);
			argv[i] = NULL;		
			argv[i+1] = NULL;
			return 1;
		}
		else if (!strcmp(argv[i], ">")){
			strcpy(nombreArchivo, argv[i+1]);
			argv[i] = NULL;	
			argv[i+1] = NULL;
			return 2;
		}
	}
	return 0;
}

int checkPipe(char* argv[], char* argv1[], char* argv2[]){
	int indexArg, aux, indexArg2;
	
	for(indexArg = 0; argv[indexArg] != NULL; indexArg++){
		aux = strcmp("|", argv[indexArg]);
		if(aux == 0)
			break;
		argv1[indexArg] = (char*) malloc ( strlen(argv[indexArg] + 1) ) ;
		strcpy(argv1[indexArg], argv[indexArg]);
	}
	argv1[indexArg] = '\0';
	

	if(argv[indexArg] == NULL)
		return 0;
	
	indexArg++;
	
	for(indexArg2 = 0; argv[indexArg] != NULL; indexArg2++){
		
		if(argv[indexArg] == NULL)
			break;
		argv2[indexArg2] = (char*) malloc ( strlen(argv[indexArg] + 1) ) ;
		strcpy(argv2[indexArg2], argv[indexArg]);
		indexArg++;
	}
	argv2[indexArg2] = '\0';
	
	return 1;
}

void buscarArchivo(char* arch, char* paths[], char* execPath){
	char returnPath[50];
	int result;
	char searchDir[50] = "";
	char* archivo;
	strcpy(returnPath, arch);
	
	if(arch[0] == '/' || (arch[0] == '.' && arch[1] == '.' && arch[2] == '/')){
		char* dir;
		char* nextDir;
		int pathReady = 0; 
		
		if(arch[0] == '/')
			searchDir[0] = '/';
		dir = strtok(arch,"/");
		nextDir = strtok(NULL,"/");
		
		if(nextDir != NULL) 
			strcat(searchDir,dir);
		else{
			nextDir = dir; 
			pathReady = 1;
		}

		while((nextDir != NULL) && !pathReady){
			dir = nextDir;
			nextDir = strtok(NULL,"/");
			strcat(searchDir,"/");
			if(nextDir != NULL)
				strcat(searchDir,dir);
		}
		archivo = dir;
	}
	
	else if(arch[0] == '.' && arch[1] == '/'){ 
		getcwd(searchDir, 50);
		strcat(searchDir,"/");
		archivo = strtok(arch, "/");
		archivo = strtok(NULL,"/");
	}
	
	else{
	
		int i;
		char aux[50];
		for(i = 0; i < 20; i++){
			if(paths[i] == NULL)
				break;
			strcpy(aux,paths[i]);
			strcat(aux,"/");
			strcat(aux,arch);
			result = access(aux, F_OK);
			if(!result){
				strcpy(execPath, aux);
				return;
			}
		}
		execPath[0] = 'X';
		return;
	}
	
	strcat(searchDir, archivo);
	result = access(searchDir, F_OK);
	if(!result)
		strcpy(execPath, searchDir);
	else
		execPath[0] = 'X';
}

void ejecutarPipeline(char* argv1[], char* argv2[], char* paths[]){
	char ejecutaPath[256];
	
	int fd[2];
	pipe(fd);
	if (fork()==0) { 
		close(fd[0]);
		dup2(fd[1],1); 
		close(fd[1]);
		buscarArchivo(argv1[0], paths, ejecutaPath);
		execv(ejecutaPath, argv1);
		perror(ejecutaPath);
		exit(1);
	} 
	else {
		close(fd[1]); 
		dup2(fd[0],0);
		close(fd[0]);
		buscarArchivo(argv2[0], paths, ejecutaPath);
		execv(ejecutaPath, argv2);
		perror(ejecutaPath);
		exit(1);
	}
}

void ejecutarEnHijo(char *ruta, char *argumentos[], int background){
	int processid;
	siginfo_t *estatus;
	estatus=malloc(1*sizeof(siginfo_t));
	
	int pid, flagWaitPID, bgProcess, pipeExecuted;
	int argC;
	char* argV[20];
	char* argv1[20]; 
	char* argv2[20];
	char comando [256]; 
	
	char hostname [20];
	gethostname(hostname,20);
	char* user;
	user = getlogin();
	
	char *paths[20];
	int pathCounter;
	
	pathCounter = updateDirectorios(paths);	
	flagWaitPID = 0;	  
	pipeExecuted = 0;
	int flagRedirect = 0;
	int doPipe = 0;
	char fileName[50];
	doPipe = checkPipe(argV, argv1, argv2);
	flagRedirect = checkRedir(argumentos, fileName);
	
	processid = fork();
	if (processid == -1) {
	/* tratamiento de error. */
		  perror("fallo en fork");
		  exit(EXIT_FAILURE);
	} else if (processid == 0) {
	/* tratamiento del hijo. */
		if(flagRedirect == 2)
				salida(fileName);
		else if(flagRedirect == 1)					
				freopen(fileName,"r",stdin);
		else if(doPipe == 1){
				ejecutarPipeline(argv1, argv2, paths);
				pipeExecuted = 1;
		}
		if(!pipeExecuted){
				execv(ruta, argumentos);
				perror(ruta);
				exit(1);
		}
	} else  {
	/* tratamiento del padre. */
		flagWaitPID = -1;
	}
	if(background == TRUE)
			waitpid(processid,&flagWaitPID,WNOHANG);
	else
			waitpid(processid,&flagWaitPID,0);
}



void ejecutarEnBackground(char *ruta, char *argumentos[]){
	int background;
	background = TRUE;
	ejecutarEnHijo(ruta, argumentos, background);
}

void ejecutarEnPrimerPlano(char *ruta, char *argumentos[]){
	int background;
	background = FALSE;
	ejecutarEnHijo(ruta, argumentos, background);
}

void ejecutarDesdeRutaAbsoluta(char *argv[], int argc){
	char *argumentos[argc+1];
	int i;
	
	//Si lo quiere ejecutar en background, no incluir el ultimo argumento
	if( !strncmp(argv[argc-1],"&",1) ){		
		for(i=0; i<argc-1; i++)
			argumentos[i] = argv[i];
		argumentos[argc] = NULL;
		ejecutarEnBackground(argumentos[0], argumentos);
	}else{
		argv[argc] == NULL;
		ejecutarEnPrimerPlano(argv[0], argv);
	}
}

void guardarRuta(int argc, char *argv[], char *aux, char *aux1[])
{
	char *diractual=malloc(50*sizeof(char));	
	int i, k=0;
	
	guardarPWDen(diractual);
	
	//Sacar el punto
	for(i=0; i<argc; i++)
		aux1[i]=malloc(100*sizeof(char));
	if(!strncmp(argv[0],"./",2)){ //-->de mas
		for(i=1; i<strlen(argv[0]); i++){
			aux[k]=argv[0][i];
			k++;
		}
	}
	else 						//-->de mas
		aux=argv[0];	
	
	strcat(strcat(aux1[0],diractual), aux);

	aux1[argc]=NULL;
	for(i=1; i<argc; i++)
		strcat(aux1[i],argv[i]);
}

void ejecutarDesdeRutaRelativa(int argc, char *argv[])
{
	int i;
	char *aux = malloc(32*sizeof(char));	
	char *comandos[argc+1], *argumentos[argc+1];
	
	//Si lo quiere ejecutar en background, no incluir el ultimo argumento
	if( !strncmp(argv[argc-1],"&",1) ){
		for(i=0; i<argc-1; i++)
			argumentos[i] = argv[i];
		guardarRuta(argc-1, argumentos, aux, comandos);
		ejecutarEnBackground(comandos[0], comandos);
	}else{
		guardarRuta(argc, argv, aux, comandos);
		ejecutarEnPrimerPlano(comandos[0], comandos);
	}
	
}

void ejecutarDesdePATH(char *directorios[], char *argv[], int argc)
{
	int cantPath;
	cantPath = updateDirectorios(directorios);
	char *rutaActual[cantPath], *archivo = malloc(32*sizeof(char)), *comandos[argc];
	strcpy(archivo,argv[0]);
	int indice, i, ejecucion;
	indice = 0;
	ejecucion = FALSE;
	
	while(indice<cantPath){
		
		cantPath = updateDirectorios(directorios);
		strcat(strcat(directorios[indice],"/"),argv[0]);
		rutaActual[indice] = malloc(32*sizeof(char));		
		strcpy(rutaActual[indice], directorios[indice]);
		
		if(access(directorios[indice], F_OK)!=-1){
			
			//Si lo quiere ejecutar en background, no incluir el ultimo argumento
			if( !strncmp(argv[argc-1],"&",1) ){
				//guardar comandos menos el ultimo
				comandos[argc-1]=NULL;
				for(i=0; i<argc-1; i++)
					if(argv[i]==NULL)
						break;
					else
						comandos[i] = argv[i];
				ejecutarEnBackground(rutaActual[indice], comandos);
			}
			else{
				//guardar comandos
				comandos[argc]=NULL;
				for(i=0; i<argc; i++)
					if(argv[i]==NULL)
						break;
					else
						comandos[i] = argv[i];
				ejecutarEnPrimerPlano(rutaActual[indice], comandos);
			}
			ejecucion = TRUE;	
		}
		indice++;
	}
	if( ejecucion == FALSE )
			printf("El archivo no existe\n");
}


