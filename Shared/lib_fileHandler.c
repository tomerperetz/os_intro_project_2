/*
====================================================================================================================
Description:
File handeling functions
====================================================================================================================
*/

// Includes --------------------------------------------------------------------
#include "lib_fileHandler.h"


// Functions --------------------------------------------------------------------
int readGradeFile(char filename[], int *grade)
{
	/*
	Description: Read grade from grades file txt - FOR DEBUGGING. not using threads.
	parameters:
			- char *filename - file string 
			- int *grade - pointer to grade variable
	Returns: true if succeded, IS_FALSE o.w
	*/

	FILE *fp = NULL;
	
	if (fopen_s(&fp, filename, "r") != FALSE)
	{ 
		/*open file failed! RaiseError*/
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		printf("open file FAILED\nFile: %s\n", filename);
		return ERR;
	}
	/*read first line to grade variable*/
	fscanf_s(fp, "%d", grade);
	
	if (fclose(fp) != IS_FALSE)
	{
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		printf("closing file FAILED\nFile: %s\n", filename);
		return ERR;
	}

	return TRUE;
}

DWORD WINAPI readGradeFileThread(LPVOID lpParam)
{

	/*
	Description: Read grade from grades file txt
	parameters:
			- LPVOID lpParam - pointer to functions args
	Returns: STUDENT_GRADE_TREAD__CODE_SUCCESS if succeded, ERR o.w
	*/

	Sleep(10);

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


void freeFilesList(char **files)
{
	/*
	Description: free memory allocation for files path array
	parameters:
			- char **files - array of strings
	Returns: VOID
	*/

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
	/*
	Description: get student id from path
	parameters:
			- char *path - path that has id as last 9 chars
			- char *ID - pointer to buffer that will hold the ID
	Returns: TRUE 
	*/

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
	/*
	Description: print integer to file path
	parameters:
			- char *user_path - output file path
			- int final_grade - integer that need to be printed
	Returns: TRUE when done, ERR o.w
	*/
	FILE *fp = NULL;
	char ID[10];
	char file_extension[5] = ".txt";
	char file_prefix[9] = "/final_";
	char *file_no_prefix = NULL;
	char *filename = NULL;
	char *output_path = NULL;
	
	getStudentID(user_path, ID);

	if (strcatDynamic(ID, file_extension, &file_no_prefix) != IS_TRUE)
		return ERR;
		
	if (strcatDynamic(file_prefix, file_no_prefix, &filename) != IS_TRUE)
		return ERR;
	
	if (strcatDynamic(user_path, filename, &output_path) != IS_TRUE)
		return ERR;
	
	if (fopen_s(&fp, output_path, "w") != FALSE)
	{
		/*open file failed! RaiseError*/
		str_safe_free(file_no_prefix);
		str_safe_free(filename);
		str_safe_free(output_path);
		raiseError(2, __FILE__, __func__, __LINE__, ERROR_ID_2_IO);
		printf("open file FAILED. file name: %s\n", output_path);
		return ERR;
	}

	fprintf_s(fp, "%d", final_grade);
	fclose(fp);

	str_safe_free(file_no_prefix);
	str_safe_free(filename);
	str_safe_free(output_path);

	return TRUE;
}