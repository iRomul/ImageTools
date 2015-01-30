#include <wchar.h>

#include "interface/cli.h"

#include "short_bmp.h"
#include "types.h"


byte is_option(wchar_t* str) {

	if( wcslen(str) == 2 )
		if( str[0] == '-' || str[0] == '/' )
			return 1;

	return 0;

}

byte is_next_non_option(int argc, int pos, wchar_t** str) {

	if( argc-1 )
		if( !is_option(str[pos+1]) )
			return 1;

	return 0;

}

options_parse_result parse(int argc, wchar_t** argv, Settings* settings) {

	int i = 1;

	if(argc == 1)
		if(settings->options_required)
			return NO_OPTIONS;
		else
			return OK;

	while((argc--)-1) {

		if( is_option(argv[i]) ) {

			switch(argv[i][1]) {

			case L'i':

				if( is_next_non_option(argc, i, argv) ) {

						wcscpy_s(settings->input_file.arg, STR_ARG_SIZE, argv[i+1]);
						--argc;
						++i;

				} else return NO_ARGUMENT_FOR_OPTION;

				break;

			case L'o':

				if( is_next_non_option(argc, i, argv) ) {

						wcscpy_s(settings->output_file.arg, STR_ARG_SIZE, argv[i+1]);
						--argc;
						++i;

				} else return OK;

				break;

			case L'e':

				if( is_next_non_option(argc, i, argv) ) {

						if( !wcscmp(argv[i+1], L"C") )
							settings->engine_type.arg = (int)'C';

						else if( !wcscmp(argv[i+1], L"ASM") )
							settings->engine_type.arg = (int)'A';

						else if( !wcscmp(argv[i+1], L"MMX") )
							settings->engine_type.arg = (int)'M';

						else if( !wcscmp(argv[i+1], L"CSTD") )
							settings->engine_type.arg = (int)'S';

						else
							settings->engine_type.arg = (int)'C';

						--argc;
						++i;

				} else settings->engine_type.arg = (int)'C';

				break;

			case L't':

				if( is_next_non_option(argc, i, argv) ) {

					settings->test_iteration.arg = _wtoi(argv[i+1]);

					--argc;
					++i;

					if( settings->test_iteration.arg < 0)
						return INVALID_DATA;

				}

				break;

			case L'v':

				settings->verbose.arg = 1;

				break;

			default:

				return UNSUPPORTED_OPTION;

			} // switch

		} else {

			return RAW_DATA_INSTEAD_OPTION;

		}

	++i;

	} // while --argc

	return OK;
	
}

void msg_box_usage() {

	wprintf_s(
		L"Usage:\n"
		L"bmp_tool -i PATH -o PATH -e [C | CSTD | ASM | MMX] -t INT -v\n"
		L"\t-o - Path to output file.\n"
		L"\t-i - Path to input file. Default - input file.\n"
		L"\t-e - Operating engine. Default - Pure C.\n"
		L"\t-t - Operating benchmark. Default - 0. 0 means don\'t benchmark\n"
		L"\t-v - Verbose mode.\n"
		L"Example: bmp_tool -i file.bmp -o output.bmp -e ASM -v -t 500\n"
		);

}

void msg_box_information() {

	wprintf_s(
		L"Simple bmp tool v0.2\n"
		L"Author: Komarevtsev RA\n"
		L"sizeof(int): %d\n"
		L"sizeof(short int): %d\n"
		L"sizeof(bmp): %d\n",
		sizeof(int),
		sizeof(short int),
		sizeof(BMPHeader)
		);
	msg_box_usage();

}

void msg_box_error(wchar_t* err) {

	fwprintf_s(stderr, L"[ERROR]\n\t%s\n", err);
	msg_box_usage();

}