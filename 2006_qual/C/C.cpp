

#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <vector>


#define INPUT_FILE		"C-1.in"
#define OUTPUT_FILE		"C-1.ans"


#ifndef NDEBUG
void waitBeforeExit()
{
	printf( "Press any key to exit...\n\n" );
	getchar();
}
#endif


char field[50][50];

struct sFieldStep
{
	char player;
	char opponent;
	int Awins;
	int Zwins;
	double p;

	std::vector<sFieldStep> nextSteps;

	sFieldStep( char player, char opponent )
		: player ( player )
		, opponent ( opponent )
	{
	}
};



int height, width;

void calculate( sFieldStep & curField )
{
	int i,j;

	for ( i = 0; i < height; i++ )
	{
		for ( j = 0; j < width; j++ )
		{
			if ( field[i][j] == curField.player )
			{
				// N
				if ( i > 0 && field[i-1][j] == curField.opponent )
				{
					curField.nextSteps.push_back( sFieldStep(curField.opponent, curField.player) );
					field[i-1][j] = curField.player;
					field[i][j] = '.';
					calculate( curField.nextSteps.back() );
					field[i-1][j] = curField.opponent;
					field[i][j] = curField.player;
				}
				// S
				if ( i < height - 1 && field[i+1][j] == curField.opponent )
				{
					curField.nextSteps.push_back( sFieldStep(curField.opponent, curField.player) );
					field[i+1][j] = curField.player;
					field[i][j] = '.';
					calculate( curField.nextSteps.back() );
					field[i+1][j] = curField.opponent;
					field[i][j] = curField.player;
				}
				// W
				if ( j > 0 && field[i][j-1] == curField.opponent )
				{
					curField.nextSteps.push_back( sFieldStep(curField.opponent, curField.player) );
					field[i][j-1] = curField.player;
					field[i][j] = '.';
					calculate( curField.nextSteps.back() );
					field[i][j-1] = curField.opponent;
					field[i][j] = curField.player;
				}
				// E
				if ( j < width - 1 && field[i][j+1] == curField.opponent )
				{
					curField.nextSteps.push_back( sFieldStep(curField.opponent, curField.player) );
					field[i][j+1] = curField.player;
					field[i][j] = '.';
					calculate( curField.nextSteps.back() );
					field[i][j+1] = curField.opponent;
					field[i][j] = curField.player;
				}
			}
		}
	}
	
	if ( curField.nextSteps.empty() )
	{
		if ( curField.player == 'A' )
		{
			curField.Awins = 0;
			curField.Zwins = 1;
			curField.p = 0.0;
		}
		else
		{
			curField.Awins = 1;
			curField.Zwins = 0;
			curField.p = 1.0;
		}
	}
	else
	{
		curField.p = 0.0;
		curField.Awins = curField.Zwins = 0;
		for ( std::vector<sFieldStep>::iterator it = curField.nextSteps.begin(); it != curField.nextSteps.end(); it++ )
		{
			curField.Awins += it->Awins;
			curField.Zwins += it->Zwins;
			curField.p += it->p;
		}
		curField.p /= curField.nextSteps.size();
		assert( curField.p <= 1.0 );
	}
}

char process( FILE * inFile )
{

	fscanf( inFile, "%d%d\n", &width, &height );
	if ( !( width > 0 && width <= 50 && height > 0 && height <= 50 ) )
	{
		printf( "kicsi a memoria!!!\n\n" );
		return 'A';
	}

	sFieldStep rootField( 'A', 'Z' );
	for ( int i = 0; i < height; fgets( field[i++], 10, inFile ) );
	calculate( rootField );
    
	sFieldStep *pNextField, *pActField = &rootField;
	for(;;)
	{
		if ( !pActField->Awins )
			return 'Z';
		else if ( !pActField->Zwins )
			return 'A';
		else
		{
			assert( !pActField->nextSteps.empty() );
			if ( pActField->player == 'A' )
			{
				std::vector<sFieldStep>::iterator it = pActField->nextSteps.begin();
				pNextField = &(*it);
				for ( it++ ; it != pActField->nextSteps.end(); it++ )
				{
					if ( it->p > pNextField->p )
						pNextField = &(*it);
				}
			}
			else
			{
				std::vector<sFieldStep>::iterator it = pActField->nextSteps.begin();
				pNextField = &(*it);
				for ( it++ ; it != pActField->nextSteps.end(); it++ )
				{
					if ( it->p < pNextField->p )
						pNextField = &(*it);
				}
			}
			pActField = pNextField;
		}
	}
}


void main()
{
	#ifndef NDEBUG
		atexit( waitBeforeExit );
	#endif

	FILE *inFile = fopen( INPUT_FILE, "rb" );
	if ( !inFile )
	{
		printf( "Cannot open input file '%s'\n\n", INPUT_FILE );
		return;
	}

	FILE *outFile = fopen( OUTPUT_FILE, "wb" );
	if ( !outFile )
	{
		printf( "Cannot open output file '%s'\n\n", OUTPUT_FILE );
		return;
	}

	int i, num;

    fscanf( inFile, "%d\n", &num );
	for( i = 0; i < num; i++ )
	{
		fprintf( outFile, "%c\n", process( inFile ) );
	}
}
