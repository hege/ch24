
#include <string>

#include <stdio.h>
#include <conio.h>
#include <assert.h>

#include <libpng/png.h>
#include <TernaryGraph/bitvector.h>



struct sLetter
{
	char		name;
	bit_vector	*bitpic;
	size_t		count;
};



#ifdef _DEBUG
void waitBeforeExit()
{
	printf("Press any key to exit...");
	getch();
}
#endif


int main( int argc, char *argv[] )
{
	// Press any key before exit...
	#ifdef _DEBUG
		atexit(waitBeforeExit);
	#endif


	// Parameters' check
	if ( argc != 2 )
	{
		fprintf(stderr, "Please specify (only) the input name\n\n");
		return -1;
	}

	std::string fileName(argv[1]);


	// Initilalize PNG enviroment
	png_structp	png_ptr;
	png_infop	info_ptr;

	if ( (png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL )
	{
		fprintf(stderr, "Error initializing PNG enviroment\n\n");
		return -1;
	}
	if ( (info_ptr = png_create_info_struct(png_ptr)) == NULL )
	{
		fprintf(stderr, "Error initializing PNG enviroment\n\n");
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return -1;
	}


	// Read input data file
	FILE *descFile;
	if ( (descFile = fopen(("in\\" + fileName + ".in").c_str(), "rb")) == NULL )
	{
		fprintf(stderr, "Error opening %s.in\n\n", argv[1]);
		return -1;
	}


	// Process letters
	
	int char_width, char_height, char_num, at, at2, x, y;

	fscanf(descFile, "%d %d %d\n", &char_num, &char_width, &char_height);

	// Create empty letters
	sLetter *chars = new sLetter[char_num * 4];
	for ( at = 0; at < char_num; at++ )
	{
		chars[at].count = 0;

		chars[at           ].bitpic = new bit_vector[char_height];
		chars[at + char_num].bitpic = new bit_vector[char_height];
		for ( y = 0; y < char_height; y++ )
		{
			chars[at           ].bitpic[y].resize(char_width);
			chars[at + char_num].bitpic[y].resize(char_width);
		}

		chars[at + char_num * 2].bitpic = new bit_vector[char_width];
		chars[at + char_num * 3].bitpic = new bit_vector[char_width];
		for ( y = 0; y < char_width; y++ )
		{
			chars[at + char_num * 2].bitpic[y].resize(char_height);
			chars[at + char_num * 3].bitpic[y].resize(char_height);
		}
	}
	// Read letters
	for ( at = 0; at < char_num; at++ )
	{
 		fscanf(descFile, "%c\n", &(chars[at].name));
        for ( y = 0; y < char_height; y++ )
		{
			for ( x = 0; x < char_width; x++ )
			{
				if ( fgetc(descFile) == '*' )
				{
					chars[at               ].bitpic[y                  ].set_bit(x                  , bit_vector::true_value);
					chars[at + char_num    ].bitpic[char_height - y - 1].set_bit(char_width  - x - 1, bit_vector::true_value);
					chars[at + char_num * 2].bitpic[char_width  - x - 1].set_bit(y                  , bit_vector::true_value);
					chars[at + char_num * 3].bitpic[x                  ].set_bit(char_height - y - 1, bit_vector::true_value);
				}
				else
				{
					chars[at               ].bitpic[y                  ].set_bit(x                  , bit_vector::false_value);
					chars[at + char_num    ].bitpic[char_height - y - 1].set_bit(char_width  - x - 1, bit_vector::false_value);
					chars[at + char_num * 2].bitpic[char_width  - x - 1].set_bit(y                  , bit_vector::false_value);
					chars[at + char_num * 3].bitpic[x                  ].set_bit(char_height - y - 1, bit_vector::false_value);
				}
			}
			fgetc(descFile); // the newline character
		}
	}


	// Read PNG file
	FILE *pngFile;
	if ( (pngFile = fopen(("in\\" + fileName + ".png").c_str(), "rb")) == NULL )
	{
		fprintf(stderr, "Error opening %s.png\n\n", argv[1]);
		return -1;
	}

	unsigned int sig_read = 0;

	png_init_io(png_ptr, pngFile);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, png_voidp_NULL);

	// Convert image into bitvector
	int height = info_ptr->height, width = info_ptr->width;	
	bit_vector *bitpic = new bit_vector[height];

	for ( y = 0; y < height; y++ ) {
		bitpic[y].resize(width);
		for ( x = 0; x < width; x++ ) {
			bitpic[y].set_bit(x, info_ptr->row_pointers[y][x] ? bit_vector::true_value : bit_vector::false_value);
		}
	}

	// Scan for letters
	int off = 0;
	for ( y = 0; y < height - char_height; y++ )
	{
		for ( x = 0; x < width - char_width; x++ )
		{
			for ( at = 0; at < char_num; at++ )
			{
				// Match with normal
				if ( !bitpic[y].compare_range(x, char_width, chars[at].bitpic[0], 0) )
				{
					for ( off = 1; off < char_height; off++ )
					{
						if ( bitpic[y + off].compare_range(x, char_width, chars[at].bitpic[off], 0) )
							break;
					}
					if ( off == char_height )
					{
						for ( off = 0; off < char_height; off++ ) {
							bitpic[y + off].import_range(x, char_width, 0);
						}
						x += char_width;
						chars[at].count++;
						break;
					}
				}
				// Match with 180 degree rotate
				at2 = at + char_num;
				if ( !bitpic[y].compare_range(x, char_width, chars[at2].bitpic[0], 0) )
				{
					for ( off = 1; off < char_height; off++ )
					{
						if ( bitpic[y + off].compare_range(x, char_width, chars[at2].bitpic[off], 0) )
							break;
					}
					if ( off == char_height )
					{
						for ( off = 0; off < char_height; off++ ) {
							bitpic[y + off].import_range(x, char_width, 0);
						}
						chars[at].count++;
						x += char_width;
						break;
					}
				}
				// Match with 90 degree rotate
				at2 += char_num;
				if ( !bitpic[y].compare_range(x, char_height, chars[at2].bitpic[0], 0) )
				{
					for ( off = 1; off < char_width; off++ )
					{
						if ( bitpic[y + off].compare_range(x, char_height, chars[at2].bitpic[off], 0) )
							break;
					}
					if ( off == char_width )
					{
						for ( off = 0; off < char_width; off++ ) {
							bitpic[y + off].import_range(x, char_height, 0);
						}
						x += char_height;
						chars[at].count++;
						break;
					}
				}
				// Match with 270 degree rotate
				at2 += char_num;
				if ( !bitpic[y].compare_range(x, char_height, chars[at2].bitpic[0], 0) )
				{
					for ( off = 1; off < char_width; off++ )
					{
						if ( bitpic[y + off].compare_range(x, char_height, chars[at2].bitpic[off], 0) )
							break;
					}
					if ( off == char_width )
					{
						for ( off = 0; off < char_width; off++ ) {
							bitpic[y + off].import_range(x, char_height, 0);
						}
						x += char_height;
						chars[at].count++;
						break;
					}
				}
			}
		}
	}

	// Write the output file
	FILE *outFile;
	if ( (outFile = fopen(("myOut\\" + fileName + ".out").c_str(), "wb")) == NULL )
	{
		fprintf(stderr, "Error opening output file for writing\n\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
		fcloseall();
		return -1;
	}

	for ( at = 0; at < char_num; at++ )
	{
		fprintf(outFile, "%c %d\n", chars[at].name, chars[at].count);

/*		fprintf(outFile, "\n");
		for ( y = 0; y < char_height; y++ ) {
			for ( x = 0; x < char_width; x++ ) {
				fputc(chars[at].bitpic[y].get_bit(x) == bit_vector::true_value ? '*' : '.', outFile);
			}
			fputc('\n', outFile);
		}
		fprintf(outFile, "\n");
		for ( y = 0; y < char_height; y++ ) {
			for ( x = 0; x < char_width; x++ ) {
				fputc(chars[at + char_num].bitpic[y].get_bit(x) == bit_vector::true_value ? '*' : '.', outFile);
			}
			fputc('\n', outFile);
		}
		fprintf(outFile, "\n");
		for ( y = 0; y < char_width; y++ ) {
			for ( x = 0; x < char_height; x++ ) {
				fputc(chars[at + char_num * 2].bitpic[y].get_bit(x) == bit_vector::true_value ? '*' : '.', outFile);
			}
			fputc('\n', outFile);
		}
		fprintf(outFile, "\n");
		for ( y = 0; y < char_width; y++ ) {
			for ( x = 0; x < char_height; x++ ) {
				fputc(chars[at + char_num * 3].bitpic[y].get_bit(x) == bit_vector::true_value ? '*' : '.', outFile);
			}
			fputc('\n', outFile);
		}*/
	}

	// Final clean-up
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fcloseall();

	printf("All done.\n\n");
	return 0;
}