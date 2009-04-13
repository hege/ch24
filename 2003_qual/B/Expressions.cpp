
#include <conio.h>
#include <list>
#include <string>

// OP = { +, -, *, / }
// VAR = { x, y }
// FUNC = { cos, sin }
// PAR = { (, ) }

// T = x | y
// F = sin | cos
// S = T | (S) | F(S) | S+S | S-S | S*S | S/S

struct sSymb
{
	std::string	sym;
	size_t		parsPos;
	bool		bTerminal;
};


void waitChar()
{
	printf("Press any key to exit...\n\n");
	getchar();
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

	char buf[80];
	fgets(buf, 80, inFile);
	int maxLen = atoi(buf);

	fclose(inFile);

	strcpy(buf, argv[1]);
	strcpy(buf + ( strlen(buf) - 2 ), "out");
	FILE *outFile = fopen(buf, "wb");
	if ( !outFile ) {
		printf("Cant open output file \'%s\'\n", buf);
		return -1;
	}

	// Do the parsing
	size_t pos, len;
	std::list<sSymb> syms;
	sSymb newSym, actSym = { "S", 0, true };
	newSym.bTerminal = true;
	syms.push_front(actSym);
	while ( !syms.empty() )
	{
		actSym = syms.front();
		syms.pop_front();
		len = actSym.sym.length();
		for ( pos = actSym.parsPos; pos < len; pos++ )
		{
			if ( actSym.sym[pos] == 'T' )
			{
				actSym.bTerminal = false;

				newSym.parsPos = pos + 1;

				newSym.sym = actSym.sym.substr(0, pos) + "x" + actSym.sym.substr(pos + 1);
				syms.push_front(newSym);
				newSym.sym = newSym.sym.substr(0, pos) + "y" + actSym.sym.substr(pos + 1);
				syms.push_front(newSym);

				if ( len + 2 <=  maxLen )
				{
					newSym.sym = actSym.sym.substr(0, pos) + "(S)" + actSym.sym.substr(pos + 1);
					syms.push_front(newSym);

					if ( len + 3 <=  maxLen )
					{
						newSym.parsPos = pos;
						newSym.sym = actSym.sym.substr(0, pos) + "F(S)" + actSym.sym.substr(pos + 1);
						syms.push_front(newSym);
					}
				}
				break;
			}
			else if ( actSym.sym[pos] == 'F' )
			{
				actSym.bTerminal = false;

				if ( len + 2 <= maxLen )
				{
					newSym.parsPos = pos + 3;

					newSym.sym = actSym.sym.substr(0, pos) + "cos" + actSym.sym.substr(pos + 1);
					syms.push_front(newSym);
					newSym.sym = actSym.sym.substr(0, pos) + "sin" + actSym.sym.substr(pos + 1);
					syms.push_front(newSym);
				}
				break;

			}
			else if ( actSym.sym[pos] == 'S' )
			{
				actSym.bTerminal = false;

				newSym.parsPos = pos;

				newSym.sym = actSym.sym.substr(0, pos) + "T" + actSym.sym.substr(pos + 1);
				syms.push_front(newSym);

				if ( len + 2 <=  maxLen )
				{
					newSym.sym = actSym.sym.substr(0, pos) + "T+S" + actSym.sym.substr(pos + 1);
					syms.push_front(newSym);
					newSym.sym = actSym.sym.substr(0, pos) + "T-S" + actSym.sym.substr(pos + 1);
					syms.push_front(newSym);
					newSym.sym = actSym.sym.substr(0, pos) + "T*S" + actSym.sym.substr(pos + 1);
					syms.push_front(newSym);
					newSym.sym = actSym.sym.substr(0, pos) + "T/S" + actSym.sym.substr(pos + 1);
					syms.push_front(newSym);
				}

				break;
			}
		}
		if ( pos == maxLen && actSym.bTerminal )
			fprintf(outFile, "%s\n", actSym.sym.c_str());
	}

	fclose(outFile);

	return 0;
}