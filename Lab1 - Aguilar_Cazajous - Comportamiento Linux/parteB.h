#define BUFFSIZE 256

void imprimirCPU(){
		printf("\nProcesador \n"); 
		FILE *f3; 
		char buffer3[BUFFSIZE+1]; 
		f3 = fopen("/proc/cpuinfo","r"); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		printf("	%s",buffer3); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		fgets(buffer3, BUFFSIZE+1, f3); 
		printf("	%s\n",buffer3); 
		fclose(f3); 
}

void imprimirKernel(){
		FILE *f4; 
		char buffer4[BUFFSIZE+1]; 
		f4 = fopen("/proc/version_signature","r"); 
		fgets(buffer4, 6, f4); 
		printf("Kernel: %s\n",buffer4); 
		fclose(f4);
}

void imprimirUptime(){
		FILE *f5; 
		char buffer5[BUFFSIZE+1]; 
		f5 = fopen("/proc/uptime","r"); 
		fgets(buffer5, BUFFSIZE+1, f5); 
		int a=1;
		int i=0;
		int cantidad=0;	
		double punto=0; 						
		while(a==1)
		{
		if(buffer5[i]==46)
			punto=i;
			
		else if(buffer5[i]==32)
			{	
				cantidad=i;
				a=-1;
			}
			i++;	
		}
		i=0;	
		double aux=0;

		while(i<cantidad)
		{
			if(buffer5[i]!=46 && punto>0)
				aux+=(buffer5[i]-48)*pow(10.0,punto-1);	
			if(buffer5[i]!=46 && punto<0)
				aux+=(buffer5[i]-48)*pow(10.0,punto);
			punto--;
			i++;
		}
		int horas=aux/3600;
		int minutos=(aux-horas*3600)/60;
		double segundos=(aux-horas*3600-minutos*60);

		printf("Uptime-HH/MM/SS-: %d:%d:%.2f",horas, minutos, segundos); 
		fclose(f5);
}


void imprimirFileSystem(){
		FILE *f; 
		char buffer[BUFFSIZE+1]; 
		char c;
		int n=0;
		f = fopen("/proc/filesystems","r"); 
		do{
			c=fgetc(f);
			if(c=='\n')
			n++;
		}while(c!=EOF);
		fgets(buffer, BUFFSIZE+1, f); 
		printf("\n\nSistemas Soportados: %d\n",n); 
		fclose(f);
		//system("wc -l /proc/filesystems");
}
