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
#define DIRECTORY "/home/loe/Edu/C/ProgrammingInUNIXCourse/Net/test"

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
	close(socket_descriptor);
	}
	free(fullpath);
	return 0;
}
clude <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <netdb.h>

#define BUF_SIZE 1024
#define CLOSE_CMD "q"

size_t meta;
int socket_descriptor;
int result;
char *filename = NULL;
char **filenames;
int status, sock;
struct addrinfo hints;
struct addrinfo *res;  // will point to the results

void read_write(int r_descriptor, int w_descriptor, int fsize) {
	char buff[fsize];
	char * ptr;
	int result, out, byte;
	do {
		result = read(r_descriptor, buff, fsize);
		ptr = buff;
		byte = result;
		while (result){
			out = write(w_descriptor, ptr, result);
			result -= out;
			ptr += result;
		} 
	} while (byte);
	printf("Sent file \n");
}

void send_file(char *name, int w_descriptor) {

	int file_descriptor, written, name_size, fsize;
	size_t name_lenght;
	struct stat file_stat;
	file_descriptor = open(name, O_RDONLY);
	if (file_descriptor < 0) {
		perror("%s");
		return;
	}
	name_lenght = strlen(name);
	
	if (fstat(file_descriptor, &file_stat) == -1) {
		fprintf(stderr, "fstats: can't access %s\n", name);
		return;
	}
	name_size = send(w_descriptor, &name_lenght, sizeof(name_lenght), 0);
	if (name_size < 0) {
		perror("");
	}

	written = send(w_descriptor, name, (name_lenght)*sizeof(char), 0);
	printf("%s\n", name);

	if (written <0) {
		perror("");
	}
	fsize = send(w_descriptor, &file_stat.st_size, sizeof(file_stat.st_size), 0);
	if (fsize < 0 ) {
		perror("");
	}
	printf("sending: %s\n", name);
	read_write(file_descriptor, w_descriptor, fsize);
	close(file_descriptor);
}

void recieve_filenames(w_descriptor) {
	int i = 0, j = 0, is_file_name = 0, file_name_len = 0, f_names_len = 0, write_desc;
	char c;
	filenames = (char **)malloc(sizeof(char *));
	
	while ((c = getchar()) != EOF) {

		if (isspace(c)) {
			if (!is_file_name) {
				continue;
			} else {

				filename[file_name_len] = '\0';
				is_file_name = file_name_len = 0;
				filenames[f_names_len++] = filename;
				filenames = (char **)realloc(filenames, sizeof(char *)*f_names_len+1);
				filename = NULL;
			}
		}
		else {
			if (!is_file_name) {
				is_file_name = 1;
				filename = (char *)malloc(sizeof(char));
			}
			filename[file_name_len++] = c;
			filename = (char *)realloc(filename, sizeof(char)*file_name_len+1);
		}
	}

	for (j = 0; j < f_names_len; ++j) {	
		send_file(filenames[j], w_descriptor);
	}

	free(filenames);
	free(filename);
	
}


int main(int argc, char *argv[]){
	if (argc < 3) {
		fprintf(stderr, "client usage: %s hostname port", argv[0]);
		exit(0);
	}

	// get ready to connect
    if ((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }
	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;

	socket_descriptor = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (socket_descriptor == -1){
		perror("get socket_descriptor");
		exit (1);
	}
	sock = connect(socket_descriptor, res->ai_addr, res->ai_addrlen);
	if (-1 == sock) {
		perror("on connect");
		exit(1);
	}
	printf("Please enter filenames you want to send to server: ");
	while (1) {

		
	recieve_filenames(socket_descriptor);

	}; 
	close(socket_descriptor);
	return 0;
}
