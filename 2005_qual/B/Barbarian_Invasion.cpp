
#include <string>

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <vector>


#ifdef _DEBUG
void waitBeforeExit()
{
	printf("Press any key to exit...");
	getch();
}
#endif


typedef char* pchar_t;

struct sCoord
{
	int x;
	int y;
};

void DrawTriangle( pchar_t map, x1, y1, x2, y2, x3, y2 )
{
	///
}


int main( int argc, char *argv[] )
{
#ifdef _DEBUG
	atexit(waitBeforeExit);
#endif

	if ( argc != 3 )
	{
		printf("Usage: ___.exe <input file> <output file>\n\n");
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

	int cityNum, i, j, k, maxX, maxY, skipCity;
	std::vector<sCoord> cityCoords;
	std::vector<int>	walledCities;
	bool **map;

	for(;;)
	{
		fscanf(fin, "%d\n", &cityNum);

		if ( !cityNum )
			break;

		cityCoords.resize(cityNum);
		walledCities.clear();
		walledCities.reserve(cityNum);
		maxX = maxY = 0;
		for ( i = 0; i < cityNum; i++ )
		{
			fscanf(fin, "%d%d\n", &cityCoords[i].x, &cityCoords[i].y);
			if ( maxX < cityCoords[i].x )
				maxX = cityCoords[i].x;
			if ( maxY < cityCoords[i].y )
				maxY = cityCoords[i].y;
		}

		pchar_t *map = new pchar_t[maxX];
		for ( i = 0; i < maxX; i++ )
		{
			map[i] = new char[maxY];
			memset(map[i], 0, maxY);
		}

		for ( skipCity = 0; skipCity < cityNum; skipCity++ )
		{
			for ( i = 3; i < cityNum; i++ )
			{
				if ( i == skipCity )
					continue;
				else
				{
					if ( walledCities.size() < 3 )
					{
						walledCities.push_back(i);
						if ( walledCities.size() == 3 )
						{
							DrawTriangle( map, cityCoords[walledCities[0]].x, cityCoords[walledCities[0]].y
											 , cityCoords[walledCities[1]].x, cityCoords[walledCities[1]].y 
											 , cityCoords[walledCities[2]].x, cityCoords[walledCities[2]].y );
						}
					}
					else
					{
						if ( map[cityCoords[i].x][cityCoords[i].y] )
							continue;
						else
						{
							for ( j = 0; j < walledCities.size(); j++ )
							{
								for ( k = j + 1; k < walledCities.size(); k++ )
								{
									DrawTriangle( map, cityCoords[i].x, cityCoords[i].y
													 , cityCoords[walledCities[j]].x, cityCoords[walledCities[j]].y 
													 , cityCoords[walledCities[k]].x, cityCoords[walledCities[k]].y );
								}
							}
							// Filter enclosed nodes
							for ( std::vector<int>::iterator it = walledCities.begin(); it != walledCities.end(); )
							{
								if ( map[cityCoords[walledCities[*it]].x][cityCoords[walledCities[*it]].y] )
									it = walledCities.begin
							}
							// Done
							walledCities.push_back(i);
						}
					}
				}
			}
		}









		// Write it out
		fprintf(fout, "%d\n", (int)floor(wallLength));

		for ( i = 0; i < maxX; i++ )
		{
			delete[] map[i];
		}
		delete[] map;
	}
}