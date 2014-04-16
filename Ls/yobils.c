#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


/*
void printFolder(char *currPath) {
	DIR *currDir;
	currDir = opendir(currPath);
	struct dirent *dp;
	dp = readdir(currDir);
	printf("\n");
	char *dot = ".";
	char *doubleDot = "..";
	while ((dp = readdir(currDir)) != NULL) {
		printf(dp->d_name);
		printf("\n");
		if((dp->d_type==DT_DIR) && (strcmp(dot,dp->d_name)) && (strcmp(doubleDot,dp->d_name))) {
			char* path[PATH_MAX];
			*path = strdup(currPath);
			strcat(*path, "/");	  	
			strcat(*path, dp->d_name);	  	
   		printFolder(*path);
  		printf("\n");
		}	
	}
}
*/

void printFolder(char *currPath) {
	DIR *currDir;
	currDir = opendir(currPath);
	struct dirent *dp;

	char *dot = ".";
	char *doubleDot = "..";
	char **pathArray = NULL;
	int numOfFolders = 0;

	while ((dp = readdir(currDir)) != NULL) {

		if((dp->d_type==DT_DIR) && (strcmp(dot,dp->d_name)) && (strcmp(doubleDot,dp->d_name))) {
			char* path[PATH_MAX];	
			*path = strdup(currPath);
			strcat(*path, "/");	  	
			strcat(*path, dp->d_name);	  	

			if (numOfFolders != 0) {
				numOfFolders = sizeof(pathArray)/sizeof(char*);
			}
			pathArray = (char**)realloc(pathArray,(numOfFolders + 1)*sizeof(char*));
			pathArray[numOfFolders] = *path;
		}	

		printf(dp->d_name);
		printf("\n");
		
	}

	int i;
	for(i = 0; i < numOfFolders; i++) {
		printFolder(pathArray[i]);
	}
}

int main(int argc, char *argv[]) {
	char *currPath = ".";
	if(argc > 1) {
	  currPath = argv[1];	
	} else {
	  char buf[PATH_MAX];
	  getcwd(buf, sizeof(buf) != 0);
	}	

  printFolder(currPath);
	return 0;
}



