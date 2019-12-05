/*
====================================================================================================================
Authors:
	- Segev Elmalem, ID: 203149000
	- Tomer Peretz, ID: 305002206
Project: TestGrade
Input: student grades directory path
Outputs: prints final grade to file: <student_dir>\final_<ID>.txt
Description:
Calculates the final grade for a student from sepreate grade files.
takes 8/10 best homework, mid exam and latest out of 2 final exam grades.
final grade  = 0.2*hw_avg + 0.2*mid_exam +0.6*final exam
====================================================================================================================
*/


// Includes --------------------------------------------------------------------
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include "../Shared/hardCodedData.h"
#include "../Shared/lib_fileHandler.h"
#include "studentGrade.h"
#include "../Shared/lib_errorHandler.h"
#include "../Shared/argparser.h"
#include "../Shared/lib_errorHandler.h"
#include "studentGrade.h"

// Functions -------------------------------------------------------------------

int main(int argc, char *argv[])
{	
	/*
	Description: recieve directory path as an arg. verify number of args and sends it to gradeManager
	convert numbers to integers and calculate the result.
	parameters:
			 - char* argv - user args. should include student directory path.
			 - int argc = number of parameters given by user.
	Return: int.
	*/

	// args parser
	if (ensureArgs(argc, 2, argv) == IS_FALSE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
		return ERR;
	}
	if (gradeManager(argv[1]) != TRUE)
		return ERR;

	return 0;
}

