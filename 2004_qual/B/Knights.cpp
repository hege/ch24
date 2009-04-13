
#include <stdio.h>
#include <string.h>
#include <set>
#include <string>


char needs[14][14];
char hasneed[14];
char hates[14][14];
char was[14];
char needok[14];
std::set<std::string> outs;

void findk(int k[14], int pos)
{
	if ( pos == 14 )
	{
		if ( !hates[k[13]][k[0]] && !hates[k[0]][k[13]] &&
			 ( !hasneed[k[13]] || needok[k[13]] || needs[k[13]][k[0]] ) &&
			 ( !hasneed[k[0]] || needs[k[0]][k[13]] || needs[k[0]][k[1]] ) )
		{
			char buf[15];
			buf[14] = 0;
			for ( int i = 0; i < 14; i++ )
				buf[i] = k[i] + 'a';
			outs.insert(std::string(buf));
		}
	}
	else
	{
		for ( int i = 0; i < 14; i++ )
		{
			if ( was[i] || hates[k[pos-1]][i] || hates[i][k[pos-1]] ||
				 ( hasneed[k[pos-1]] && !needok[k[pos-1]] && !needs[k[pos-1]][i] ) )
				continue;
			else
			{
				if ( hasneed[i] )
				{
					needok[i] = needs[i][k[pos-1]] ? 1 : 0;
					was[i] = 1;
					k[pos] = i;
					findk(k, pos+1);
					needok[i] = 0;
					was[i] = 0;
				}
				else
				{
					needok[i] = 1;
					was[i] = 1;
					k[pos] = i;
					findk(k, pos+1);
					was[i] = 0;
				}
			}
		}
	}
}

void waitChar()
{
	printf("Press any key to exit...\n\n");
	getchar();
}

int main( int argc, char *argv[] )
{
#ifdef _DEBUG
	atexit(waitChar);
#endif

	if ( argc != 2 ) {
		printf("Need file parameter\n");
		return -1;
	}

	char buf[128];

	strcpy(buf, argv[1]);
	strcat(buf, ".in");
	FILE *inFile = fopen(buf, "rb");
	if ( !inFile ) {
		printf("Cant open input file \'%s\'\n", buf);
		return -1;
	}

	int i, ruleNum, knight;
	size_t len, pos;

	memset(needs, 0, 196);
	memset(hasneed, 0, 14);
	fscanf(inFile, "%d %d\n", &i, &ruleNum);
	for ( i = 0; i < ruleNum; i++ )
	{
		fgets(buf, 128, inFile);
		len = strlen(buf);
		knight = buf[0] - 'a';
		// hates
		if ( buf[2] == 'h' ) {
			for ( pos = 8; pos < len; pos += 6 ) {
				hates[knight][buf[pos] - 'a'] = 1;
			}
		}
		// needs
		else {
			hasneed[knight] = 1;
			for ( pos = 8; pos < len; pos += 5 ) {
				needs[knight][buf[pos] - 'a'] = 1;
			}
		}
	}

	fclose(inFile);

	int k[14];

	// make the main stuff
	memset(k, 0, 14);
	memset(needok, 0, 14);
	for ( i = 0; i < 14; i++ )
	{
		k[0] = i;
		was[i] = 1;
		needok[i] = 1;
		findk(k, 1);
		was[i] = 0;
		needok[i] = 0;
	}

	// write out
	strcpy(buf, argv[1]);
	strcat(buf, ".out");
	FILE *outFile = fopen(buf, "wb");
	if ( !outFile ) {
		printf("Cant open output file \'%s\'\n", buf);
		return -1;
	}
	for ( std::set<std::string>::iterator it = outs.begin(); it != outs.end(); it++ ) {
		fprintf(outFile, "%s\n", it->c_str());
	}
}    
