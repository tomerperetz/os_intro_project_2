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
void printLst(int lst[], int lst_size)
{
	int i = 0;
	for (i = 0; i < lst_size; i++) {
		printf("%d\n", lst[i]);
	}

}

void gradeLstToStruct(int grades_list[], student_grades_struct *student_grades_struct) {
	int lst_idx = 0, hw_idx = 0, mid_exam_idx = 0, final_exam_idx = 0;
	for (lst_idx = 0; lst_idx < TOT_NUM_OF_FILES; lst_idx++) {
		if (lst_idx < NUM_OF_HW) {
			student_grades_struct->hw_grades_arr[hw_idx] = grades_list[lst_idx];
			hw_idx++;
		}
		else if (lst_idx == MID_EXAM_IDX)
			student_grades_struct->mid_term_grades_arr[mid_exam_idx] = grades_list[lst_idx];
		else {
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
		"/ex10.txt", "/midterm.txt", "/moedA.txt", "/moedB.txt" }, *file_path = NULL;
	int idx = 0, grade = -1;

	for (idx = 0; idx < TOT_NUM_OF_FILES; idx++) {
		if (strcatDynamic(dir_path, file_names[idx], &file_path) == FALSE) {
			return ERR;
		}
		//=================================================================
		//************ TO DO: USE Thread!!!!! *********************************
		//=================================================================
		if (readGradeFile(file_path, &grade) != TRUE) {
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			str_safe_free(file_path);
			return ERR;
		}
		grades_list[idx] = grade;
		str_safe_free(file_path);
		file_path = NULL;
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












//
//
//
//int tomer(int argc, char *argv[])
//{
//	char **files;
//	int hw_grades_list[NUM_OF_HW];
//	int mid_grades_list[NUM_OF_MID_EXAMS];
//	int final_grades_list[NUM_OF_FINAL_EXAMS];
//	student_grades_struct student_grades_struct;
//
//	// args parser
//	if (ensureArgs(argc, 2, argv) == IS_FALSE) {
//		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
//		return ERR;
//	}
//	// init files list
//	files = initGradesList(argv[1]);
//	if (files == NULL)
//	{
//		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
//		printf("File Path: %s", argv[1]);
//		return ERR;
//	}
//	printLst(files, 2);
//	//------------------------------------------------------//
//	//		REPLACE WITH THREAD                             //
//	//------------------------------------------------------//
//	// init hw grades
//	for (int i = 0; i < NUM_OF_HW; i++)
//	{
//		if (readGradeFile(files[i], &hw_grades_list[i]) != TRUE)
//		{
//			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
//			freeFilesList(files);
//			return NULL;
//		}
//	}
//
//	// init mid term exam grade
//	for (int i = NUM_OF_HW; i < NUM_OF_MID_EXAMS+ NUM_OF_HW; i++)
//	{
//		if (readGradeFile(files[i], &mid_grades_list[i- NUM_OF_HW])!=TRUE)
//		{
//			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
//			freeFilesList(files);
//			return NULL;
//		}
//	}
//
//	// init final exam grade
//	for (int i = NUM_OF_MID_EXAMS + NUM_OF_HW; i < NUM_OF_MID_EXAMS + NUM_OF_HW + NUM_OF_FINAL_EXAMS; i++)
//	{
//		if (readGradeFile(files[i], &final_grades_list[i - NUM_OF_HW - NUM_OF_MID_EXAMS])!= TRUE)
//		{
//			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
//			freeFilesList(files);
//			return NULL;
//		}
//	}
//	//------------------------------------------------------//
//	//		END REPLACE WITH THREAD                         //
//	//------------------------------------------------------//
//	initStudentStruct(hw_grades_list, mid_grades_list, final_grades_list, &student_grades_struct);
//
//	analyzeStudent(&student_grades_struct);
//
//	printToFile(argv[1], student_grades_struct.final_course_grade);
//
//	freeFilesList(files);
//
//	return 0;
//}