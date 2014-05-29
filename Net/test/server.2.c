#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define CLOSE_CMD "q"
#define PERMISSIONS 0777
#define DIRECTORY "/home/loe/Edu/C/ProgrammingInUNIXCourse/Net/test/"

size_t meta;
int socket_descriptor;
struct sockaddr_in server_address;
int PORT = 9000;
int res, client_descriptor;
char buf[BUF_SIZE];
int result;
size_t bleft, blleft;
int copy_descriptor, w;
char *fullpath;

int main(){
	char *file_name;
	size_t file_name_size, file_size;
	int fnsize, fsize, stop_signal;
	fullpath = NULL;
	fullpath = (char *)realloc(fullpath, strlen(DIRECTORY));
	/* creating directory for copies */
	mkdir(DIRECTORY, S_IRWXU | S_IRWXG | S_IRWXO);
	socket_descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (socket_descriptor == -1){
		perror("In getting socket descriptor");
		exit (1);
	}
	memset( (void *) &server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(PORT);
	if ((res = bind(socket_descriptor, (struct sockaddr *) &server_address, sizeof(server_address))) < 0) {
		perror("In bind socket descriptor");
		exit(1);
	}
	listen(socket_descriptor, 1);
	while (1) {
		client_descriptor = accept(socket_descriptor, NULL, NULL);
		do {
			file_name = NULL;
			result = 0;
			if ((fnsize = read(client_descriptor, &file_name_size, sizeof(size_t))) < 0) {
				perror("sadness");
			}
			file_name = (char*)realloc(file_name, file_name_size);
			while (result < file_name_size) {
				if ((result += recv(client_descriptor, file_name, file_name_size, 0)) < 0) { 
					perror("In trying to get file name");
				}
			}
			printf("recieving: %s\n", file_name);
			fullpath = (char *)realloc(fullpath, strlen(DIRECTORY)+file_name_size);
			sprintf(fullpath, "%s%s", DIRECTORY, file_name);
			printf("%s\n", fullpath);
			if ((copy_descriptor = open(fullpath, O_CREAT | O_WRONLY, PERMISSIONS)) == -1) {
				perror("in create file ");
			}
			if ((fsize = read(client_descriptor, &file_size, sizeof(size_t))) < 0 ) {
				perror("sadness in getting file size ");
			}

			stop_signal = file_size;
			printf("initial stop_signal %d\n", stop_signal);
			while (stop_signal > 0) {
				bleft = (stop_signal - BUF_SIZE > 0) ? BUF_SIZE : stop_signal;
				printf("count bleft %lu\n", bleft);
				if ((result = read(client_descriptor, buf, bleft)) < 0) {
					perror("In read client descriptor");
					break;
				}
				printf("read file %d\n", result);
				blleft = result;
				while (blleft > 0) {
					if ((w = write(copy_descriptor, buf, result)) < 0 ) {
						perror("In write copy");
						break;
					}
					blleft -= w;
					printf("written file %lu\n", blleft);
				}
				
				// bleft -= result;
				stop_signal -= bleft;
				printf("rest of stop_signal %d\n", stop_signal);
			}
			printf("Recieved %s\n", file_name);
			if (close(copy_descriptor) == -1){
				perror("In close copy_descriptor");
			}
			// while (bleft > 0) {
			// 	sscanf(buf, "%s", cmd);
			// }; 
		} while (1);
	printf("Bye!\n");
	shutdown(client_descriptor, SHUT_RDWR);
	close(client_descriptor);
	close(socket_descriptor);

	}
	free(fullpath);
	return 0;
}
