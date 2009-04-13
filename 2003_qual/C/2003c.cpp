#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

#define INPUT_FILE		"in\\C-7.in"
#define OUTPUT_FILE		"out\\C-7.out"



struct Fragment
{
	string			fragment;
	unsigned short	startId;
	unsigned short	endId;
	bool			used;
};

struct Node
{
	vector<int> edgesIn;	// Fragment
	vector<int> edgesOut;
};



unsigned char conv[128];

inline unsigned short getId( const char * str )
{
	unsigned short ret = 0;
	for ( int i = 0; i < 5; ++i )
	{
		ret <<= 2;
		ret += conv[str[i]];
	}
	assert( ret < 1024 );
	return ret;
}

int main( int argc, char* argv[] )
{
	conv['A'] = 0;
	conv['C'] = 1;
	conv['T'] = 2;
	conv['G'] = 3;

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

	// Get parameters
	int fragNum, fragLen;
	fscanf( inFile, "%d %d\n", &fragNum, &fragLen );

	// Get fragments
	Fragment *frags = new Fragment[fragNum];
	Node nodes[1024];
	vector< vector< int > > starts( 1024 ), endings( 1024 );
	for ( int i = 0; i < fragNum; ++i )
	{
		frags[i].fragment.resize( fragLen );
		fscanf( inFile, "%s\n", (char*)frags[i].fragment.c_str() );
		frags[i].startId = getId( frags[i].fragment.c_str() );
		frags[i].endId = getId( frags[i].fragment.c_str() + fragLen - 5 );
		frags[i].used = false;
		starts[frags[i].startId].push_back( i );
		endings[frags[i].endId].push_back( i );
		nodes[frags[i].startId].edgesOut.push_back( i );
		nodes[frags[i].endId].edgesIn.push_back( i );
	}

	// Look for start fragment
	int startId = 0;
	for ( int pos = 0; pos < 1024; ++pos )
	{
		if ( nodes[pos].edgesOut.size() > nodes[pos].edgesIn.size() )
		{
			startId = pos;
			break;
		}
	}

	// Search
	vector< pair< int, typename vector<int>::iterator > > traceStack; // act. fragment, node's edgesOut iterator (next fragment)
	traceStack.reserve( fragNum + 1 ); // kicsit csalunk...
	traceStack.push_back( make_pair( -1, nodes[startId].edgesOut.begin() ) );
	do
	{
		if ( traceStack.size() == fragNum + 1 )
			break;
		pair<int, typename vector<int>::iterator> &actEdge( traceStack.back() );
		for ( ; actEdge.second != nodes[frags[actEdge.first].endId].edgesOut.end(); ++actEdge.second )
		{
			if ( !frags[*actEdge.second].used )
			{
				frags[*actEdge.second].used = true;
				traceStack.push_back( make_pair( *actEdge.second, nodes[frags[*actEdge.second].endId].edgesOut.begin() ) );
				break;
			}
		}
		if ( actEdge.second == nodes[frags[actEdge.first].endId].edgesOut.end() )
		{
			frags[actEdge.first].used = false;
			traceStack.pop_back();
			if ( traceStack.back().second != nodes[frags[traceStack.back().first].endId].edgesOut.end() )
				++traceStack.back().second;
		}
	}
	while ( traceStack.size() );

	// Print output
	if ( !traceStack.empty() )
	{
		for ( int i = 1; i < fragNum; fprintf( outFile, frags[traceStack[i++].first].fragment.substr( 0, fragLen - 5 ).c_str() ) );
		fprintf( outFile, frags[traceStack.back().first].fragment.c_str() );
	}
	else
	{
		printf( "Csecs van..." );
	}

	// Clean-up
	delete []frags;

	return 0;
}