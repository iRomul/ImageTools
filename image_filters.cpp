#include "image_filters.h"
#include "short_bmp.h"
#include <malloc.h>
#define _USE_MATH_DEFINES
#include <math.h>

double gauss_distribution(int i, double sigma) {

	if( i == 0 ) return 1.0;
	else
	if( i <  0 ) return gauss_distribution(-i, sigma);
	else
	return pow(	M_E,  -(i*i) / (2*sigma*sigma)  );

}

void filter_binary_threshold(Image* image, uint8_t threshold) {

	Size2i i = {0, 0};
	BGRComponents px;

	for( i.width = 0; i.width < image->size.width; i.width++ ) {

		for( i.height = 0; i.height < image->size.height; i.height++ ) {

			px = image_read_at( image, i.width, i.height );
			if( px.B < threshold ) px.B = 0; else px.B = 255;
			if( px.G < threshold ) px.G = 0; else px.G = 255;
			if( px.R < threshold ) px.R = 0; else px.R = 255;

			image_write_at( image, px, i.width, i.height );

		}

	}

}

void filter_gauss_blur(Image* image, double sigma) {

	long i = 0, j = 0, k = 0;
	//enum {HORIZONTAL = 0, VERTICAL = 1} v;
	int v;
	double sum = 0.0;
	double r, g, b;

	BGRComponents read_px;
	BGRComponents write_px;
	Image tmp;	

	long n = (long)ceil(sigma * 3.0); /* Three sigma rule */
	long k_size = (int)(2 * n + 1);
	double* kernel;

	if( sigma == 0.0 ) return;

	kernel = (double*)calloc(k_size, sizeof(double)); /* kernel = [-n..n] */

	image_clone(&tmp, image);

	for( i = -n; i <= n; i++ ) {

		kernel[i + n] = gauss_distribution(i, sigma);

		sum += kernel[i + n];

	}

	for( v = 0; v <= 1; v++ ) {

		for( i = 0; i < (long)image->size.width; i++ ) {

			for( j = 0; j < (long)image->size.height; j++ ) {

				b = g = r = 0.0;				

				for( k = 0; k < k_size; k++ ) {

					switch(v) {
					case 0:	read_px = image_read_at(&tmp, i + k - n, j); break;
					case 1:	read_px = image_read_at(&tmp, i, j + k - n); break;
					}

					b += (double)read_px.B * kernel[k];
					g += (double)read_px.G * kernel[k];
					r += (double)read_px.R * kernel[k];

				}

				write_px.B = (uint8_t)(b / sum);
				write_px.G = (uint8_t)(g / sum);
				write_px.R = (uint8_t)(r / sum);

				image_write_at(image, write_px, i, j);

			}		

		}

		image_mem_free(&tmp);
		image_clone(&tmp, image);

	}

	free(kernel);

	image_mem_free(&tmp);

}

void convert_BGR_to_grayscale(Image* image) {

	Size2i i = {0, 0};
	BGRComponents px;
	uint8_t gray_px;

	for( i.width = 0; i.width < image->size.width; i.width++ ) {

		for( i.height = 0; i.height < image->size.height; i.height++ ) {

			px = image_read_at( image, i.width, i.height );

			gray_px = (uint8_t)(px.R*0.2126 + px.G*0.7152 + px.B*0.0722);
			px.B = px.G = px.R = gray_px;

			image_write_at( image, px, i.width, i.height );

		}

	}

}