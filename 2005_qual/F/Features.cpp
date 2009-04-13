
#include <string>

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <algorithm>
#include <vector>


#ifdef _DEBUG
void waitBeforeExit()
{
	printf("Press any key to exit...");
	getch();
}
#endif



int main( int argc, char *argv[] )
{
#ifdef _DEBUG
	atexit(waitBeforeExit);
#endif

	if ( argc != 3 )
	{
		printf("Usage: ___.exe <inpout file> <output file>\n\n");
		return 1;
	}

	FILE *fin = fopen( argv[1], "rb" );
	if ( !fin )
	{
		printf("Cant open input file\n\n");
		return -1;
	}

	FILE *fout = fopen( argv[2], "wb" );
	if ( !fout )
	{
		printf("Cant open output file\n\n");
		return -1;
	}

	int c;
	int num, sel, i, cost, j;
	size_t casenum, casepos;
	unsigned short *costs, *cases;

	while ( c = fgetc(fin) )
	{
		// Read on test case
		ungetc(c, fin);
		fscanf(fin, "%d %d", &num, &sel);

		if ( num == 0 )
			break;

		casenum = pow(2, num);

		memset(cases, 0xff, sel+sel+2);

		for ( i = 0; i < num; i++ )
		{
			fscanf(fin, " %d", &cost);
			costs[i] = cost;
		}
		fgetc(fin); // new-line character

		// Count cases
		for ( casepos = 0; casepos < casenum; casepos++ )
		{
			cost = 0;
			for ( i = 0; i < num; i++ )
			{
				if ( casepos & (1 << i) )
					cost += costs[i];
			}
			if ( cost < cases[sel-1] )
			{
				for ( j = 0; j < sel; j++ )
				{
					if ( costs[j] > sel )
						break;
				}
				memmove(costs, costs+1, sizeof(unsigned short));
				costs[j] = cost;
			}
		}

		fprintf(fout, "%d\n", cases[sel-1]);
	}
}