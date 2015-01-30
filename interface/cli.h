#include <stdio.h>

// --- CONSTS ---- //

#define STR_ARG_SIZE 50

// ---- TYPES ---- //

typedef struct {

	wchar_t key;
	short arg_required;
	short key_required;

} opt_key;

typedef struct {

	opt_key key;
	wchar_t arg[50];

} wstr_option;

typedef struct {

	opt_key key;
	long arg;	

} int_option;

typedef struct {

	opt_key key;
	short arg;

} void_option;

typedef struct {
	
	short options_required;
	wstr_option input_file;
	wstr_option output_file;
	int_option engine_type;
	int_option test_iteration;
	void_option verbose;
	
} Settings;

typedef enum {

	NO_OPTIONS,
	NO_ARGUMENT_FOR_OPTION,
	NO_REQUIRED_OPTION,
	RAW_DATA_INSTEAD_OPTION,
	UNSUPPORTED_OPTION,
	INVALID_DATA,
	OK

} options_parse_result;

// ---- FUNCTIONS ---- //

options_parse_result parse(int, wchar_t**, Settings*);

uint8_t is_option(wchar_t*);

uint8_t is_next_non_option(int, int, wchar_t**);

void msg_box_usage();

void msg_box_information();

void msg_box_error(wchar_t* err);