
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <crtdbg.h>

using namespace std;


struct Fragment
{
	string			fragment;
	unsigned short	beginId;
	unsigned short	endId;
	bool			used;
};

struct Node
{
	vector< pair< Node*, Fragment* > > edgesOut;
	vector< pair< Node*, Fragment* > > edgesIn;
};

Node * nodes[1024];

vector<int> startIdx[1024];
vector<int> endIdx[1024];


bool SolveRecursive( Fragment *frags, Node *pNode, int step, int n, vector<int> &traceStack )
{
	if ( step == n )
		return true;

	// Find a suitable outgoing edge
	vector< pair< Node*, Fragment* > >::iterator it( pNode->edgesOut.begin() ), end( pNode->edgesOut.end() );
	for ( ; it != end; ++it )
	{
		if ( !it->second->used )
		{
			traceStack.push_back( (int)( it->second - frags ) );
			it->second->used = true;
			if ( SolveRecursive( frags, it->first, step+1, n, traceStack ) )
				return true;
			traceStack.pop_back();
			it->second->used = false;
		}
	}
	return false;
}

bool Solve( Fragment *frags, Node *pNode, int step, int n, vector<int> &traceStack )
{
	vector< pair< Node*, Fragment* > >::iterator it, end;
	vector< typename vector< pair< Node*, Fragment* > >::iterator > itStack( n );
	itStack.push_back( pNode->edgesOut.begin() );
	end = pNode->edgesOut.end();
	do
	{
		if ( step == n )
			return true;

		it = itStack.back();

		for ( ; it != end; ++it )
		{
			if ( !it->second->used )
			{
				traceStack.push_back( (int)( it->second - frags ) );
				it->second->used = true;
				itStack.push_back( it->first->edgesOut.begin() );
				end = it->first->edgesOut.end();
				++step;
				break;
			}
		}
		if ( it == end )
		{
			frags[traceStack.back()].used = false;
			traceStack.pop_back();
			itStack.pop_back();
			end = nodes[traceStack.back()]->edgesOut.end();
			assert( step ); // There should be a solution
			--step;
		}
	}
	while ( step );

	return false;
}

int main( int argc, char *argv[] )
{
	memset( nodes, 0, sizeof( nodes ) );

	// Check arguments
	if ( argc != 2 )
	{
		printf( "Usage: DNA.exe <input file>\n\n" );
		return -1;
	}

	// Input file
	ifstream inFile( argv[1], ios_base::in | ios_base::binary );
	if ( !inFile.is_open() )
	{
		printf( "Error opening input file '%s'\n\n", argv[1] );
		return -1;
	}

	// Output file
	char buf2[80];
	strcpy( buf2, argv[1] );
	strcpy( buf2 + ( strlen( buf2 ) - 2 ), "out" );
	ofstream outFile( buf2, ios_base::out | ios_base::binary );
	if ( !outFile )
	{
		printf( "Cant open output file \'%s\'\n", buf2 );
		return -1;
	}

	// Process input
	unsigned char conv[128];
	conv['A'] = 0;
	conv['C'] = 1;
	conv['T'] = 2;
	conv['G'] = 3;

	int n, l, i;
	inFile >> n >> l;
	inFile.getline( buf2, sizeof( buf2 ) ); // pop the rest...
	Fragment *frags = new Fragment[n];
	for ( i = 0; i < n; ++i )
	{
		getline( inFile, frags[i].fragment );
		frags[i].beginId =	( conv[frags[i].fragment[0]] << 8 ) |
							( conv[frags[i].fragment[1]] << 6 ) |
							( conv[frags[i].fragment[2]] << 4 ) |
							( conv[frags[i].fragment[3]] << 2 ) |
							( conv[frags[i].fragment[4]] );
		frags[i].endId =	( conv[frags[i].fragment[l-5]] << 8 ) |
							( conv[frags[i].fragment[l-4]] << 6 ) |
							( conv[frags[i].fragment[l-3]] << 4 ) |
							( conv[frags[i].fragment[l-2]] << 2 ) |
							( conv[frags[i].fragment[l-1]] );
		startIdx[frags[i].beginId].push_back( i );
		endIdx[frags[i].endId].push_back( i );
		frags[i].used = false;

		if ( !nodes[frags[i].beginId] )
			nodes[frags[i].beginId] = new Node;
		if ( !nodes[frags[i].endId] )
			nodes[frags[i].endId] = new Node;
		nodes[frags[i].beginId]->edgesOut.push_back( make_pair( nodes[frags[i].endId],   frags+i ) );
		nodes[frags[i].endId  ]->edgesIn. push_back( make_pair( nodes[frags[i].beginId], frags+i ) );
	}

	bool bSolved( false );
	vector<int> traceStack;
	do
	{
		// Search for possible start node
		for ( i = 0; i < 1024; ++i )
			if ( nodes[i] && nodes[i]->edgesIn.size() < nodes[i]->edgesOut.size() )
				break;
		if ( i != n )
		{
			bSolved = Solve( frags, nodes[i], 0, n, traceStack );
			break;
		}
		// OK, anyone could do it...
		for ( i = 0; i < 1024; ++i )
		{
			if ( nodes[i] )
			{
				bSolved = Solve( frags, nodes[i], 0, n, traceStack );
				break;
			}
		}
	}
	while(0);


	if ( !bSolved )
	{
		cerr << "Vmit elbasztam..." << endl << endl;
	}
	else
	{
		cerr << "Hurray!" << endl << endl;
		for ( i = 0; i < n-1; ++i )
			outFile << frags[traceStack[i]].fragment.substr( 0, l-5 );
		outFile << frags[traceStack.back()].fragment;
	}

	// Cleanup
	for ( i = 0; i < 1024; ++i )
		if ( nodes[i] )
			delete nodes[i];
	delete []frags;

	return 0;
}
