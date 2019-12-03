// Includes --------------------------------------------------------------------
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include "../Shared/hardCodedData.h"
#include "../Shared/lib_fileHandler.h"
#include "studentGrade.h"
#include "../Shared/lib_errorHandler.h"
#include "../Shared/argparser.h"

// Constants -------------------------------------------------------------------
void printLst(char lst[][STUDENT_DIR_NAME_LEN], int lst_size)
{
	int i = 0;
	for (i = 0; i < lst_size; i++) {
		printf("%s\n", lst[i]);
	}

}

int main(int argc, char *argv[])
{
	char **files;
	int hw_grades_list[NUM_OF_HW];
	int mid_grades_list[NUM_OF_MID_EXAMS];
	int final_grades_list[NUM_OF_FINAL_EXAMS];
	student_grades_struct student_grades_struct;

	// args parser
	if (ensureArgs(argc, 2, argv) == IS_FALSE)

		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);

	// init files list
	files = initGradesList(argv[1]);
	if (files == NULL)
	{
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		printf("File Path: %s", argv[1]);
		return ERR;
	}
	printLst(files, 2);
	//------------------------------------------------------//
	//		REPLACE WITH THREAD                             //
	//------------------------------------------------------//
	// init hw grades
	for (int i = 0; i < NUM_OF_HW; i++)
	{
		if (readGradeFile(files[i], &hw_grades_list[i]) != TRUE)
		{
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			freeFilesList(files);
			return NULL;
		}
	}

	// init mid term exam grade
	for (int i = NUM_OF_HW; i < NUM_OF_MID_EXAMS+ NUM_OF_HW; i++)
	{
		if (readGradeFile(files[i], &mid_grades_list[i- NUM_OF_HW])!=TRUE)
		{
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			freeFilesList(files);
			return NULL;
		}
	}

	// init final exam grade
	for (int i = NUM_OF_MID_EXAMS + NUM_OF_HW; i < NUM_OF_MID_EXAMS + NUM_OF_HW + NUM_OF_FINAL_EXAMS; i++)
	{
		if (readGradeFile(files[i], &final_grades_list[i - NUM_OF_HW - NUM_OF_MID_EXAMS])!= TRUE)
		{
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			freeFilesList(files);
			return NULL;
		}
	}
	//------------------------------------------------------//
	//		END REPLACE WITH THREAD                         //
	//------------------------------------------------------//
	initStudentStruct(hw_grades_list, mid_grades_list, final_grades_list, &student_grades_struct);

	analyzeStudent(&student_grades_struct);

	printToFile(argv[1], student_grades_struct.final_course_grade);

	freeFilesList(files);

	return 0;
}