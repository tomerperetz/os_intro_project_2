/*
====================================================================================================================
Authors:
	- Segev Elmalem, ID: 203149000
	- Tomer Peretz, ID: 305002206
Project: TestManager
Input: students grades directory path
Outputs: <user_path>/final_grades.txt
Description:
Calculate grades for each student and prints summary to output file.
====================================================================================================================
*/

// Includes --------------------------------------------------------------------

#include "../Shared/hardCodedData.h"
#include "../Shared/argparser.h"
#include "../Shared/lib_errorHandler.h"
#include "../Shared/lib_osHandler.h"
#include "../Shared/lib_str_func.h"
#include "../Shared/lib_fileHandler.h"
#include "studentManager.h"

// Functions --------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Checks whether the given arguments are valid  
	if (ensureArgs(argc, EXPECTED_ARGC, argv) != TRUE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
		return TRUE;
	}
	// call the maneger program
	if (manager(argv[1]) != TRUE)
		return TRUE;
	// Prints the relevant exit message if the program has been successful

	printf("The grades have arrived, captain\n");
	return FALSE;
}