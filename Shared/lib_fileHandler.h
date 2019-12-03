#pragma once
#include "./hardCodedData.h"
#include "lib_errorHandler.h"
#include "lib_str_func.h"
#include <Windows.h>

int readGradeFile(char filename[], int *grade);

char* getFilePath(const char dir_path[], const char file_name[]);

void freeFilesList(char **files);

int getStudentID(char *path, char *ID);

void str_safe_free(char *ptr);

int printToFile(char *filename, int final_grade);