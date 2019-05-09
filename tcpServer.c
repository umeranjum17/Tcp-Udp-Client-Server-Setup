#include <arpa/inet.h>
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
    struct sockaddr_in AddressS,AddressC;
  int sizen;

  int i, sock, connected, enable = 1;  
  

  char mainbuff[chunksize+1];


  FILE *file;

  size_t fileSize = 0, x = 0, readtotal = 0;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
  {
    perror("server socket");
    exit(1);
  }

  if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int)) == -1) 
  {
    perror("server--setsockopt");
    exit(1);
  }
 AddressS.sin_addr.s_addr = INADDR_ANY; 
  bzero(&(AddressS.sin_zero),8); 
  AddressS.sin_family = AF_INET;         
  AddressS.sin_port = htons(5000);     
  
  if (bind(sock, (struct sockaddr *)&AddressS, sizeof(struct sockaddr))
    == -1) {
    perror("binding error ");
  exit(1);
}

if (listen(sock, 5) == -1) {
  perror("for server listen");
  exit(1);
}

printf("\nwaiting for a connection\n");
fflush(stdout);

sizen = sizeof(struct sockaddr_in);
connected = accept(sock, (struct sockaddr *) &AddressC,&sizen);

printf("\nconnection established on (%s, %d).\n",inet_ntoa(AddressC.sin_addr),ntohs(AddressC.sin_port));

file = fopen("copy.txt", "w");
if (file == NULL) {
  printf("\nError");
  exit(1);
}
if (ferror(file)) 
{
  printf("\nError");

  exit(1);
}

x = -1;
recv(connected,mainbuff,chunksize,0);
sscanf(mainbuff, "%ld\n", &fileSize);
printf("\nTCP Server - File Size: %ld bytes\n", fileSize);
readtotal = 0;
printf("\nTCP Server - Receiving file...\n");
while (readtotal < fileSize)
{
  x = recv(connected,mainbuff,chunksize,0);
  if (x < 1) break;

  mainbuff[x] = '\0';
  
  fputs(mainbuff, file);
  readtotal += x;
  
}
printf("\ncompleted\n");

fclose(file);
close(connected);

close(sock);
return 0;
} 
