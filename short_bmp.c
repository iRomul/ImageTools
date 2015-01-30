#include <time.h>
#include <math.h>
#include <string.h>
#include "short_bmp.h"

int _0(int w) {

	return( 3 * (w + 1) - ((w + 1) * 3 % 4) - 3 * w );

}

/* Image basic functions */

int		image_mem_alloc(Image* image, Size2i size) {

	image->matrix_ptr = (BGRComponents*) malloc(
		sizeof(BGRComponents) * 
		size.height *
		size.width
		);

	image->size = size;

	return (int)image->matrix_ptr;

}

void	image_mem_free(Image* image) {

	free(image->matrix_ptr);
	image->matrix_ptr = NULL;

}

/* Reading/writing image from/to BMP24 */

int		image_load(FILE* file_handler, Image* image) {

	int result;
	uint32_t delta = 0;
	uint32_t i = 0;

	BMPHeader bmp_header;
	Size2i size = {0, 0};
	
	result = fread(&bmp_header, sizeof(BMPHeader), 1, file_handler);
	if( result != 1 ) {
		fwprintf_s(stderr, L"Unexpected end of file\n");
		return 0;
	}
	
	if(
		bmp_header.sig[0] != 'B'
		||
		bmp_header.sig[1] != 'M'
		) {

		fwprintf_s(stderr, L"Thus file probably not bitmap image\n");
		return 0;

	}

	if( bmp_header.bit_per_pix != 24 )
		fwprintf_s(stderr, L"This format is unsupported, BMP24 only\n");

	size.height = bmp_header.height;
	size.width = bmp_header.width;

	if( !image_mem_alloc(image, size) ) {
		fwprintf_s(stderr, L"Malloc return NULL\n");
		return 0;
	}

	image->size = size;

	delta = _0(size.width);

	rewind(file_handler);
	fseek(file_handler, bmp_header.ptr_to_data, SEEK_CUR);

	for( i = 0; i < size.height; i++ ) {

		fread(
			&(image->matrix_ptr[ i * size.width ]),
			sizeof(BGRComponents),
			size.width,
			file_handler
			);

		fseek(file_handler, (int)delta, SEEK_CUR);

	}

	fclose(file_handler);

	return 1;

}

void	image_save(FILE* file_handler, Image* image) {

	uint32_t delta = 0;
	uint32_t i = 0;
	uint8_t noise = 0;

	Size2i size = image->size;

	BMPHeader bmp_header;
	// BITMAPFILEHEADER
	bmp_header.sig[0] = 'B';
	bmp_header.sig[1] = 'M';
	bmp_header.file_size = sizeof(BMPHeader) + sizeof(BGRComponents) * size.height * size.width; // FIX
	bmp_header.empty1 = 0;
	bmp_header.empty2 = 0;
	bmp_header.ptr_to_data = sizeof(BMPHeader);
	// BITMAPINFO
	bmp_header.header_size = sizeof(BMPHeader) - 14;
	bmp_header.width = size.width;
	bmp_header.height = size.height;
	bmp_header.color_planes = 1;
	bmp_header.bit_per_pix = 24;
	bmp_header.compression = 0; /* BI_RGB */
	// TODO: Fix
	bmp_header.pixel_array_length = 0; //FIX
	bmp_header.x_resolution = 3779; /* 96 PPI ~ 3779 PPM */
	bmp_header.y_resolution = 3779; /* 96 PPI ~ 3779 PPM */
	bmp_header.colors = 0;
	bmp_header.primary_colors = 0;

	fwrite(&bmp_header, sizeof(BMPHeader), 1, file_handler);

	delta = _0(image->size.width);

	//rewind(file_handler);
	//fseek(file_handler, bmp_header.ptr_to_data, SEEK_CUR);

	for( i = 0; i < image->size.height; i++ ) {

		fwrite(
			&(image->matrix_ptr[ i * image->size.width ]),
			sizeof(BGRComponents),
			image->size.width,
			file_handler
			);

		fwrite(&noise, 1, delta, file_handler);

	}

	return;

}

int		image_clone(Image* dst, Image* src) {

	Size2i i = {0, 0};

	if( !image_mem_alloc(dst, src->size) ) return 0;
	/*memcpy(
		dst,
		src,
		sizeof(BGRComponents) * src->size.width * src->size.height
		);*/

	for( i.width = 0; i.width < src->size.width; i.width++ ) {

		for( i.height = 0; i.height < src->size.height; i.height++ ) {

			image_write_at(
				dst,
				image_read_at( src, i.width, i.height ),
				i.width,
				i.height
				);

		}

	}

	return 0;

}

BGRComponents image_read_at(Image* image, int x, int y) {

	long w = image->size.width;
	long h = image->size.height;

	return image->matrix_ptr[ abs(y % h) * w + abs(x % w) ];

}

void image_write_at(Image* image, BGRComponents px, int x, int y) {

	if( x >= 0 && y >= 0 && x < image->size.width && y < image->size.height ) {

		image->matrix_ptr[ y * image->size.width + x ] = px;

	}

}

void nothing() {
	// Absolutely
}