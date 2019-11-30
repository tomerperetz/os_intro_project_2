#include "../Shared/hardCodedData.h"
#include "../Shared/argparser.h"
#include "../Shared/lib_errorHandler.h"
#include "../Shared/lib_osHandler.h"


void printLst(char lst[][STUDENT_DIR_NAME_LEN], int lst_size)
{
	int i = 0;
	for (i = 0; i < lst_size; i++) {
		printf("%s\n", lst[i]);
	}
		
}
int strcatDynamic(const char first_source[], const char second_source[], char **buffer)
{
	int retVal1 = ERR, retVal2 = ERR, size_of_file_path = 0;
	
	size_of_file_path = (strlen(first_source) + strlen(second_source)+1);
	*buffer = (char*) malloc(sizeof(char)*size_of_file_path);
	if (*buffer == NULL) {
		raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_CONTENT);
		return IS_FALSE;
	}
	retVal1 = strcpy_s(*buffer, sizeof(char)*size_of_file_path, first_source);
	retVal2 = strcat_s(*buffer, sizeof(char)*size_of_file_path, second_source);
	if (retVal1 != 0 || retVal2 != 0) {
		raiseError(9, __FILE__, __func__, __LINE__, ERROR_ID_9_CONTENT);
		free(*buffer);
		return ERR;
	}
	return IS_TRUE;
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
int parseStudentDirLst(char student_dirs[][STUDENT_DIR_NAME_LEN], int *dir_list_len, char file_path[])
{
	FILE *student_id_fp = NULL;
	int ret_val1 = ERR, ret_val2 = ERR;
	char id[ID_LEN_STR], grades_prefix[9] = "/grades_";
	/*TO DO: change to Tomer's open safe func*/
	if (fopen_s(&student_id_fp, file_path, "r") != IS_FALSE || student_id_fp == NULL) {
		return ERR;
	}
	while (!feof(student_id_fp) && *dir_list_len <= MAX_NUM_OF_STUDENTS) {
		ret_val1 = strcpy_s(student_dirs[*dir_list_len], STUDENT_DIR_NAME_LEN, grades_prefix);
		if (ret_val1 != 0) {
			raiseError(9, __FILE__, __func__, __LINE__, ERROR_ID_9_CONTENT);
			break;
		}
		// reading a line that contains student ID
		fgets(id, STUDENT_DIR_NAME_LEN, student_id_fp);
		// check whether it is a valid id num
		ret_val1 = removeEnterCharFromString(id);
		ret_val2 = isStudentIdValid(id);
		// increases the students list by 1 if it is indeed a valid ID
		if (ret_val1 == IS_TRUE && ret_val2 == IS_TRUE) {
			strcat_s(student_dirs[*dir_list_len], STUDENT_DIR_NAME_LEN, id);
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
	return IS_TRUE;
}

void str_safe_free(char *ptr) {
	if (ptr != NULL)
		free(ptr);
}

int manager(char *dir_path) {
	int dir_list_len = 0, idx = 0, return_value1 = ERR, return_value2 = ERR, return_value = ERR;
	char *file_path = NULL, students_grades_dir_lst[MAX_NUM_OF_STUDENTS][STUDENT_DIR_NAME_LEN], *student_grade_path = NULL;
	char *cmd = NULL, test_grades_str[20] = "TestGrade.exe ";
	const char student_ids_file_name[20] = "/studentIds.txt";

	
	
	if (strcatDynamic(dir_path, student_ids_file_name, &file_path) == IS_FALSE) {
		return ERR;
	}
	
	if (parseStudentDirLst(students_grades_dir_lst, &dir_list_len, file_path) != IS_TRUE) {
		free(file_path);
		raiseError(4, __FILE__, __func__, __LINE__, ERROR_ID_4_CONTENT);
		return ERR;
	}
	free(file_path);

	for (idx = 0; idx < dir_list_len; idx++) {
		return_value1 = strcatDynamic(dir_path, students_grades_dir_lst[idx], &student_grade_path);
		return_value2 = strcatDynamic(test_grades_str, student_grade_path, &cmd);
		if (return_value1 != IS_TRUE || return_value1 != IS_TRUE) {
			break;
		}
		return_value = CreateProcessSimpleMain(cmd);
		if (return_value != IS_TRUE) {
			break;
		}

		str_safe_free(student_grade_path);
		str_safe_free(cmd);
		student_grade_path = NULL;
		cmd = NULL;
	}
	str_safe_free(student_grade_path);
	str_safe_free(cmd);
	return return_value;
}

int main(int argc, char *argv[])
{
	if (ensureArgs(argc, EXPECTED_ARGC, argv) != IS_TRUE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_CONTENT);
		return IS_TRUE;
	}
	if (manager(argv[1]) != IS_TRUE)
		return IS_TRUE;
	
	printf("The grades have arrived, captain");
	return IS_FALSE;
}