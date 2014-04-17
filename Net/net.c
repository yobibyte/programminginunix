#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h>
      
#define BUF_SIZE 1024



int main(int argc, char *argv[]) {
  int socket_descriptor;
  struct sockaddr_in server_address;
  int PORT = 4040;
  int ADDRESS;
  int res;
  int client_descriptor;
  char buf[BUF_SIZE];



  socket_descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);

  if (socket_descriptor == -1){	
    perror(NULL);
    exit (1);			
  }

//  bzero( (void *) &server_address, sizeof(server_address));
 
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(PORT);
  res = bind(socket_descriptor, (struct sockaddr*) &server_address, sizeof(server_address));
  if (-1 == res) {
    perror(NULL);	
    exit(1);			
  } 

  listen(socket_descriptor, 1);
  int result;
  while (1) {	
    client_descriptor = accept(socket_descriptor, NULL, NULL);		
    result = read(client_descriptor, buf, BUF_SIZE);			
    buf[result] = 0;
    puts(buf);		
    shutdown(client_descriptor, SHUT_RDWR);				
    close(client_descriptor);						
  }
  return 0;
}
