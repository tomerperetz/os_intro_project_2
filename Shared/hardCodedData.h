/*
====================================================================================================================
Description:
Hard coded data: includes and defines
====================================================================================================================
*/


#pragma once
#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>	
#include <string.h>
#include <windows.h>

//Const That Relevant For All Projects
static const int ERR = -1;
static const int ERR_2 = -2;
static const int ERR_3 = -3;
static const int IO_ERR = -4;
static const int EQUAL = 0;
static const int IS_TRUE = 1;
static const int IS_FALSE = 0;
static const char END_OF_STR = '\0';

//defines for project 2
#define GRADE_TH 60
#define NUM_OF_HW 10
#define NUM_OF_HW_TO_USE 8
#define NUM_OF_MID_EXAMS 1
#define NUM_OF_FINAL_EXAMS 2
#define TIMEOUT_IN_MILLISECONDS 10000
#define BRUTAL_TERMINATION_CODE -10
#define HW_WEIGHT 0.2
#define MID_TERM_EXAM_WEIGHT 0.2
#define FINAL_EXAM_WEIGHT 0.6
#define MAX_FILE_NAME_LEN 12
#define MAX_FILES 13
#define MAX_NUM_OF_STUDENTS 10
#define ID_LEN_STR 12
#define STUDENT_DIR_NAME_LEN 20
#define GRADE_LEN 5
