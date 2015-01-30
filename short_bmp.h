#pragma once
#include <stdio.h>
#include <stdint.h>

// ---- TYPES ---- //

typedef struct {

	uint32_t width;
	uint32_t height;

} Size2i;

#pragma pack(push, 1)
typedef struct {

	//Main part
	uint8_t		sig[2];
	uint32_t	file_size;
	uint16_t	empty1;
	uint16_t	empty2;
	uint32_t	ptr_to_data;
	//Info part
	uint32_t	header_size;
	uint32_t	width;
	uint32_t	height;
	uint16_t	color_planes;
	uint16_t	bit_per_pix;
	uint32_t	compression;
	uint32_t	pixel_array_length;
	uint32_t	x_resolution;
	uint32_t	y_resolution;
	uint32_t	colors;
	uint32_t	primary_colors;
	
} BMPHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {

	uint8_t B;
	uint8_t G;
	uint8_t R;

} BGRComponents;
#pragma pack(pop)

typedef struct {

	Size2i size;
	BGRComponents* matrix_ptr;

} Image;

// ---- FUNCTIONS ---- //

/* Image basic functions */

int		image_mem_alloc(Image*, Size2i);

void	image_mem_free(Image*);

int		image_clone(Image* /*dst*/, Image* /*src*/);

/* Working with pixels */

BGRComponents image_read_at(Image*, int, int);

void	image_write_at(Image*, BGRComponents, int, int);

/* Reading/writing image from/to BMP24 */

int		image_load(FILE*, Image*);

void	image_save(FILE*, Image*);

void	nothing();