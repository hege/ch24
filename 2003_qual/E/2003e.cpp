#include <stdio.h>
#include <io.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <set>

using namespace std;

#define INPUT_FILE		"in\\E-7.in"
#define OUTPUT_FILE		"out\\E-7.out"


int main( int argc, char* argv[] )
{
	FILE *inFile = fopen( INPUT_FILE, "rb" );
	if ( !inFile )
	{
		cerr << "Error opening input file" << endl << endl;
		return -1;
	}
	FILE *outFile = fopen( OUTPUT_FILE, "wb" );
	if ( !outFile )
	{
		cerr << "Error opening output file" << endl << endl;
		return -1;
	}

	int i;
	string cypherText;
	int cypherTextLen = _filelength(_fileno(inFile));
	cypherText.resize( cypherTextLen );
	fgets( (char*)cypherText.c_str(), cypherTextLen+1, inFile );

	//Step 5
	int l = cypherTextLen / 2;
	vector<unsigned char> c( l );
	for ( i = 0; i < l; ++i )
		c[i] = ( ((unsigned char)( cypherText[2*i] - 'A' )) << 4 ) | ((unsigned char)( cypherText[2*i+1] - 'A' ));

	// Step 4
	set<int> primes;
	for ( i = 2; i <= l; ++i )
	{
		for ( set<int>::iterator it = primes.begin(); it != primes.end(); ++it )
			if ( i % *it == 0 )
				break;
		if ( it == primes.end() )
			primes.insert( i );
	}
	vector<unsigned char> b( l );
	for ( i = 0; i < l; ++i )
	{
		if ( primes.find(i+1) == primes.end() )
			b[i] = c[i];
		else
			b[i] = (unsigned char)( c[i] - i ) - 1;

	}

	// Step 3
	vector<unsigned char> a( l );
	a[0] = b[0];
	for ( i = 1; i < l; ++i )
	{
		a[i] = (unsigned char)( b[i] - b[i-1] );
	}

	// Step 2
	vector<unsigned char> s;
	for ( i = 0; i < l; ++i )
	{
		if ( s.size() > 1 )
		{
			s[0] += s[1];
			s[1] =  s[0] - s[1];
			s[0] -= s[1];
		}
		if ( s.size() > 12 )
		{
			s.insert( s.begin(), 12, 0 );
			for ( size_t i = 0; i < 12; ++i ) s[i] = *(s.end()-12+i);
			s.erase( s.end()-12, s.end() );
		}
		s.insert( s.begin(), a[l-i-1] );
	}

	// Step 1
	int n = (int)sqrt((double)l);
	string plainText( l, 0 );
	int row = 0, col = 0, tmp;
	for ( i = 0; i < l; ++i )
	{
		plainText[col+row*n] = s[i];
		if ( i < n*n/2 )
		{
			tmp = col;
			col = row ? col+1 : 0;
			row = row ? row-1 : tmp+1;
		}
		else
		{
			tmp = row;
			row = col==n-1 ? n-1   : row-1;
			col = col==n-1 ? tmp+1 : col+1;
		}
	}

	fputs( plainText.c_str(), outFile );

	return 0;
}