#include <sys/socket.h>
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
	//printf("%s",name);
	//printf("FILENAMafkjhdsflahdf;aE: %s\n", name);
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
