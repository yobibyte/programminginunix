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
      cp.left[1] = '\0';
    //Here we try to parse patterns of type: [ab]?, [ab]*, [a-b]?, [a-b]*, [ab], [a-b]
    }  else { 
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
          cp.left = (char*) realloc(cp.left, (wordLen + 2) * sizeof(char));
          cp.left[wordLen] = source[ctr];
          cp.left[wordLen + 1] = '\0';
          wordLen++;
          continue;
        } else {
          
          cp.right = (char*) realloc(cp.right, (wordLen + 2) * sizeof(char));
          cp.right[wordLen] = source[ctr];
          cp.right[wordLen + 1] = '\0';
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
      numOfPatterns--;
      break;
    }
    
  }

  return patterns;
}


int checkLine(Pattern* patterns, char* line) {

  int strCtr, ptrnCtr, matchCtr;
  strCtr = ptrnCtr = matchCtr = 0;
  
  while(1) {
    
    if(ptrnCtr == numOfPatterns) {
      return 1;
    }
    
    Pattern p = patterns[ptrnCtr];
    
    if(p.type == '\0') {
      if(p.right == NULL) {
        
        char substr[strlen(p.left) + 1];
        memcpy(&substr, line + sizeof(char) * strCtr, strlen(p.left));
        substr[strlen(p.left)] = '\0';
        
        if(strcmp(substr, p.left) == 0) { 
          strCtr += strlen(p.left);
          ptrnCtr++;
        } else {
          strCtr += strlen(p.left);
          break;  
        }  
      } else {
        char c = line[strCtr];
        if(c >= p.left[0] && c <= p.right[0]) {
          strCtr++;
          ptrnCtr++;
        } else {
          break;
        }
      }
    //needs debugging
    } else if(p.type == '*') {
      if(p.right == NULL) {
        char substr[strlen(p.left) + 1];
        memcpy(&substr, line + sizeof(char) * strCtr, strlen(p.left));
        substr[strlen(p.left)] = '\0';
          
        if(strcmp(substr, p.left) == 0) { 
          strCtr += strlen(p.left);
        } else {
          ptrnCtr++;
        }
      } else {
        //two part pattern here 
      }
    } else if(p.type == '?') {
      if(p.right == NULL) { 
        char substr[strlen(p.left) + 1];
        memcpy(&substr, line + sizeof(char) * strCtr, strlen(p.left));
        substr[strlen(p.left)] = '\0';
          
        if(strcmp(substr, p.left) == 0) { 
          if(matchCtr < 2) {
            matchCtr++;
            strCtr += strlen(p.left);
          } else {
            matchCtr = 0;
            ptrnCtr++;
          }
        } else {
          matchCtr = 0;
          ptrnCtr++;
        }
      } else {
        //two part patterns here 
      }
    }//finished needs debugging
  }

  if(strlen(line) > 0) {
    char substr[strlen(line)];
    memcpy(&substr, line + sizeof(char)*strCtr, strlen(line));
    substr[strlen(line)] = '\0';
    if(checkLine(patterns, substr)) {
      return 1;
    }
  }

  return 0;
}

void testParcePatterns(char* input) {

  Pattern* patterns = parsePatterns(input);

  printf("Your pattern: %s\n", input);
  printf("You have %d patterns\n", numOfPatterns);

  for(int i = 0; i < numOfPatterns; i++) {
    printf("Pattern %d\t Left: %s\t Right: %s\t Type: %c\n", i+1, patterns[i].left, patterns[i].right, patterns[i].type);
  }
}

int main(int argc, char** argv) {

  if(argc == 1) {
    fprintf(stderr, "You didn't provide any pattern.\n");
    return 1;
  }

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

  Pattern* patterns = parsePatterns(argv[PATTERN]);  
  
  //testParcePatterns(argv[PATTERN]);
  //testParcePatterns("a*[bc]c?[b-c][bc-]");
  
  char c = getc(inputStream);
  while(c != EOF) {
    char* currLine = NULL;
    int wordLength = 0;
    while(1) {
      currLine = (char*) realloc(currLine, (wordLength + 2) * sizeof(char));
      currLine[wordLength] = c;
      currLine[wordLength+1] = '\0';
      c = getc(inputStream);
      if(c == '\n' || c == EOF)  {
        if(checkLine(patterns, currLine)) {
          printf("%s\n",currLine);
        }
        break;
      }
      wordLength++;
    }
    c = getc(inputStream); 
  }
  
  return 0;
}
//TODO add several files matching
