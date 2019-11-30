#include "studentGrade.h"



void copy_arr(int *src, int *dst, int n)
{
	for (int i = 0; i < n; i++)
		dst[i] = src[i];
	return TRUE;
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

	*final_course_grade_ptr = HW_WEIGHT * hw_grade + MID_TERM_EXAM_WEIGHT * mid_term_exam_grade + FINAL_EXAM_WEIGHT * final_exam_grade;
	
	return TRUE;
}

int analyzeStudent(student_grades_struct *student_grades_ptr)
{
	int status = FALSE;
	int hw_grade = 0;
	int mid_exam_grade = 0;
	int final_exam_grade = 0;

	status = sortGrades(&(student_grades_ptr->hw_grades_arr), NUM_OF_HW);
	if (status != TRUE)
	{ 
		printf("Error in sorting student homework!");
		return ERR;
	}

	status = getGradesAvg(&(student_grades_ptr->hw_grades_arr), NUM_OF_HW_TO_USE, GRADE_TH, &hw_grade);
	printf("hw_avg: %d \n", hw_grade);
	if (status != TRUE)
	{
		printf("Error in calculating homework average!");
		return ERR;
	}

	status = getFinalExamGrade(student_grades_ptr->final_exam_grades_arr[0], student_grades_ptr->final_exam_grades_arr[1], GRADE_TH, &final_exam_grade);
	printf("final exam grade: %d \n", final_exam_grade);
	if (status != TRUE)
	{
		printf("Error in getting final exam test!");
		return ERR;
	}

	mid_exam_grade = student_grades_ptr->mid_term_grades_arr[0];
	if (mid_exam_grade < 60)
		mid_exam_grade = 0;

	printf("mid exam grade: %d \n", mid_exam_grade);

	status = calcFinalGrade(hw_grade, mid_exam_grade, final_exam_grade, &(student_grades_ptr->final_course_grade));
	if (status != TRUE)
	{
		printf("Error in getting final exam test!");
		return ERR;
	}
	

}

void printStudent(student_grades_struct *student_grades_ptr)
{

	printf("hw grades arr:\n");
	printGradesArr(&student_grades_ptr->hw_grades_arr, NUM_OF_HW);

	printf("mid term grades arr:\n");
	printGradesArr(&student_grades_ptr->mid_term_grades_arr, NUM_OF_MID_EXAMS);

	printf("final exam grades arr:\n");
	printGradesArr(&student_grades_ptr->final_exam_grades_arr, NUM_OF_FINAL_EXAMS);

	printf("final course grade: %d\n", student_grades_ptr->final_course_grade);

	return;
}


char** initGradesList(char **user_path)
{
	char **files;
	int path_len = 0;
	int max_len = 0;

	// Allocate memory for files array
	files = (char**)malloc(MAX_FILES * sizeof *files);
	if (files == NULL)
	{
		printf("Memory allocation faild!\n");
		return NULL;
	}

	// Allocate memory for each file path
	path_len = strlen(user_path) + 1;
	for (int i = 0; i < MAX_FILES; i++)
	{
		max_len = MAX_FILE_NAME_LEN + path_len + 1;
		files[i] = (char*)malloc((max_len) * sizeof(char));
		if (files[i] == NULL)
		{
			printf("Memory allocation faild!\n");
			return NULL;
		}
		strcpy_s(files[i], path_len, user_path);
	}

	// init path list
	strcat_s(files[0], max_len, "ex01.txt");
	strcat_s(files[1], max_len, "ex02.txt");
	strcat_s(files[2], max_len, "ex03.txt");
	strcat_s(files[3], max_len, "ex04.txt");
	strcat_s(files[4], max_len, "ex05.txt");
	strcat_s(files[5], max_len, "ex06.txt");
	strcat_s(files[6], max_len, "ex07.txt");
	strcat_s(files[7], max_len, "ex08.txt");
	strcat_s(files[8], max_len, "ex09.txt");
	strcat_s(files[9], max_len, "ex10.txt");
	strcat_s(files[10], max_len, "midterm.txt");
	strcat_s(files[11], max_len, "moedA.txt");
	strcat_s(files[12], max_len, "moedB.txt");

	return files;
}

