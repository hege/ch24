
#include <string>

#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string>
#include <map>


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

	int inFileNum = _fileno( fin );
	size_t bufSize = _filelength( inFileNum ), actBufSize = 0;
	char c;

	std::map<std::string, size_t> wordCnt;
	std::map<std::string, size_t>::iterator wIt;
	std::string word;

	// Index words
	while ( !feof(fin) )
	{
		c = fgetc(fin);

		if ( c != ' ' && c != '\n' && c != '\r' )
			word.push_back(c);
		else
		{
			if ( !word.empty() )
			{
				if ( (wIt = wordCnt.find( word )) == wordCnt.end() )
					wordCnt.insert( std::make_pair(word, 1) );
				else
					wIt->second++;
				word.clear();
			}
		}
	}

	// Get max number word (errrgh, whatever...)
	size_t maxCnt = 0;
	std::string maxWord;
	for ( wIt = wordCnt.begin(); wIt != wordCnt.end(); wIt++ )
	{
		if ( wIt->second > maxCnt )
		{
			maxWord = wIt->first;
			maxCnt = wIt->second;
		}
	}

	// Reopen
	fclose(fin);
	fin = fopen( argv[1], "rb" );

	// Replace stuffs
	while ( !feof(fin) )
	{
		c = fgetc(fin);

		if ( c != ' ' && c != '\n' && c != '\r' )
		{
			word.push_back(c);
		}
		else
		{
			if ( !word.empty() )
			{
				if ( word == maxWord )
					fputc('*', fout);
				else
					fprintf(fout, "%s", word.c_str());
				word.clear();
			}
			fputc(c, fout);
		}
	}
}
