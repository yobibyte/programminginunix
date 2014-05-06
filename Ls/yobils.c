#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<time.h>
#include<pwd.h>
#include <grp.h>
#include <errno.h>


void printFolder(char *currPath) {
	DIR *currDir;
  currDir = opendir(currPath);
	struct dirent *dp;
	char *dot = ".";
	char *doubleDot = "..";
	char **pathArray = NULL;
	int numOfFolders = 0;

	struct stat st;

  if(currDir == NULL) {
    fprintf(stderr,"Input file doesn't exist or you have no access to it\n");
    return;
  }

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

		if(strcmp(dot,dp->d_name)&&strcmp(doubleDot,dp->d_name)) {
               
		  char* currFilePath [PATH_MAX];
		  *currFilePath = strdup(currPath);
		  strcat(*currFilePath, "/");
		  strcat(*currFilePath, dp->d_name);

	  	stat(dp->d_name, &st);

 
		  printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
		  printf( (st.st_mode & S_IRUSR) ? "r" : "-");
		  printf( (st.st_mode & S_IWUSR) ? "w" : "-");
		  printf( (st.st_mode & S_IXUSR) ? "x" : "-");
		  printf( (st.st_mode & S_IRGRP) ? "r" : "-");
		  printf( (st.st_mode & S_IWGRP) ? "w" : "-");
		  printf( (st.st_mode & S_IXGRP) ? "x" : "-");
		  printf( (st.st_mode & S_IROTH) ? "r" : "-");
		  printf( (st.st_mode & S_IWOTH) ? "w" : "-");
		  printf( (st.st_mode & S_IXOTH) ? "x" : "-");

		  printf("\t");
		
		
		  printf("%ld ", st.st_nlink);

		  int size = st.st_size;
		  printf("%d", size);	
		  printf("\t");

		  struct passwd *pwd;
		  pwd = getpwuid(st.st_uid);
		  printf("%s\t",pwd->pw_name);
		  
					  
     	          struct group *g;
		  g = getgrgid(st.st_gid);
		  printf("%s\t",g->gr_name);
		  
	 	  struct tm ts;
		  char buf[80];
		  // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
		  ts = *localtime(&st.st_atime);
		  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &ts);
		  printf("%s\t", buf);

		  printf(dp->d_name);
	    printf("\n");

    }  
	
	}

	for(int i = 0; i < numOfFolders; i++) {
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

