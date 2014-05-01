/*
 *This program can print lines containing combinations of the next pattern
 * [0-9]* or [0-9]? Where * is from zero to infinity and ? is from zero to one
 * input can be either from file or from standard input
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int isFileInput = 0;
char* ptrns;

int main(int argc, char** argv) {

  FILE *inpt;

  if(argc > 2) {
	  isFileInput = 1;
    inpt = fopen(argv[2], "r");
    if(NULL == inpt) {
      fprintf(stderr,"Input file doesn't exist or you have no access to it\n");
      return 1;
    }
	} else {
    inpt = stdin;
  }

  
  char c = getc(inpt);
  while(EOF != c) {
    printf("%c", c);
    c = getc(inpt);
  }

	return 0;
}


