#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 1024
#define CLOSE_CMD "q"
#define PERMISSIONS 0777
#define DIRECTORY "/home/loe/Edu/C/ProgrammingInUNIXCourse/Net/"

size_t meta;
int socket_descriptor;
struct sockaddr_in server_address;
int PORT = 9000;
int res, client_descriptor;
char buf[BUF_SIZE];
int result = 0;
size_t bleft, blleft;
int copy_descriptor, w;
char *fullpath;

int main(){
	char *file_name = NULL;
	size_t file_name_size, file_size;
	int fnsize, fsize, fd;
	fullpath = (char *)malloc(sizeof(char *));
	fullpath = (char *)realloc(fullpath, strlen(DIRECTORY));
	/* creating directory for copies */
	mkdir(DIRECTORY, S_IRWXU | S_IRWXG | S_IRWXO);
	socket_descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (socket_descriptor == -1){
		perror(NULL);
		exit (1);
	}

	//file_name = (char *)malloc(sizeof(char *));
	memset( (void *) &server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(PORT);
	if ((res = bind(socket_descriptor, (struct sockaddr *) &server_address, sizeof(server_address))) < 0) {
		perror(NULL);
		exit(1);
	}
	listen(socket_descriptor, 1);

	while (1) {
		client_descriptor = accept(socket_descriptor, NULL, NULL);
		do {

			if ((fnsize = read(client_descriptor, &file_name_size, sizeof(size_t))) < 0) {
				perror("sadness ");
			}

			printf("%lld",file_name_size);	
			file_name = (char*)realloc(file_name, file_name_size);
			printf("BUGAGA! %с\n", file_name[0]);
			while (result < file_name_size) {
				printf("%d\t",result);
				if ((result += recv(client_descriptor, file_name, file_name_size, 0)) < 0) { 
					perror("trying to get file name ");
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

			bleft = file_size;
			while (bleft > 0) {
				if ((result = read(client_descriptor, buf, file_size)) < 0) {
					perror("1");
					break;
				}
				printf("%d\n", result);
				blleft = result;
				while (blleft > 0) {
					if ((w = write(copy_descriptor, buf, result)) < 0 ) {
						perror("2");
						break;
						// continue;
					}
					blleft -= w;
				}
				
				bleft -= result;
			}
			printf("Recieved %s\n", file_name);
			if (close(copy_descriptor) == -1){
				perror("");
			}
			// while (bleft > 0) {
			// 	sscanf(buf, "%s", cmd);
			// }; 
		} while (1);
	printf("Bye!\n");
	shutdown(client_descriptor, SHUT_RDWR);
	close(client_descriptor);

	}
	free(fullpath);
	return 0;
}
