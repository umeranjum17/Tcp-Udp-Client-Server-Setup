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


int main()

{
  struct hostent *host;
  struct sockaddr_in AddressS; 

  int s;  
  char buffer[chunksize+1];
  
  FILE *fp;
  size_t fileSize, nRead, size;


  host = gethostbyname("localhost");

  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("Here is udp client socket");
    exit(1);
  }

  AddressS.sin_family = AF_INET;     
  AddressS.sin_port = htons(5500);   
  AddressS.sin_addr = *((struct in_addr *)host->h_addr);
  bzero(&(AddressS.sin_zero),8); 

  fp = fopen("test.txt", "r");

  if (fp != NULL) {
    if (ferror(fp)) {
        printf("\nThere is an error\n");
	close(s);
	exit(1);
    }
    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    printf("\nClient file size: %ld bytes\n", fileSize);
    rewind(fp);
    sprintf(buffer, "%ld\n", fileSize);


    size = sizeof(struct sockaddr_in);
    if (sendto(s, buffer, chunksize, 0, (struct sockaddr *) &AddressS, size) == -1) {
	
  perror("sendto()");
	return 1;
    }
    printf("\nsending...\n");
    while ((nRead = fread(buffer, 1, sizeof buffer-1, fp)) > 0) {
    	 buffer[nRead] = '\0';
	 if (sendto(s, buffer, strlen(buffer), 0, (struct sockaddr *) &AddressS, size) == -1) {
		perror("sendto()");
		return 1;
	   }
    }
    fclose(fp);
    printf("\nFinished\n");
  }
  else {
  	printf("\nerror.\n");
  }
  close(s);  
 return 0;
}
