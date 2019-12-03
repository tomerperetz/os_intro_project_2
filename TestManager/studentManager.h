#pragma once
#include "../Shared/hardCodedData.h"
#include "../Shared/lib_errorHandler.h"
#include "../Shared/lib_fileHandler.h"
#include "../Shared/lib_osHandler.h"
#include "../Shared/lib_str_func.h"


int manager(char *dir_path); 
int isStudentIdValid(char *id_str);
int removeEnterCharFromString(char *str);
int parseStudentIDs(char student_grades_dir_names[][STUDENT_DIR_NAME_LEN], char final_grades_path_lst[][STUDENT_DIR_NAME_LEN],
	char ids_lst[][ID_LEN_STR], int *dir_list_len, char id_lst_file_path[]);
int printFinalGradeToFile(char *file_path, char *id, int grade, int *start_flag);