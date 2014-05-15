#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct Pattern;

struct Pattern {
  char* left;
  char* right;
  char type;
};

FILE* openFile(char* path) {
  return fopen(path, "r") ;
}  

char** parsePatterns(char* source) {
  return;
}

int checkLine(char* line) {
  return 1;
}

int main(int argc, char** argv) {

  FILE* inputStream;
  if(argc > 2) {
    inputStream = openFile(argv[2]);
    if(NULL == inputStream) {
      fprintf(stderr, "Input file doesn't exist or you have no access to it.\n");
      return 1;
    }
  } else {
    inputStream = stdin;
  }

  char c = getc(inputStream);
  while(c != EOF) {
    char* currLine = NULL;
    int wordLength = 0;
    while(1) {
      currLine = (char*) realloc(currLine, (wordLength + 1) * sizeof(char));
      currLine[wordLength] = c;
      c = getc(inputStream);

      if(currLine[wordLength] == '\n') {
        break;
      }

      wordLength++;
    }
    if(checkLine(currLine)) {
      printf(currLine);
    }
  }

  return 0;
}
