
#include <stdio.h>
#include <string.h>

void waitChar()
{
	printf("Press any key to exit...\n\n");
	getchar();
}

bool testxc( int x[8], int c[8] )
{
	for ( int i = 0; i < 8; i++ ) {
		if ( x[i] != c[i] )
			return false;
	}
	return true;
}

int main( int argc, char *argv[] )
{
	atexit(waitChar);

	if ( argc != 2 ) {
		printf("Need file parameter\n");
		return -1;
	}

	FILE *inFile = fopen(argv[1], "rb");
	if ( !inFile ) {
		printf("Cant open input file \'%s\'\n", argv[1]);
		return -1;
	}

	int x[8], c[8], A[8], B[8];
	fscanf(inFile, "%d %d %d %d %d %d %d %d\n", &x[0], &x[1], &x[2], &x[3], &x[4], &x[5], &x[6], &x[7]);
	fscanf(inFile, "%d %d %d %d %d %d %d %d\n", &c[0], &c[1], &c[2], &c[3], &c[4], &c[5], &c[6], &c[7]);
	fscanf(inFile, "%d %d %d %d %d %d %d %d\n", &A[0], &A[1], &A[2], &A[3], &A[4], &A[5], &A[6], &A[7]);
	fscanf(inFile, "%d %d %d %d %d %d %d %d\n", &B[0], &B[1], &B[2], &B[3], &B[4], &B[5], &B[6], &B[7]);


	fclose(inFile);

	char buf2[80];
	strcpy(buf2, argv[1]);
	strcpy(buf2 + ( strlen(buf2) - 2 ), "out");
	FILE *outFile = fopen(buf2, "wb");
	if ( !outFile ) {
		printf("Cant open output file \'%s\'\n", buf2);
		return -1;
	}

	int newx;
	for ( size_t cnt = 7; cnt < 10000000; cnt++ )
	{
		if ( testxc(x, A) ) {
			fprintf(outFile, "A wins at %d.\n", cnt);
			fclose(outFile);
			return 0;
		}
		if ( testxc(x, B) ) {
			fprintf(outFile, "B wins at %d.\n", cnt);
			fclose(outFile);
			return 0;
		}

		newx = ( x[7]*c[0] + x[6]*c[1] + x[5]*c[2] + x[4]*c[3] + x[3]*c[4] + x[2]*c[5] + x[1]*c[6] + x[0]*c[7] ) % 1000;
		memmove(x, x + 1, 7 * sizeof(int));
		x[7] = newx;
	}

	printf("Didn't found anything!\n");
}
