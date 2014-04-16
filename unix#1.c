#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct elem elem_type;
typedef elem_type *elem_ptr;

struct elem {
  elem_ptr next;
  char value[];
};	

elem_ptr firstElem;
elem_ptr currElem;
elem_ptr prevElem;
int isQuoteOpened = 0;
int wordLength = 0;

void printElements() {
  currElem = firstElem;
  do {
    printf("\"%s\"\n",currElem->value);
    currElem = currElem->next;
  }  while (currElem->next);
  printf("\"%s\"\n",currElem->value);
}	

elem_ptr createElem(char *word, int length) {
  elem_ptr createdElem = malloc(sizeof(char) * length + sizeof(elem_ptr) + 1); 
  createdElem->value[length+1] = '\0';
  strcpy((createdElem->value), word);
  return createdElem;
}	

void insertElem(elem_ptr newElem) {
  if (!firstElem) {
    firstElem = newElem;
  } else {
    if (strcmp(currElem->value, newElem->value) < 0) {
      if (!currElem->next) {
        currElem->next = newElem;
      } else {
        prevElem = currElem;
	currElem = currElem->next;
	insertElem(newElem);
      }
    } else {
      if(!prevElem) {
        firstElem = newElem;
	newElem->next = currElem;
      } else {
        firstElem = prevElem;
	prevElem->next = newElem;
	newElem->next = currElem;
      }
    }
  }
  prevElem = NULL;
  currElem = firstElem;
}	

int isWord(int c) {
  if(c == 9 || c == 13 || c == 32 || c == 10) {
    return 0;
  }
  return 1;
}	

void mainLoop () {
  char *tmp = NULL;
  int c = fgetc(stdin);
  while(c != EOF) {	  
   if (!isWord(c) || c == 34) {
     if (c == 34) {
       isQuoteOpened = !isQuoteOpened;
     }			
     c = (char) fgetc(stdin);
     continue;
   }
   wordLength = 0;
   while((isWord(c) && c != EOF) || isQuoteOpened) {	
     if (c == 34) {
        isQuoteOpened =!isQuoteOpened;
       break;
     }	
     tmp = (char*) realloc(tmp, (wordLength+1)*sizeof(char));     
     tmp[wordLength] = c; 
     c = (char) fgetc(stdin);
     wordLength++;
   }
    elem_ptr tmpElem = createElem(tmp,wordLength);
    insertElem(tmpElem); 	
    c = fgetc(stdin);
  }		  
}

int main() {
  mainLoop();  
  fclose(stdin);
  if(isQuoteOpened) {
    printf("Hey! Close all the quotes\n");
  } else {
    printElements();
  }
  return 0; 
}

