#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "short_bmp.h"
#include "image_filters.h"

void msg_cli_usage();

double gauss_distribution(double, double);

int wmain(int argc, wchar_t* argv[]) {

	/***		DATA		***/

	Image image;
	Image thr_image;

	wchar_t input_file_path[256];
	wchar_t output_file_path[256];

	FILE* file_handler;

	double radius = 1.0;

	/***		CODE		***/

	/** Checking program options **/

	if( argc == 4 ) {

		radius = _wtof(argv[1]);
		wcscpy_s(input_file_path, 256, argv[2]);
		wcscpy_s(output_file_path, 256, argv[3]);

	} else {

		msg_cli_usage();
		exit(EXIT_FAILURE);

	}

	/** Reading BMP file **/
	/* Trying to open input file */
	
	_wfopen_s(&file_handler, input_file_path, L"rb");
	if( !file_handler ) {
		fwprintf_s(stderr, L"Can\'t open file!\n");
		exit(EXIT_FAILURE);
	}

	/* Loading BMP file */

	if( !image_load(file_handler, &image) ) {
		
		fwprintf_s(stderr, L"Can't open file, sorry\n");
		exit(EXIT_FAILURE);
		
	};

	/* Closing file */

	fclose(file_handler);

	/** Processing **/

	image_clone(&thr_image, &image);
	filter_gauss_blur(&thr_image, radius);

	/** Saving result **/

	_wfopen_s(&file_handler, output_file_path, L"wb");
	if( !file_handler ) {
		fwprintf_s(stderr, L"Can\'t open file!\n");
		exit(EXIT_FAILURE);
	}

	image_save(file_handler, &thr_image);

	fclose(file_handler);

	image_mem_free(&image);
	image_mem_free(&thr_image);

	//system("pause");
	
	return EXIT_SUCCESS;

}

void msg_cli_usage() {

	wprintf_s(
		L"Usage\n"
		L"bmp_tools radius input_file output_file\n"
		L"radius - blur function radius\n"
		L"input_file - path to input file\n"
		L"output_file - path to output file\n"
		);

}