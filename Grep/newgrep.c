#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define PATTERN 1
#define FILEPATH 2

int numOfPatterns = 0;
int patternsArraySize = 0;

typedef struct Pattern {
  char* left;
  char* right;
  char type;
} Pattern;

FILE* openFile(char* path) {
  return fopen(path, "r") ;
}  

Pattern* parsePatterns(char* source) {
  int sourceLen = strlen(source);
  Pattern* patterns = NULL;
  int ctr = 0;

  while(1) {


    Pattern cp;
    cp.left = NULL;
    cp.right = NULL;    
    cp.type = '\0';

    // Here we can parce patterns of type: 'a' or 'a*' or 'a?', where a is a single char
    if(source[ctr] != '[') { 
      cp.left = malloc(sizeof(char) * 2);
      cp.left[0] = source[ctr];
    //Here we try to parse patterns of type: [ab]?, [ab]*, [a-b]?, [a-b]*, [ab], [a-b]
    } 
    
    else { 
      int isLeft = 1;
      int wordLen = 0; //to store length of right of left part o a pattern, i.e [a-b] left part is 1 and right part is 1;
      while(source[++ctr] != ']') {
        if(ctr > sourceLen) {
          fprintf(stderr, "Check you pattern! Something is wrong with it.");//TODO add throwing exception
        }
        if(source[ctr] == '-' && isLeft == 1 && source[ctr+2] == ']') {
          isLeft = 0;
          wordLen = 0;
          continue;
        }
        if(isLeft == 1) {
          cp.left = (char*) realloc(cp.left, (wordLen + 1) * sizeof(char));
          cp.left[wordLen] = source[ctr];
          wordLen++;
          continue;
        } else {
          cp.right = (char*) realloc(cp.right, (wordLen + 1) * sizeof(char));
          cp.right[wordLen] = source[ctr];
          wordLen++;
          continue;
        }
      }
    }

    ctr++;

    //set pattern type
    if(source[ctr] == '?' || source[ctr] == '*') {
      cp.type = source[ctr];
      ctr++;
    } 
      
    patternsArraySize += sizeof(cp);
    patterns = realloc(patterns, patternsArraySize);
    patterns[numOfPatterns] = cp;
    numOfPatterns++;
    
    if(ctr > sourceLen) {
      break;
    }
    
  }

  return patterns;
}


int checkLine(Pattern* patterns, char* line) {
  int sourceLen = strlen(line);
  int ctr = 0;
 

  for(int i = 0; i < numOfPatterns; i++) {

    if(ctr > sourceLen) {
      fprintf(stderr, "Check you pattern! Something is wrong with it.");//TODO add throwing exception
    }

    
    Pattern cp = patterns[i];
  
    int matchCtr = 0;
    while(1) {
      if(cp.right == NULL) {
        if(cp.type == NULL) {
          if(cp.left[0] != line[ctr]) {
            return 0;
          } else {
            ctr++;
          }
        } else if(cp.right == '*') {
          if(cp.right[0] == line[ctr]) {
            matchCtr++;
            ctr++;
            continue;
          } else {
            break;
          }
        } else if(cp.right == '?') {
          if(cp.right[0] == line[ctr] && matchCtr == 0) {    
            if(cp.left[0] != line[ctr]) {
              return 0;
            } else {
              ctr++;
            }
          } else {
            break;
          }
        }
      }
    }  
  }

  return 1;
}

void testParcePatterns(char* input) {

  Pattern* patterns = parsePatterns(input);

  printf("Your pattern: %s\n", input);
  printf("You have %d patterns\n", numOfPatterns - 1);

  for(int i = 0; i < numOfPatterns - 1; i++) {
    printf("Pattern %d\t Left: %s\t Right: %s\t Type: %c\n", i+1, patterns[i].left, patterns[i].right, patterns[i].type);
  }
}

int main(int argc, char** argv) {

  FILE* inputStream;
  if(argc > 2) {
    inputStream = openFile(argv[FILEPATH]);
    if(NULL == inputStream) {
      fprintf(stderr, "Input file doesn't exist or you have no access to it.\n");
      return 1;
    }
  } else {
    inputStream = stdin;
  }

  //Pattern* patterns = parsePatterns(argv[PATTERN]);  

  testParcePatterns("a*[bc]c?[b-c][bc-]");

  /*
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
    if(checkLine(patterns, currLine)) {
      printf(currLine);
    }
  }
  */
  return 0;
}
//TODO add search of substirng in string, not exact search as is
//TODO add processing '-' as symbol if there is no symbols after it inside brackets
//TODO add several files matching
