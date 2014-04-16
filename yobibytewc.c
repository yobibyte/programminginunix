#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_OF_STRINGS ((const unsigned char) 'l')
#define NUM_OF_BYTES ((const unsigned char) 'c')
#define NUM_OF_CHARS ((const unsigned char) 'm')
#define NUM_OF_WORDS ((const unsigned char) 'w')
#define MAX_STRING_LENGTH ((const unsigned char) 'L')

#define READ_MODE ((const unsigned char*) "r")

int totalStrings = 0;
int totalBytes = 0;
int totalChars = 0;
int totalWords = 0;
int totalMaxString = 0;

void getAllKeys(char *fileName) {
	FILE *currFile = fopen(fileName, READ_MODE);
	printf("% 5d ",numOfStrings(currFile));
	currFile = fopen(fileName, READ_MODE);
	printf("% 5d ",numOfWords(currFile));
	currFile = fopen(fileName, READ_MODE);
	printf("% 5d ",numOfBytes(currFile));
	printf(fileName);
	printf("\n");
}

int numOfStrings(FILE *file) {
	int numOfStrings = 0;
	int c = getc(file);
	while(c != EOF) {
		if (c == '\n') {		
			numOfStrings++;
		}	
		c = getc(file);
	}
	return numOfStrings;	
}

int numOfBytes(FILE *file) {
	int numOfBytes = 0;
	int c = getc(file);
	while(c != EOF) {
		numOfBytes++;
		c = getc(file);
	}
	return numOfBytes*sizeof(char);	
}

int numOfChars(FILE *file) {
	int numOfChars = 0;
	int c = getc(file);
	while(c != EOF) {
		numOfChars++;
		c = getc(file);
	}
	return numOfChars;
}

int maxStringLength(FILE *file) {
	int maxStringLength = 0;
	int currStringLength = 0;
	int c = getc(file);
	while(c != EOF) {
		while (c != '\n') {
			currStringLength++;
			c = getc(file);
		}
		if(currStringLength > maxStringLength) {
			maxStringLength = currStringLength;
		}
		currStringLength = 0;
		c = getc(file);
	}
	return maxStringLength;
}

int numOfWords(FILE *file) {
	int numOfWords = 0;
	int isWord = 0;
	int c = getc(file);
	while(c != EOF) {
		if (!isWord) {
			if(!isspace(c)) {
				isWord = 1;
			}
		} else {
			if(isspace(c)) {
				numOfWords++;
				isWord = 0;
			}
		}
		c = getc(file);
	}
	return numOfWords;
}

void useKey(char currKey, FILE *currFile) {
			switch(currKey) {
				case NUM_OF_STRINGS:
					printf("% 5d",numOfStrings(currFile));
					break;
				case NUM_OF_BYTES:
					printf("% 5d",numOfBytes(currFile));
					break;
				case NUM_OF_CHARS:
					printf("% 5d", numOfChars(currFile));
					break;
				case NUM_OF_WORDS:
					printf("% 5d",numOfWords(currFile));
					break;
				case MAX_STRING_LENGTH:
					printf("% 5d",maxStringLength(currFile));
					break;
				default:
					printf("Key %c is not recognized\n", currKey);
					break;
			}
}	

int main(int argc, char *argv[]) {
	int i,j;		
	char **fileList = malloc(argc*sizeof(char*));
	char *currFileName;
	char *keyList = (char*) malloc(sizeof(char) * argc);
	int numOfFiles = 0;
	int numOfKeys = 0;

	for (i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			if (strlen(argv[i]) > 2) {
				printf("Check your input. One or several keys are not correct!\n");
				return 1;
			}
			keyList[numOfKeys++] = argv[i][1];
		} else {
			fileList[numOfFiles++] = argv[i];
		}
	}

	if((numOfFiles == 0)) {
		printf("There is no files specified\n");
		return 1;
	}	

	if((numOfKeys == 0)) {
		for(i=0; i < numOfFiles; i++) {
			currFileName = fileList[j];	
			getAllKeys(fileList[i]);
		}

		if(numOfFiles > 1) {
			printf("% 5d", totalStrings);
			printf("% 5d", totalWords);
			printf("% 5d ", totalBytes);
			printf("total\n");
		}
		return 0;
	}	
	
	for (j = 0; j < numOfFiles; j++) {
		currFileName = fileList[j];	
		for (i = 0; i < numOfKeys; i++) {
		FILE *currFile = fopen(currFileName, READ_MODE);
			char currKey = keyList[i];
			useKey(currKey, currFile);
		}
		printf(" ");
		printf(currFileName);
		printf("\n");
	}

	if(numOfFiles > 1) {
		for (i = 0; i < numOfKeys; i++) {
			char currKey = keyList[i];
			switch(currKey) {
				case NUM_OF_STRINGS:
					printf("% 5d", totalStrings);
					break;
				case NUM_OF_BYTES:
					printf("% 5d",totalBytes);
					break;
				case NUM_OF_CHARS:
					printf("% 5d", totalChars);
					break;
				case NUM_OF_WORDS:
					printf("% 5d", totalWords);
					break;
				case MAX_STRING_LENGTH:
					printf("% 5d", totalMaxString);
				break;
			}
		}
		printf("total\n");
	}	

	return 0;
}
//TODO make same flags parsing
//stdin
//make in one iteration
//file reset
//stderr for errors
//null files -> crash
//makefile
//getopt
//long keys --ws
//TODO recursive ls -lR
//grep
