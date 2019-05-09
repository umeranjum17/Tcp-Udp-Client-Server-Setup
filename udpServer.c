#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#define segmentSize 1024

int main()
{
  struct sockaddr_in AddressS,AddressC;
  int size;

  int i, sskt, connected, bytesReceived, enable = 1;  
  char buffer[segmentSize+1];

  FILE *file;
  size_t fileSize, totalBytesRead;
  if ((sskt = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("server socket");
    exit(1);
  }

  AddressS.sin_family = AF_INET;         
  AddressS.sin_port = htons(5500);     
  AddressS.sin_addr.s_addr = INADDR_ANY; 
  bzero(&(AddressS.sin_zero),8); 

  printf("Wait connecting...\n");

  if (bind(sskt, (struct sockaddr *)&AddressS, sizeof(struct sockaddr))
    == -1) {
    perror("binding error");
  exit(1);
}


size = sizeof(struct sockaddr_in);

file = fopen("copy.txt", "w");
if (file == NULL) {
  printf("\nerror\n");
  exit(1);
}
if (ferror(file)) {
  printf("\nerror\n");
  exit(1);
}
bytesReceived = -1;
recvfrom(sskt, buffer, segmentSize, 0, (struct sockaddr *) &AddressC,&size);
sscanf(buffer, "%ld\n", &fileSize);
printf("\nfile Size: %ld bytes\n", fileSize);
totalBytesRead = 0;
printf("\nReceiving.\n");

while (totalBytesRead < fileSize)
{
  bytesReceived = recvfrom(sskt, buffer, segmentSize, 0, (struct sockaddr *) &AddressC,&size);
  
  if (bytesReceived < 1) break;
  buffer[bytesReceived] = '\0';
  fputs(buffer, file);
  totalBytesRead += bytesReceived;
}
printf("\nFinished.\n");

fclose(file);
close(connected);

close(sskt);
return 0;
} 

