#include "../Shared/hardCodedData.h"
#include "../Shared/argparser.h"
#include "../Shared/lib_errorHandler.h"





int getFilePath(const char dir_path[], const char file_name[], char *file_path_buffer)
{
	int retVal = 0, size_of_file_path = 0;

	size_of_file_path = sizeof(char)*(strlen(dir_path) + strlen(file_name)+1);
	file_path_buffer = (char*) malloc(size_of_file_path);
	if (file_path_buffer == NULL)
		return ERR;
	file_path_buffer[size_of_file_path] = '\0';
	strcpy_s(file_path_buffer, size_of_file_path, dir_path);
	strcat_s(file_path_buffer, size_of_file_path, file_name);
	return IS_TRUE;
}

int parseStudentLst(char *student_ids[], char file_path[])
{
	return IS_TRUE;
}

int main(int argc, char *argv[])
{
	char *file_path_buffer = NULL;
	const char student_ids_file_name[20] = "/studentIds.txt";

	if (ensureArgs(argc, EXPECTED_ARGC, argv) != IS_TRUE) {
		raiseError(1, __FILE__, __func__, __LINE__, ERROR_ID_1_CONTENT);
		return IS_TRUE;
	}

	if (getFilePath(argv[1], student_ids_file_name, file_path_buffer) != IS_TRUE) {
		raiseError(7, __FILE__, __func__, __LINE__, ERROR_ID_7_CONTENT);
		return IS_TRUE;
	}
	
	free(file_path_buffer);
	//if (fopen_s(&fp, "Computation.txt", "w") != IS_FALSE) {
	//	raiseError(4, _FILE, __func, __LINE_, ERROR_ID_4_CONTENT);
	//}
	
	printf("The grades have arrived, captain");
	return IS_FALSE;
}