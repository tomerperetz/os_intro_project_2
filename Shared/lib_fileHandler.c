#include "./hardCodedData.h"
#include "lib_errorHandler.h"
#include <Windows.h>

void readGradeFile(char *filename[], int *grade)
{
	/*
	Description: Read grade from grades file txt
	parameters:
			- char *filename - file string 
			- int *grade - pointer to grade variable
	Returns: IS_TRUE if succeded, IS_FALSE o.w
	*/

	FILE *fp = NULL;
	

	printf("working on file: %s\n", filename);

	if (fopen_s(&fp, filename, "r") != FALSE)
	{ 
		/*open file failed! RaiseError*/
		printf("open file FAILED\n");
		return IO_ERR;
	}
	
	/*read first line to grade variable*/
	fscanf_s(fp, "%d", grade);
	fclose(fp);
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