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

typedef struct stPtrn patternStruct;
typedef patternStruct *ptrnPtr;

enum ptrnType {
  STAR,
  QUEST
};

struct stPtrn {
  int start;
  int end;
  enum ptrnType type;
  ptrnPtr next;
};

int main(int argc, char** argv) {

  FILE *inpt;
  ptrnPtr firstPtrn = malloc(sizeof(patternStruct));
 
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
  
  ptrns = argv[1];
  int ptrnStrLength;
  if(ptrns == NULL) {
    fprintf(stderr, "Your pattern is incorrect. Sorry.\n");
    return 1;
  } else {
    ptrnStrLength = strlen(argv[1]);
  }
  
  int isInPtrn = 0;
  ptrnPtr prevPtrn;
  ptrnPtr currPtrn = malloc(sizeof(patternStruct));
  
  firstPtrn->start =  ptrns[1];
  firstPtrn->end = ptrns[3];
  if(ptrns[5]=='*') {
    firstPtrn->type = STAR;
  } else {
    firstPtrn->type = QUEST;
  }
  firstPtrn->next = currPtrn;
  prevPtrn = firstPtrn;

  for(int i = 6; i < ptrnStrLength-5; i++) {
    char tmp = ptrns[i];
    if(tmp == '[' && !isInPtrn) {
      isInPtrn = 1;
      currPtrn->start = ptrns[i+1];
      currPtrn->end = ptrns[i+3];
      if(ptrns[i+5] == '*') {
        currPtrn->type = STAR;
      } else {
        currPtrn->type = QUEST;
      }
      prevPtrn->next = currPtrn;
      currPtrn = malloc(sizeof(patternStruct));
    }
  }

  char c = getc(inpt);
  currPtrn = firstPtrn;
  while(EOF != c) {
    while(c != '\n') {
      while(currPtrn->next) {
        int ctr = -1;
        if(currPtrn->type==QUEST) {
          ctr = 1;
        }
        while(ctr<>0) {
          if(ctr < -1) {
          
          }
          ctr--;
        }
        //c = getc(inpt);
      }
    }
    currPtrn = firstPtrn;
    c = getc(inpt);
  }
 

 


	return 0;
}


