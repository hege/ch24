
#include <stdio.h>
#include <string.h>

#include "dict.h"


void waitChar()
{
	printf("Press any key to exit...\n\n");
	getchar();
}

int main( int argc, char *argv[] )
{
#ifdef _DEBUG
	atexit(waitChar);
#endif

	if ( argc != 2 ) {
		printf("Need file parameter\n");
		return -1;
	}

	char buf[32];

	strcpy(buf, argv[1]);
	strcat(buf, ".dict");
	FILE *inFile = fopen(buf, "rb");
	if ( !inFile ) {
		printf("Cant open input dictionary file \'%s\'\n", buf);
		return -1;
	}

	// Fill-up dictiionary
	dictNode dict(0);
	while ( !feof(inFile) )
	{
		fscanf(inFile, "%s\n", buf);
		dict.addWord(buf);
	}
	fclose(inFile);

	// Do the text
	strcpy(buf, argv[1]);
	strcat(buf, ".in");
	inFile = fopen(buf, "rb");
	if ( !inFile ) {
		printf("Cant open input text file \'%s\'\n", buf);
		return -1;
	}
	strcpy(buf, argv[1]);
	strcat(buf, ".out");
	FILE *outFile = fopen(buf, "wb");
	if ( !outFile ) {
		printf("Cant open output text file \'%s\'\n", buf);
		return -1;
	}

	// Parse
	char c;
	int len = 0;
	*buf = 0;
	while ( (c = fgetc(inFile)) )
	{


		if ( isalpha(c) || c == '*' ) {
			buf[len++] = c;
			buf[len] = 0;
		} else {
			if ( len ) {
				dict.getWord(buf);
				fprintf(outFile, buf);
				len = 0;
			}
			if ( c == EOF )
				break;
			else
				fputc(c, outFile);
		}
	}

}

