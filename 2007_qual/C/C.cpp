
#include <stdio.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

char in[] = "test.in";
char out[] = "test.out";

class cSyn
{
public:
	unsigned char id[128];
	set< string > words;
	
	cSyn()
	{
		memset( id, 0, sizeof( id ) );
	}
	void Add( const string &word, size_t idx )
	{
		words.insert( word );
		id[idx/8] |= (unsigned char)( 1 << ( idx % 8 ) );
	}
	bool Check( unsigned char *id2 )
	{
		bool bRet = true;
		for ( int i = 0; i < sizeof( id ) && bRet; ++i )
			bRet = ( ( id[i] & id2[i] ) == id[i] );
		return bRet;
	}
};

int main( int argc, char *argv[] )
{
	FILE *fin = fopen( in, "r" );
	if ( !fin )
	{
		cout << "Error opening input file!" << endl << endl;
		return -1;
	}
	FILE *fout = fopen( out, "w" );
	if ( !fout )
	{
		cout << "Error opening output file!" << endl << endl;
		return -1;
	}

	int K, L, M;
	char line[256], *token;
	map< string, vector< string > > A2C, B2C;
	map< string, vector< string > >::iterator A2Cit, B2Cit;
	vector< cSyn > syns;
	map< string, set< size_t > > C2M;
	map< string, set< size_t > >::iterator C2M_it;
	map< string, size_t > Cs;
	map< string, size_t >::iterator Cs_it;


	fscanf( fin, "%d %d %d\n", &K, &L, &M );
	for ( int i = 0; i < K; ++i )
	{
		fgets( line, sizeof( line ), fin );
		A2Cit = A2C.insert( make_pair( strtok( line, " \n" ), vector<string>() ) ).first;
		for ( token = strtok( NULL, " \n" ); token; token = strtok( NULL, " \n" ) )
		{
			A2Cit->second.push_back( token );
		}
	}
	for ( int i = 0; i < L; ++i )
	{
		fgets( line, sizeof( line ), fin );
		B2Cit = B2C.insert( make_pair( strtok( line, " \n" ), vector<string>() ) ).first;
		for ( token = strtok( NULL, " \n" ); token; token = strtok( NULL, " \n" ) )
		{
			B2Cit->second.push_back( token );
		}
	}
	syns.reserve( M );
	for ( int i = 0; i < M; ++i )
	{
		fgets( line, sizeof( line ), fin );
		syns.push_back( cSyn() );
		for ( token = strtok( line, " \n" ); token; token = strtok( NULL, " \n" ) )
		{
			Cs_it = Cs.insert( make_pair( token, Cs.size() ) ).first;
			syns.back().Add( token, Cs_it->second );
			C2M_it = C2M.insert( make_pair( token, set<size_t>() ) ).first;
			C2M_it->second.insert( i );
		}
	}
	// forditsuk ki B2C-t
	map< size_t, set< string > > M2B;
	map< size_t, set< string > >::iterator M2Bit;
	for ( B2Cit = B2C.begin(); B2Cit != B2C.end(); ++B2Cit )
	{
		unsigned char id[128], mask[128];
		size_t nId;
		memset( id, 0, sizeof( id ) );
		memset( mask, 0, sizeof( mask ) );
		for ( vector<string>::iterator it = B2Cit->second.begin(); it != B2Cit->second.end(); ++it )
		{
			nId = Cs.find( *it )->second;
			id[nId/8] |= ( 1 << ( nId % 8 ) );
		}
		for ( vector< cSyn >::iterator it = syns.begin(); it != syns.end(); ++it )
		{
			if ( it->Check( id ) )
			{
				M2Bit = M2B.insert( make_pair( (int)(it - syns.begin()), set<string>() ) ).first;
				M2Bit->second.insert( B2Cit->first );
			}
		}
	}

	// index 'em
	map< string, set< string > > A2B;
	map< string, set< string > >::iterator A2Bit;
	for ( A2Cit = A2C.begin(); A2Cit != A2C.end(); ++A2Cit )
	{
		unsigned char id[128], mask[128];
		size_t nId;
		memset( id, 0, sizeof( id ) );
		memset( mask, 0, sizeof( mask ) );
		//for ( vector<string>::iterator it = A2Cit->second.begin(); it != A2Cit->second.end(); ++it )
		//{
		//	nId = Cs.find( *it )->second;
		//	id[nId/8] |= ( 1 << ( nId % 8 ) );
		//}
		//for ( vector< cSyn >::iterator it = syns.begin(); it != syns.end(); ++it )
		//{
		//	if ( it->Check( id ) )
		//	{
		//		if ( ( M2Bit = M2B.find( (int)(it - syns.begin()) ) ) == M2B.end() )
		//			break;
		//		else
		//		{
		//			A2Bit = A2B.insert( make_pair( A2Cit->first, set<string>() ) ).first;
		//			for ( set<string>::iterator bit = M2Bit->second.begin(); bit != M2Bit->second.end(); ++bit )
		//				A2Bit->second.insert( *bit );
		//			for ( int i = 0; i < sizeof( id ); ++i )
		//				mask[i] |= it->id[i];
		//		}
		//	}
		//}
		//if ( it != syns.end() )
		//{
		//	fprintf( fout, "%s AMBIGUOUS\n", A2Cit->first.c_str() );
		//	continue;
		//}
		//for ( int i = 0; i < sizeof( id ); ++i )
		//	if ( id[i] != mask[i] )
		//		break;
		//if ( i != sizeof( id ) )
		//{
		//	fprintf( fout, "%s AMBIGUOUS\n", A2Cit->first.c_str() );
		//	continue;
		//}
		for ( vector<string>::iterator it = A2Cit->second.begin(); it != A2Cit->second.end(); ++it )
		{
			C2M_it = C2M.find( *it );
			for ( set<size_t>::iterator sit = C2M_it->second.begin(); sit != C2M_it->second.end(); ++sit )
			{
				if ( ( M2Bit = M2B.find( *sit ) ) == M2B.end() || M2Bit->second.empty() )
					break;
				A2Bit = A2B.insert( make_pair( A2Cit->first, set<string>() ) ).first;
				for ( set<string>::iterator bit = M2Bit->second.begin(); bit != M2Bit->second.end(); ++bit )
					A2Bit->second.insert( *bit );
			}
			if ( sit != C2M_it->second.end() )
				break;
		}
		if ( it != A2Cit->second.end() )
		{
			fprintf( fout, "%s AMBIGUOUS\n", A2Cit->first.c_str() );
			continue;
		}
		fprintf( fout, "%s", A2Cit->first.c_str() );
		for ( set<string>::iterator it = A2Bit->second.begin(); it != A2Bit->second.end(); ++it )
		{
			fprintf( fout, " %s ", it->c_str() );
		}
		fprintf( fout, "\n" );
	}


	fcloseall();
	return 0;
}
