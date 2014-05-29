#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define CLOSE_CMD "q"

size_t meta;
int socket_descriptor;
int result;
char *filename;
char **filenames;
int status, sock;
struct addrinfo hints;
struct addrinfo *res;  // will point to the results

void read_write(int r_descriptor, int w_descriptor, size_t fsize) {
	char buff[BUF_SIZE];
	char* ptr;
	int result, out, sent_data = 0, counter = 0;
	while (sent_data < fsize) {
		result = read(r_descriptor, buff, BUF_SIZE);
		sent_data += result;
		ptr = buff;
		while (result){
			out = write(w_descriptor, ptr, result);
			result -= out;
			ptr += result;
		}
	}
	printf("Sent file \n");
}
/*
void read_write(int r_descriptor, int w_descriptor, size_t fsize) {
	char buff[BUF_SIZE];
	char * ptr;
	int result, out, sent_data = 0, counter = 0;
	while (sent_data < fsize) {
		counter = result = read(r_descriptor, buff, BUF_SIZE);
		ptr = buff;
		while (result){
			out = write(w_descriptor, ptr, result);
			result -= out;
			ptr += result;
		}
		sent_data += counter;
	}
	printf("Sent file \n");
}
*/
void send_file(char *name, int w_descriptor) {
	int file_descriptor, written, name_size, fsize;
	size_t name_lenght, *file_size;
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
	if (name_size <0) {
		perror("In sending name size");
	}
	written = send(w_descriptor, name, name_lenght*sizeof(char), 0);
	if (written <0) {
		perror("In sending file name");
	}
	file_size = (size_t*) &file_stat.st_size;
	fsize = send(w_descriptor, file_size, sizeof(file_size), 0);
	if (fsize < 0 ) {
		perror("In sending file size");
	}
	printf("sending: %s\n", name);
	read_write(file_descriptor, w_descriptor, *file_size);
	close(file_descriptor);
}

void recieve_filenames(int w_descriptor) {
	int is_file_name = 0, file_name_len = 0, f_names_len = 0;
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
	for (int i = 0; i < f_names_len; ++i) {	
		send_file(filenames[i], w_descriptor);

	}
	free(filenames);
	free(filename);
	
}


int main(int argc, char *argv[]){
	if (argc < 3) {
		fprintf(stderr, "client usage: %s hostname port", argv[0]);
		exit(0);
	}

  if ((status = getaddrinfo(argv[1], argv[2], &hints, &res)) != 0) {
     fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
     return 2;
  }
	memset(&hints, 0, sizeof (hints));
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
