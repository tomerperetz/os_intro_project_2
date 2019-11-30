#include "../Shared/hardCodedData.h"
#include "../Shared/argparser.h"
#include "../Shared/lib_errorHandler.h"
#include "../Shared/lib_osHandler.h"
#include "../Shared/lib_str_func.h"
#include "../Shared/lib_fileHandler.h"

void printLst(char lst[][STUDENT_DIR_NAME_LEN], int lst_size)
{
	int i = 0;
	for (i = 0; i < lst_size; i++) {
		printf("%s\n", lst[i]);
	}
		
}

int isStudentIdValid(char *id_str)
{
	int idx = 0;
	while (id_str[idx] != END_OF_STR && idx <= ID_LEN_STR - 3)
	{
		if (id_str[idx] < 48 || id_str[idx] > 57)
			return IS_FALSE;
		idx++;
	}
	if (idx < ID_LEN_STR - 3) 
		return IS_FALSE;
	return IS_TRUE;
}

int removeEnterCharFromString(char *str)
{
	int idx = 0;
	while (str[idx] != END_OF_STR)
	{
		if (str[idx] == '\n') {
			str[idx] = END_OF_STR;
			if (idx < ID_LEN_STR - 3) {
				return IS_FALSE;
			}
			idx--;
		}
		idx++;
	}
	return IS_TRUE;
}
int parseStudentDirLst(char student_dirs[][STUDENT_DIR_NAME_LEN], char final_lst[][STUDENT_DIR_NAME_LEN], int *dir_list_len, char file_path[])
{
	FILE *student_id_fp = NULL;
	int ret_val1 = ERR, ret_val2 = ERR, ret_val = IS_TRUE;
	char id[ID_LEN_STR], grades_prefix[9] = "/grades_", final_prefix[9] = "/final_";
	/*TO DO: change to Tomer's open safe func*/
	if (fopen_s(&student_id_fp, file_path, "r") != IS_FALSE || student_id_fp == NULL) {
		return ERR;
	}
	while (!feof(student_id_fp) && *dir_list_len <= MAX_NUM_OF_STUDENTS) {
		ret_val1 = strcpy_s(student_dirs[*dir_list_len], STUDENT_DIR_NAME_LEN, grades_prefix);
		ret_val2 = strcpy_s(final_lst[*dir_list_len], STUDENT_DIR_NAME_LEN, final_prefix);
		if (ret_val1 != 0 || ret_val2 != 0) {
			raiseError(9, __FILE__, __func__, __LINE__, ERROR_ID_5_STRING_H);
			ret_val = ERR;
			break;
		}
		// reading a line that contains student ID
		fgets(id, STUDENT_DIR_NAME_LEN, student_id_fp);
		// check whether it is a valid id num
		ret_val1 = removeEnterCharFromString(id);
		ret_val2 = isStudentIdValid(id);
		// increases the students list by 1 if it is indeed a valid ID
		if (ret_val1 == IS_TRUE && ret_val2 == IS_TRUE) {
			ret_val1 = strcat_s(student_dirs[*dir_list_len], STUDENT_DIR_NAME_LEN, id);
			ret_val2 = strcat_s(final_lst[*dir_list_len], STUDENT_DIR_NAME_LEN, id);
			if (ret_val1 != 0 || ret_val2 != 0) {
				raiseError(9, __FILE__, __func__, __LINE__, ERROR_ID_5_STRING_H);
				ret_val = ERR;
				break;
			}
			*dir_list_len += 1;
		}
		else {
			strcpy_s(student_dirs[*dir_list_len], STUDENT_DIR_NAME_LEN, "0");
			break;
		}
	}
	// close the file and check whether its done successfully
	if (fclose(student_id_fp) != IS_FALSE)
		return ERR;
	return ret_val;
}

int manager(char *dir_path) {
	int dir_list_len = 0, idx = 0, return_value1 = ERR, return_value2 = ERR, return_value = IS_TRUE;
	char *file_path = NULL, students_grades_dir_lst[MAX_NUM_OF_STUDENTS][STUDENT_DIR_NAME_LEN], *student_grade_path = NULL;
	char *cmd = NULL, test_grades_str[20] = "TestGrade.exe ", final_lst[MAX_NUM_OF_STUDENTS][STUDENT_DIR_NAME_LEN],
		*final_grade_student_path_no_sufix = NULL, *final_grade_student_path = NULL, sufix[5] = ".txt";
	const char student_ids_file_name[20] = "/studentIds.txt";

	
	
	if (strcatDynamic(dir_path, student_ids_file_name, &file_path) == IS_FALSE) {
		return ERR;
	}
	
	if (parseStudentDirLst(students_grades_dir_lst, final_lst, &dir_list_len, file_path) != IS_TRUE) {
		free(file_path);
		raiseError(4, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		return ERR;
	}
	free(file_path);
	
	for (idx = 0; idx < dir_list_len; idx++) {
		return_value1 = strcatDynamic(dir_path, students_grades_dir_lst[idx], &student_grade_path);
		return_value2 = strcatDynamic(test_grades_str, student_grade_path, &cmd);
		if (return_value1 != IS_TRUE || return_value1 != IS_TRUE) {
			return_value = ERR;
			break;
		}
		return_value1 = CreateProcessSimpleMain(cmd);
		if (return_value1 == ERR) {
			return_value = ERR;
			break;
		}

		str_safe_free(cmd);
		cmd = NULL;

		return_value1 = strcatDynamic(student_grade_path, final_lst[idx], &final_grade_student_path_no_sufix);
		return_value2 = strcatDynamic(final_grade_student_path_no_sufix, sufix, &final_grade_student_path);
		if (return_value1 != IS_TRUE || return_value1 != IS_TRUE) {
			return_value = ERR;
			break;
		}
		str_safe_free(student_grade_path);
		student_grade_path = NULL;


		printf("%s\n", final_grade_student_path);
		str_safe_free(final_grade_student_path_no_sufix);
		str_safe_free(final_grade_student_path);
		final_grade_student_path_no_sufix = NULL;
		final_grade_student_path = NULL;
	}
	str_safe_free(student_grade_path);
	str_safe_free(cmd);
	return return_value;
}

int main(int argc, char *argv[])
{
	if (ensureArgs(argc, EXPECTED_ARGC, argv) != IS_TRUE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_ARGS);
		return IS_TRUE;
	}
	if (manager(argv[1]) != IS_TRUE)
		return IS_TRUE;
	
	printf("The grades have arrived, captain");
	return IS_FALSE;
}