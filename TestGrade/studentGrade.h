#pragma once
#include "../Shared/hardCodedData.h"

typedef struct student_grades_s
{
	int hw_grades_arr[NUM_OF_HW];
	int mid_term_grades_arr[NUM_OF_MID_EXAMS];
	int final_exam_grades_arr[NUM_OF_FINAL_EXAMS];
	int final_course_grade;
} student_grades_struct;

void copy_arr(int *src, int *dst, int n);
int sortGrades(int *scores_list, int elements_number);
void printGradesArr(int *grades_list, int n);
int getGradesAvg(int *sorted_grades_list, int desired_elements_number, int min_th, int *avg_ptr);
int getFinalExamGrade(int moedA, int moedB, int min_th, int *final_grade_ptr);
int calcFinalGrade(int hw_grade, int mid_term_exam_grade, int final_exam_grade, int *final_course_grade_ptr);
int analyzeStudent(student_grades_struct *student_grades_ptr);
void printStudent(student_grades_struct *student_grades_ptr);
char** initGradesList(char *user_path);
void initStudentStruct(int *hw_grades_list, int *mid_grades_list, int *final_grades_list, student_grades_struct *student);