#include <stdio.h>
#include <string>
#include <stack>
#include <iostream>

using namespace std;

#define INPUT_FILE		"in\\B-4.in"
#define OUTPUT_FILE		"out\\B-4.out"

struct sIdea
{
	string expression;
	size_t firstNonTerm;
	
	sIdea( const string &expression, size_t firstNonTerm )
		: expression( expression )
		, firstNonTerm( firstNonTerm )
	{ }
};

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

	int length;
	fscanf( inFile, "%d\n", &length );

	// naive: S = (S) | S + S | S - S | S * S | S / S | sin(S) | cos(S) | x | y
	//
	// better:	S = T | T + S | T - S | T * S | T / S
	//			T = x | y | (S) | sin(S) | cos(S)

	stack<sIdea> ideas;
	ideas.push( sIdea( "S", 0 ) );

	while ( !ideas.empty() )
	{
		sIdea actIdea( ideas.top() );
		ideas.pop();

		if ( actIdea.expression.length() == length && actIdea.firstNonTerm == length )
		{
			fprintf( outFile, "%s\n", actIdea.expression.c_str() );
		}
		else
		{
			switch( actIdea.expression[actIdea.firstNonTerm] )
			{
			case 'T':
				ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "x" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm + 1 ) );
				ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "y" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm + 1 ) );
				if ( length - actIdea.expression.length() >= 2 )
				{
					ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "(S)" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm + 1 ) );
					if ( length - actIdea.expression.length() >= 5 )
					{
						ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "sin(S)" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm + 4 ) );
						ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "cos(S)" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm + 4 ) );
					}
				}
				break;
			case 'S':
				ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "T" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm ) );
				if ( length - actIdea.expression.length() >= 2 )
				{
					ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "T+S" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm ) );
					ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "T-S" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm ) );
					ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "T/S" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm ) );
					ideas.push( sIdea( actIdea.expression.substr( 0, actIdea.firstNonTerm ) + "T*S" + actIdea.expression.substr( actIdea.firstNonTerm + 1 ), actIdea.firstNonTerm ) );
				}
				break;
			default:
				if ( actIdea.firstNonTerm < actIdea.expression.length() )
				{
					++actIdea.firstNonTerm;
					ideas.push( actIdea );
				}
				break;
			}
		}
	}

	return 0;
}