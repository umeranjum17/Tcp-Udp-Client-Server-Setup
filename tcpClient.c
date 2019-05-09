#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>

#define chunksize 1024

int main(){

  struct hostent *myhost;
  struct sockaddr_in AddressS;  
  myhost = gethostbyname("127.0.0.1");
  char mainbuff[chunksize+1];
  int s;  

  FILE *fil;
  size_t fsize, readno;

  if( (s = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
  {
    
    perror("Here is tcp client socket ");
    

    exit(1);
    
  }
AddressS.sin_port = htons(5000);   
  AddressS.sin_addr = *((struct in_addr *)myhost->h_addr);
  
  AddressS.sin_family = AF_INET;     
     bzero(&(AddressS.sin_zero),8); 

  if (connect(s, (struct sockaddr *)&AddressS,sizeof(struct sockaddr)) == -1) 
  {
    perror("Here is tcp client connect");
    


    exit(1);
  }

  fil = fopen("test.txt", "r");

  if (fil != NULL) {
       if (ferror(fil)) {
        printf("\nThere is an error\n");
	close(s);
	

  exit(1);
    }

    fseek(fil, 0L, SEEK_END);
    fsize = ftell(fil);
     
    //showing all file size     
    printf("\nClient file size : %ld bytes\n", fsize);
    rewind(fil);


    sprintf(mainbuff, "%ld\n", fsize);
    send(s,mainbuff,strlen(mainbuff), 0);
    

    printf("\nSending now.....\n");
    while ((readno = fread(mainbuff, 1, sizeof mainbuff-1, fil)) > 0){
    	mainbuff[readno] = '\0';    
    	
      send(s,mainbuff,strlen(mainbuff), 0);


    }

    fclose(fil);
    printf("\nfinished.\n");
  
  }
  
  else {
  	printf("\nerror\n");
  }
  close(s);  
 return 0;
}
