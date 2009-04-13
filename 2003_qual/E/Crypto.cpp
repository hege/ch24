

#include <io.h>
#include <set>
#include <math.h>


void waitChar()
{
	printf("Press any key to exit...\n\n");
	getchar();
}

void makePrimes(size_t upBound, std::set<size_t> &primes)
{
	std::set<size_t>::iterator it;
	
	for ( size_t i = 3; i <= upBound; i += 2 )
	{
		for ( it = primes.begin(); it != primes.end(); it++ )
		{
			if ( !(i % *it) )
				break;
		}
		if ( it == primes.end() )
			primes.insert(i);
	}
	primes.insert(2);
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

	// Step 5
	size_t csize = _filelength(_fileno(inFile))/2;
	unsigned char *text = (unsigned char*)malloc(csize);
	unsigned char lsb, msb;
	
	for ( size_t pos = 0; !feof(inFile); pos++ )
	{
		msb = fgetc(inFile) - 'A';
		lsb = fgetc(inFile) - 'A';
		text[pos] = ( msb << 4 ) + lsb;
	}

	// Step 4
	std::set<size_t> primes;
	makePrimes(csize, primes);
	for ( pos = 0; pos < csize; pos++ )
	{
		if ( primes.find(pos+1) != primes.end() )
			text[pos] -= (unsigned char)pos + 1;
	}

	// Step 3
	for ( pos = csize - 1; pos > 0; pos-- )
	{
		text[pos] -= text[pos-1];
	}

	// Step 2
	unsigned char *text2 = (unsigned char*)malloc(csize), buf[12];
	size_t lpos = csize-1;

	text2[lpos] = text[lpos];
	text2[lpos-1] = text[lpos-1];

	for ( pos = 2; pos < 13; pos++ )
	{
		lsb = text2[lpos-pos+2];
		text2[lpos-pos+2] = text2[lpos-pos+1];
		text2[lpos-pos+1] = lsb;

		text2[lpos-pos] = text[lpos-pos];
	}
	for ( pos = 13; pos < csize; pos++ )
	{
		lsb = text2[lpos-pos+2];
		text2[lpos-pos+2] = text2[lpos-pos+1];
		text2[lpos-pos+1] = lsb;

		memcpy(buf, text2+lpos-11, 12);
		memmove(text2+lpos-pos+13, text2+lpos-pos+1, pos-12);
		memcpy(text2+lpos-pos+1, buf, 12);

		text2[lpos-pos] = text[lpos-pos];
	}

	// Step 1
	size_t side = (size_t)sqrt((double)csize), x, y, i, cnt = 0;
	for ( y = 0; y < side; y++ )
	{
		for ( i = 0; i <= y; i++ )
		{
			text[(y - i) * side + i] = text2[cnt++];
		}
	}
	for ( x = 1; x < side; x++ )
	{
		for ( i = 0; i < side - x; i++ )
		{
			text[(side - i - 1) * side + x + i] = text2[cnt++];
		}
	}


	// Spit it out
	char buf2[80];
	strcpy(buf2, argv[1]);
	strcpy(buf2 + ( strlen(buf2) - 2 ), "out");
	FILE *outFile = fopen(buf2, "wb");
	if ( !outFile ) {
		printf("Cant open output file \'%s\'\n", text2);
		return -1;
	}
	fputs((char*)text, outFile);
	fclose(outFile);

	return 0;
}