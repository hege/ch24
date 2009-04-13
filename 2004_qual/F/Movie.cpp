

#include <stdio.h>
#include <string.h>
#include <vector>


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

	char buf[128];

	strcpy(buf, argv[1]);
	strcat(buf, ".in");
	FILE *inFile = fopen(buf, "rb");
	if ( !inFile ) {
		printf("Cant open input dictionary file \'%s\'\n", buf);
		return -1;
	}

	int actNo, filmNo, i, j, k, num;
	char *cat;

	size_t costs[100], bestcost = -1;
	char was[100][100];
	int acts[100];

	memset(was, 0, 10000);

	fscanf(inFile, "%d %d\n", &actNo, &filmNo);

	for ( i = 0; i < actNo; fscanf(inFile, "%d\n", &costs[i++]) );

	for ( k = 0; k < filmNo; k++ )
	{
		fgets(buf, 128, inFile);
		
		num = atoi(strtok(buf, " \n"));
		

		for ( j = 0; j < num; j++ ) acts[j] = atoi(strtok(NULL, " \n")) - 1;
		
		for ( i = 0; i < num; i++ )
		{
			for ( j = i + 1; j < num; j++ )
			{
				was[acts[i]][acts[j]] = 1;
				was[acts[j]][acts[i]] = 1;
			}
		}
	}

	for ( i = 0; i < actNo; i++ )
	{
		for ( j = i + 1; j < actNo; j++ )
		{
			if ( !was[i][j] && costs[i] + costs[j] < bestcost )
				bestcost = costs[i] + costs[j];
		}
	}

	strcpy(buf, argv[1]);
	strcat(buf, ".out");
	FILE *outFile = fopen(buf, "wb");
	if ( !outFile ) {
		printf("Cant open output dictionary file \'%s\'\n", buf);
		return -1;
	}

	fprintf(outFile, "%d\n", bestcost);
}


