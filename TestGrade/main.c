// Includes --------------------------------------------------------------------
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include "../Shared/hardCodedData.h"
#include "../Shared/lib_fileHandler.h"
#include "studentGrade.h"
#include "../Shared/lib_errorHandler.h"
#include "../Shared/argparser.h"
#include "../Shared/lib_osHandler.h"

// Functions -------------------------------------------------------------------
void gradeLstToStruct(int grades_list[], student_grades_struct *student_grades_struct) {
	int lst_idx = 0, hw_idx = 0, mid_exam_idx = 0, final_exam_idx = 0;
	for (lst_idx = 0; lst_idx < TOT_NUM_OF_FILES; lst_idx++) 
	{
		if (lst_idx < NUM_OF_HW) 
		{
			student_grades_struct->hw_grades_arr[hw_idx] = grades_list[lst_idx];
			hw_idx++;
		}
		else if (lst_idx == MID_EXAM_IDX)
			student_grades_struct->mid_term_grades_arr[mid_exam_idx] = grades_list[lst_idx];
		else 
		{
			student_grades_struct->final_exam_grades_arr[final_exam_idx] = grades_list[lst_idx];
			final_exam_idx++;
		}
		
	}
	student_grades_struct->final_course_grade = 0;
}

int getAllStudentGrades(char *dir_path, int grades_list[])
{
	char file_names[TOT_NUM_OF_FILES][MAX_FILE_NAME_LEN] = \
	{"/ex01.txt", "/ex02.txt", "/ex03.txt", "/ex04.txt", "/ex05.txt", "/ex06.txt", "/ex07.txt", "/ex08.txt", "/ex09.txt", \
		"/ex10.txt", "/midterm.txt", "/moedA.txt", "/moedB.txt" };
	int idx = 0, grade = -1;
	char **files_list[TOT_NUM_OF_FILES];

	/* Create file list array */
	for (idx = 0; idx < TOT_NUM_OF_FILES; idx++) 
	{
		if (strcatDynamic(dir_path, file_names[idx], &files_list[idx]) == FALSE)
		{
			return ERR;
		}
	}

	/* Load grades to grades array using threads */
	if (mainCreateReadGradesThreadSimple(files_list, grades_list) != STUDENT_GRADE_TREAD__CODE_SUCCESS)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		return ERR;
	}

	/* free memory */
	for (idx = 0; idx < TOT_NUM_OF_FILES; idx++)
	{
		free(files_list[idx]);
	}

	return TRUE;
}

int gradeManager(char *dir_path) 
{
	int grades_list[TOT_NUM_OF_FILES];
	student_grades_struct student_grades_struct;

	if (getAllStudentGrades(dir_path, grades_list) != TRUE) {
		return ERR;
	}
	gradeLstToStruct(grades_list, &student_grades_struct);
	analyzeStudent(&student_grades_struct);
	printToFile(dir_path, student_grades_struct.final_course_grade);
	return TRUE;
}

int main(int argc, char *argv[])
{
	// args parser
	if (ensureArgs(argc, 2, argv) == IS_FALSE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
		return ERR;
	}
	if (gradeManager(argv[1]) != TRUE)
		return ERR;

	return 0;
}

