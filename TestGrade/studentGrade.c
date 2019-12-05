/*
====================================================================================================================
Description:
Caculate student grade main functions
====================================================================================================================
*/

// Includes --------------------------------------------------------------------------------------------------------
#include "studentGrade.h"
#include <math.h>


// Functions -------------------------------------------------------------------------------------------------------

void copyArr(int *src, int *dst, int n)
{
	/*
	Description: copy n elemnts from arr src to arr dst
	parameters:
			- int *src - source arr
			- int *dst - dst arr
			- int n - number of elemnts to copy
	Returns: VOID
	*/
	for (int i = 0; i < n; i++)
		dst[i] = src[i];
}

int sortGrades(int *grades_list, int elements_number)
{
	/*
	Description: Rearrange grades array in decending order
	parameters:
			- int *grades_list - unsorted grades arr
			- int elements_number - number of elemnts in the list
	Returns: TRUE when done
	*/

	int temp = -1;

	for (int i = 0; i < elements_number; ++i)
	{
		for (int j = i + 1; j < elements_number; ++j)
		{
			if (grades_list[i] < grades_list[j])
			{

				temp = grades_list[i];
				grades_list[i] = grades_list[j];
				grades_list[j] = temp;
			}
		}
	}
	return TRUE;
}

void printGradesArr(int *grades_list, int n)
{
	/*
	Description: Prints grades array screen. for debugging purpose.
	parameters:
			 - int *grades_list - grade array
			 - int n - number of elemnts
	Return: VOID
	*/

	for (int i = 0; i < n; ++i)
		printf("%d\n", grades_list[i]);
	
	return;
}

int getGradesAvg(int *sorted_grades_list, int desired_elements_number, int min_th, int *avg_ptr)
{
	/*
	Description: Calculate average HW score by using only certain number of elemnts and minimal threshold
	parameters:
			- int *sorted_grades_list - sorted list in decending order - use highest scores
			- int desired_elements_number - number of elements to use
			- int min_th - minimal threshold. if lower use 0.
			- int *avg_ptr - pointer to variable in main function 
	Returns: TRUE when done
	*/

	int sum = 0;
	int cur_grade = 0;
	for (int i = 0; i < desired_elements_number; ++i)
	{
		cur_grade = sorted_grades_list[i];
		if (cur_grade < min_th)
			cur_grade = 0;
		sum += cur_grade; 
	}

	*avg_ptr = (sum / desired_elements_number) + (sum % desired_elements_number > 0);
	return TRUE;
}

int getFinalExamGrade(int moedA, int moedB, int min_th, int *final_grade_ptr)
{
	/*
	Description: Calculate final exam score - take latest out of meod A and moed B. 
				 if grade is under minimal threshold, grade is 0.
	parameters:
			- int moedA - moed A grade
			- int moedB - moed B grade
			- int min_th - minimal threshold. if lower use 0.
			- int *final_grade_ptr - pointer to variable in main function 
	Returns: TRUE when done
	*/

	int grade = moedA;
	if (moedB > 0)
		grade = moedB;
	if (grade < min_th)
		grade = 0;

	*final_grade_ptr = grade;

	return TRUE;
}

int calcFinalGrade(int hw_grade, int mid_term_exam_grade, int final_exam_grade, int *final_course_grade_ptr)
{
	/*
	Description: Calculate final course grade according to given weights.
	parameters:
			- int hw_grade
			- int mid_term_exam_grade
			- int final_exam_grade
			- int *final_course_grade_ptr - pointer to variable in main function
	Returns: TRUE when done
	*/

	*final_course_grade_ptr = (int)ceil(HW_WEIGHT * hw_grade + MID_TERM_EXAM_WEIGHT * mid_term_exam_grade + FINAL_EXAM_WEIGHT * final_exam_grade);
	
	return TRUE;
}

int analyzeStudent(student_grades_struct *student_grades_ptr)
{
	/*
	Description: calculate final grade using the following formula:
		hw_avg = 8/10 best homework grades. round up.
		mid_term_exam = grade
		final_exam_grade = latest grade from Moed A and Moed B.
		final grade = 0.2 * hw_avg + 0.2 * mid_term_exam + 0.6 * final_exam grade.
		round up.
	parameters:
			 - student_grades_struct *student_grades_ptr - student grades struct pointer
	Return: int TRUE if done, ERR o.w
	*/

	int status = FALSE;
	int hw_grade = 0;
	int mid_exam_grade = 0;
	int final_exam_grade = 0;

	status = sortGrades((student_grades_ptr->hw_grades_arr), NUM_OF_HW);
	if (status != TRUE)
	{ 
		raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
		printf("details: Error in sorting student homework!");
		return ERR;
	}

	status = getGradesAvg((student_grades_ptr->hw_grades_arr), NUM_OF_HW_TO_USE, GRADE_TH, &hw_grade);
	if (status != TRUE)
	{
		raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
		printf("details: Error in calculating homework average!");
		return ERR;
	}

	status = getFinalExamGrade(student_grades_ptr->final_exam_grades_arr[0], student_grades_ptr->final_exam_grades_arr[1], GRADE_TH, &final_exam_grade);
	if (status != TRUE)
	{
		raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_OTHER);
		printf("details: Error in getting final exam test!");
		return ERR;
	}

	mid_exam_grade = student_grades_ptr->mid_term_grades_arr[0];
	if (mid_exam_grade < 60)
		mid_exam_grade = 0;


	status = calcFinalGrade(hw_grade, mid_exam_grade, final_exam_grade, &(student_grades_ptr->final_course_grade));
	if (status != TRUE)
	{
		printf("Error in getting final exam test!");
		return ERR;
	}
	return TRUE;
	
}

void printStudent(student_grades_struct *student_grades_ptr)
{
	/*
	Description: Prints student struct fields to screen. for debugging purpose.
	parameters:
			 - student_grades_struct *student_grades_ptr - student grades struct pointer
	Return: VOID
	*/
	printf("hw grades arr:\n");
	printGradesArr(student_grades_ptr->hw_grades_arr, NUM_OF_HW);

	printf("mid term grades arr:\n");
	printGradesArr(student_grades_ptr->mid_term_grades_arr, NUM_OF_MID_EXAMS);

	printf("final exam grades arr:\n");
	printGradesArr(student_grades_ptr->final_exam_grades_arr, NUM_OF_FINAL_EXAMS);

	printf("final course grade: %d\n", student_grades_ptr->final_course_grade);

	return;
}

void gradeLstToStruct(int grades_list[], student_grades_struct *student_grades_struct) 
{
	/*
	Description: load student grade from array to struct
	parameters:
			 - student_grades_struct *student_grades_struct - student grades struct
			 - int grades_list[] - grades array
	Return: VOID
	*/

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

void freeFileList(char **files_list)
{
	/*
	Description: free file list
	parameters:
			 - char **files_list - file list arr
	Return: VOID
	*/
	for (int idx = 0; idx < TOT_NUM_OF_FILES; idx++)
	{
		free(files_list[idx]);
	}
}

int getAllStudentGrades(char *dir_path, int grades_list[])
{
	/*
	Description: load student grade from file to array using threads.
	parameters:
			 - char *dir_path - student grades directory path
			 - int grades_list[] - empty grade list array
	Return: int TRUE if done, ERR o.w
	*/

	char file_names[TOT_NUM_OF_FILES][MAX_FILE_NAME_LEN] = \
	{"/ex01.txt", "/ex02.txt", "/ex03.txt", "/ex04.txt", "/ex05.txt", "/ex06.txt", "/ex07.txt", "/ex08.txt", "/ex09.txt", \
		"/ex10.txt", "/midterm.txt", "/moedA.txt", "/moedB.txt" };
	int idx = 0, grade = -1;
	char *files_list[TOT_NUM_OF_FILES];

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
		freeFileList(files_list);
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		return ERR;
	}

	/* free memory */
	freeFileList(files_list);

	return TRUE;
}

int gradeManager(char *dir_path)
{
	/*
	Description: main function: takes directory path, load grades, calc final score and prints to file.
	parameters:
			 - char *dir_path - student grades directory path
	Return: int TRUE if done, ERR o.w
	*/

	int grades_list[TOT_NUM_OF_FILES];
	student_grades_struct student_grades_struct;

	// Load student grade to arr
	if (getAllStudentGrades(dir_path, grades_list) != TRUE)
		return ERR;

	// Load grade arr to struct
	gradeLstToStruct(grades_list, &student_grades_struct);
	
	// Calculate student final grade
	if (analyzeStudent(&student_grades_struct) != TRUE)
	{
		return ERR;
	}
	
	// Print grade to file
	printToFile(dir_path, student_grades_struct.final_course_grade);
	
	return TRUE;
}
