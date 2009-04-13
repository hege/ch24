

#include <stdio.h>
#include <conio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <fstream>



#define INPUT_FILE		"../ProblemSet/6ch24_EC/samples/A-sample-0.in"
#define OUTPUT_FILE		"A-sample-0.ans"


#ifndef NDEBUG
void waitBeforeExit()
{
	printf( "Press any key to exit...\n\n" );
	getchar();
}
#endif

void main()
{
	#ifndef NDEBUG
		atexit( waitBeforeExit );
	#endif

	std::ifstream inFile;
}