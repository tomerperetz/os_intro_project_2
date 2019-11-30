
/*
====================================================================================================================
Description:
this lib is used for error handeling.
====================================================================================================================
*/


#pragma once
#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS
#endif

#include "hardCodedData.h"
#pragma once
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "hardCodedData.h"
//
//static const int ERROR_ID_1 = 1;
//static const int ERROR_ID_GLOBAL= -1;

#
# define STRCPY_ERROR "String copy error. please reset the program.\n"
static const char ERROR_ID_1_ARGS[170] = \
"ERROR ID 1: The given arguments are not in the correct format / number of arguments is incorrect.\n"\
"For more help please run the following command: ~<Project-Name> --help\n";
static const char ERROR_ID_2_IO[100] = \
"ERROR ID 2: IO ERROR has occured.\nProgram can't open the file\n";
static const char ERROR_ID_3_TIMEOUT[100] = \
"ERROR ID 3: Run Time Error: Process was timeout!\n" \
"Terminating brutally!\n";
static const char ERROR_ID_4_MEM_ALLOCATE[100] = \
"Allocation Error: Memory allocation failed!\n" \
"Terminating brutally!\n";
static const char ERROR_ID_5_STRING_H[100] = \
"STRLIB ERROR: Use of one of the functions from the <string.h> directory failed\n";


void raiseError(int error_id, const char *file_path, const char *function_name, int line, const char *description);

