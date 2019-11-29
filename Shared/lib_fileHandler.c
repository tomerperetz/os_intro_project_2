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