#include "studentManager.h"

int manager(char *dir_path) {
	int dir_list_len = 0, idx = 0, return_value1 = ERR, return_value2 = ERR, return_value = TRUE, final_grade = -1, start_flag = TRUE;
	char *id_lst_file_path = NULL, students_grades_dir_lst[MAX_NUM_OF_STUDENTS][STUDENT_DIR_NAME_LEN], *student_grade_path = NULL;
	char *cmd = NULL, final_grades_path_lst[MAX_NUM_OF_STUDENTS][STUDENT_DIR_NAME_LEN],
		ids_lst[MAX_NUM_OF_STUDENTS][ID_LEN_STR], *final_grade_student_path_no_sufix = NULL, *final_grade_student_path = NULL,
		sufix[5] = ".txt", *total_grades_file = NULL;
	const char student_ids_file_name[20] = "/studentIds.txt", total_grades_sufix[20] = "/final_grades.txt",
		test_grades_str[20] = "TestGrade.exe ";
	
	// Generates the path for all students final grade file 
	if (strcatDynamic(dir_path, total_grades_sufix, &total_grades_file) == FALSE) {
		return ERR;
	}
	// Generates the path for the file that contain all students ids
	if (strcatDynamic(dir_path, student_ids_file_name, &id_lst_file_path) == FALSE) {
		return ERR;
	}
	// extracts all stutents ids and contain them in a list 
	if (parseStudentIDs(students_grades_dir_lst, final_grades_path_lst, ids_lst, &dir_list_len, id_lst_file_path) != TRUE) {
		free(total_grades_file);
		free(id_lst_file_path);
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		return ERR;
	}
	// free unnecessary allocated memory 
	free(id_lst_file_path);

	// Main loopRuns over all the ID
	for (idx = 0; idx < dir_list_len; idx++) {
		// Generates the path to the grades_XXXXXXXXX directory 
		
		return_value1 = strcatDynamic(dir_path, students_grades_dir_lst[idx], &student_grade_path);
		// Generates the cmd commad that would create a process later
		return_value2 = strcatDynamic(test_grades_str, student_grade_path, &cmd);
		// Checks that return values are OK 
		if (return_value1 != TRUE || return_value1 != TRUE) {
			return_value = ERR;
			break;
		}
		//===========================================================
		return_value1 = CreateProcessSimpleMain(cmd, ids_lst[idx]);
		//===========================================================
		//return_value1 = TRUE;  for testing
		// Checks that return values are OK 
		// There was an External Fatal Error with the process creation
		if (return_value1 == ERR) {
			return_value = ERR;
			break;
		}
		// There was an Internal error in the process or in one of the threads that related to the process
		else if (return_value1 == FALSE) {
			str_safe_free(student_grade_path);
			str_safe_free(cmd);
			cmd = NULL;
			student_grade_path = NULL;
			continue;
		}
		
		str_safe_free(cmd);
		cmd = NULL;
		// Generates the path to the student final grade file 
		return_value1 = strcatDynamic(student_grade_path, final_grades_path_lst[idx], &final_grade_student_path_no_sufix);
		return_value2 = strcatDynamic(final_grade_student_path_no_sufix, sufix, &final_grade_student_path);
		// Checks that return values are OK 
		if (return_value1 != TRUE || return_value1 != TRUE) {
			return_value = ERR;
			break;
		}
		// free the allocated memory and initialize the pointer again to NULL
		str_safe_free(student_grade_path);
		student_grade_path = NULL;
		// Extracts the student final grade
		if (readGradeFile(final_grade_student_path, &final_grade) != TRUE) {
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			return_value = ERR;
			break;
		}
		
		// Prints the final student grade into the main file that contains all final grades
		if (printFinalGradeToFile(total_grades_file, ids_lst[idx], final_grade, &start_flag) != TRUE) {
			return_value = ERR;
			break;
		}
		// free all allocated memory and initialize the pointer again to NULL
		str_safe_free(final_grade_student_path_no_sufix);
		str_safe_free(final_grade_student_path);
		final_grade_student_path_no_sufix = NULL;
		final_grade_student_path = NULL;
	}
	// free all alocated memory
	str_safe_free(total_grades_file);
	str_safe_free(final_grade_student_path_no_sufix);
	str_safe_free(final_grade_student_path);
	str_safe_free(student_grade_path);
	str_safe_free(cmd);

	return return_value;
}



int isStudentIdValid(char *id_str)
{
	int idx = 0;
	//Runs over the first 9 chars in the string and checks that they are integers
	while (id_str[idx] != END_OF_STR && idx <= ID_LEN_STR - 3)
	{
		//checks whether the current char is an integer
		if (id_str[idx] < 48 || id_str[idx] > 57)
			return FALSE;
		idx++;
	}
	// if idx smaller then 9 so the ID is not Valid!
	if (idx < ID_LEN_STR - 3)
		return FALSE;
	return TRUE;
}

int removeEnterCharFromString(char *str)
{
	int idx = 0;
	// Runs over the string and looks for the first '\n' and replaces it with '\0'
	while (str[idx] != END_OF_STR)
	{
		if (str[idx] == '\n') {
			str[idx] = END_OF_STR;
			// if the fist occurrence of '\n' is before idx < 9 so the string is not an valid ID
			if (idx < ID_LEN_STR - 3) {
				return FALSE;
			}
			idx--;
		}
		idx++;
	}
	return TRUE;
}

int parseStudentIDs(char student_grades_dir_names[][STUDENT_DIR_NAME_LEN], char final_grades_path_lst[][STUDENT_DIR_NAME_LEN],
	char ids_lst[][ID_LEN_STR], int *dir_list_len, char id_lst_file_path[])
{
	FILE *fp = NULL;
	int ret_val1 = ERR, ret_val2 = ERR, ret_val = TRUE;
	char id[ID_LEN_STR], grades_prefix[9] = "/grades_", final_prefix[9] = "/final_";
	// open the students IDs file
	if (fopen_s(&fp, id_lst_file_path, "r") != FALSE || fp == NULL) {
		return ERR;
	}
	// Runs line by line over the file and read the content to 3 diffrent arrays
	while (!feof(fp) && *dir_list_len <= MAX_NUM_OF_STUDENTS) {
		// Copies the relevant prefix for the file/directory in to the relevant array
		strcpy_s(student_grades_dir_names[*dir_list_len], STUDENT_DIR_NAME_LEN, grades_prefix);
		strcpy_s(final_grades_path_lst[*dir_list_len], STUDENT_DIR_NAME_LEN, final_prefix);

		// reading a line that contains student ID
		fgets(id, ID_LEN_STR, fp);
		// check whether it is a valid ID num and removes the 'new line' char
		ret_val1 = removeEnterCharFromString(id);
		ret_val2 = isStudentIdValid(id);

		// increases the students list by 1 if it is indeed a valid ID
		if (ret_val1 == TRUE && ret_val2 == TRUE) {
			strcat_s(student_grades_dir_names[*dir_list_len], STUDENT_DIR_NAME_LEN, id);
			strcat_s(final_grades_path_lst[*dir_list_len], STUDENT_DIR_NAME_LEN, id);
			strcpy_s(ids_lst[*dir_list_len], ID_LEN_STR, id);
			*dir_list_len += 1;
		}
		// else the file pointer reached to the end-of-the-file/empty-new-line/irrelevant-data
		else {
			break;
		}
		strcpy_s(id, ID_LEN_STR, "notID");
	}
	// close the file and check whether its done successfully
	if (fclose(fp) != FALSE)
		return ERR;
	return ret_val;
}


int printFinalGradeToFile(char *file_path, char *id, int grade, int *start_flag)
{
	FILE *fp = NULL;
	// Checks whether this is the first time that the program trys to open the final grades file
	if (*start_flag == TRUE) {
		// open the file saflly with 'w' mode --> fp points to the begining of the file
		if (fopen_s(&fp, file_path, "w") != FALSE || fp == NULL) {
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			return ERR;
		}
	}
	else {
		// open the file saflly with 'a' mode --> fp points to the end of the file
		if (fopen_s(&fp, file_path, "a") != FALSE || fp == NULL) {
			raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
			return ERR;
		}
	}
	// Prints the student ID and the students final grade in the file
	fprintf_s(fp, "%s %d\n", id, grade);
	// closes the file saflly
	if (fclose(fp) != FALSE) {
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		return ERR;
	}
	// Changes the flag's value to false for the next time
	*start_flag = FALSE;
	return TRUE;


}
