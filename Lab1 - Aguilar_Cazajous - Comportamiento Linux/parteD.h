void imprimirPeticiones(){
	//printf("\n");
	char peticiones[50];
	int i, pet1, pet2;
	
	FILE *f; 
	char buffer[BUFFSIZE+1]; 
	char c;
	int n=0;
	f = fopen("/proc/diskstats","r");
	
	fgets(buffer, BUFFSIZE+1, f);
	while(!strstr(buffer, "sda"))
		fgets(buffer, BUFFSIZE+1, f);
	
	sscanf(buffer,"%*d       %*d %*s %d %*d %*d %*d %d",&pet1,&pet2);
	printf("\nPeticiones a disco: %d\n",pet1+pet2);
	fclose(f);
}

void imprimirMemoria(){
		printf("\nMemoria Libre/Total: ");
		//Mem:          970M       841M       129M        11M        30M       327M
		int memTotal, memLibre;		
		FILE *fp; 
		char buffer[BUFFSIZE+1];
		fp = popen( "free -h", "r");
		fgets(buffer, BUFFSIZE+1, fp);
		fgets(buffer, BUFFSIZE+1, fp);
		sscanf(buffer,"%*s%*c          %d%*c       %*d%*c       %d%*c",&memTotal,&memLibre);
		printf(" %dM/%dM\n",memLibre,memTotal);
		
		//printf("	Memoria Disponible/Total: %s\n",mem); 
}

void imprimirPromedioCarga(){
		printf("\n");
		int v1,v2;
		FILE *f;
		char buffer[BUFFSIZE+1];
		f=fopen("/proc/loadavg","r");
		fgets(buffer, BUFFSIZE+1, f);
		sscanf(buffer,"%d%*c%d",&v1,&v2);
		printf("Promedio de Carga:  %d.%d\n",v1,v2);
		fclose(f);
}
