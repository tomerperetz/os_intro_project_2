#include "lib_fileHandler.h"
#include "lib_str_func.h"
#include "lib_osHandler.h"

int readGradeFile(char filename[], int *grade)
{
	/*
	Description: Read grade from grades file txt
	parameters:
			- char *filename - file string 
			- int *grade - pointer to grade variable
	Returns: IS_TRUE if succeded, IS_FALSE o.w
	*/

	FILE *fp = NULL;
	
	if (fopen_s(&fp, filename, "r") != FALSE)
	{ 
		/*open file failed! RaiseError*/
		printf("open file FAILED\nFile: %s\n", filename);
		return IO_ERR;
	}
	/*read first line to grade variable*/
	fscanf_s(fp, "%d", grade);
	
	if (fclose(fp) != IS_FALSE)
		return IO_ERR;

	return TRUE;
}

DWORD WINAPI readGradeFileThread(LPVOID lpParam)
{
	Sleep(10);

	/*
	Description: Read grade from grades file txt
	parameters:
			- char *filename - file string
			- int *grade - pointer to grade variable
	Returns: IS_TRUE if succeded, IS_FALSE o.w
	*/

	FILE *fp = NULL;
	STUDENT_GRADE_TREAD_params_t *p_params;


	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("Recived null pointer");
		return ERR;
	}

	p_params = (STUDENT_GRADE_TREAD_params_t *)lpParam;
	

	if (fopen_s(&fp, *(p_params->cur_file), "r") != FALSE)
	{
		/*open file failed! RaiseError*/
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		printf("File: %s\n", *(p_params->cur_file));
		return ERR;
	}
	/*read first line to grade variable*/
	fscanf_s(fp, "%d", p_params->cur_grade);

	if (fclose(fp) != IS_FALSE)
	{
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		printf("error closing file: %s\n", *(p_params->cur_file));
		return ERR;
	}


	return STUDENT_GRADE_TREAD__CODE_SUCCESS;
}


char* getFilePath(const char dir_path[], const char file_name[])
{
	int retVal = 0, size_of_file_path = 0;
	char *file_path_buffer;

	size_of_file_path = sizeof(char)*(strlen(dir_path) + strlen(file_name) + 1);
	file_path_buffer = (char*)malloc(size_of_file_path);
	if (file_path_buffer == NULL)
		return NULL;
	file_path_buffer[size_of_file_path] = '\0';
	strcpy_s(file_path_buffer, size_of_file_path, dir_path);
	strcat_s(file_path_buffer, size_of_file_path, file_name);
	return file_path_buffer;

}

void freeFilesList(char **files)
{

	// free memory allocation
	for (int i = 0; i < MAX_FILES; i++)
	{
		if (files[i] != NULL)
			free(files[i]);
	}
	if (files != NULL)
		free(files);

}

int getStudentID(char *path, char *ID)
{
	int path_len = 0;
	path_len = strlen(path);

	for (int i = path_len - 9, j=0 ; i < path_len, j<9 ;i++, j++)
	{
		ID[j] = path[i];
	}
		
	ID[9] = '\0';

	return TRUE;
}



int printToFile(char *user_path, int final_grade)
{
	FILE *fp = NULL;
	char ID[10];
	char file_extension[5] = ".txt";
	char file_prefix[9] = "/final_";
	char *file_no_prefix = NULL;
	char *filename = NULL;
	char *output_path = NULL;
	
	getStudentID(user_path, ID);

	if (strcatDynamic(ID, file_extension, &file_no_prefix) != IS_TRUE)
	{
		str_safe_free(file_no_prefix);
		return FALSE;
	}
		
	if (strcatDynamic(file_prefix, file_no_prefix, &filename) != IS_TRUE)
	{
		str_safe_free(filename);
		return FALSE;
	}
	if (strcatDynamic(user_path, filename, &output_path) != IS_TRUE)
	{
		str_safe_free(output_path);
		return FALSE;
	}
	if (fopen_s(&fp, output_path, "w") != FALSE)
	{
		/*open file failed! RaiseError*/
		printf("open file FAILED\n");
		return IO_ERR;
	}

	fprintf_s(fp, "%d", final_grade);
	fclose(fp);

	str_safe_free(file_no_prefix);
	str_safe_free(filename);
	str_safe_free(output_path);

	return TRUE;
}