/*
====================================================================================================================
Description:
This lib contains all functions related to string handeling
====================================================================================================================
*/

#include "../Shared/lib_errorHandler.h"
#include "../Shared/hardCodedData.h"

int countOpretors(char *dest_str)
{
	int counter = 0;
	for (int i = 0; dest_str[i] != '\0'; i++)
	{
		if ((dest_str[i] == '+') || (dest_str[i] == '*'))
			counter++;
	}
	return counter;
}

int getCharindex(char desired_char, char *dest_str)
{

	/*
	Description: get index for first apperance of a given char
	parameters: 
			 - char desired_char - char that is being looked for
			 - char *dest_str - string to look for the char in
	Returns: int index if the char was found. ERR o.w
	*/
	
	int len = ERR;
	len = strlen(dest_str);

	for (int i = 0; i<=len; i++)
	{
		if (dest_str[i] == desired_char)
		{
			return i;
		}
	}
	
	return ERR;

}

int getFirstIntFromString(char *dest_str, int last_index)
{
	/*
	Description: get first integer in a string with the structure: "X + Y". convert from string to int.
	parameters:
			 - char *dest_str - string to look for the int in
			 - int last_index - maximal index the int might be found in.
	Returns: int.
	*/

	int int_x = - 1;
	char char_x[MAX_SON_ARG_INPUT_LEN];
	
	for (int i = 0; i < last_index; i++)
	{
		char_x[i] = dest_str[i];
	}

	char_x[last_index] = '\0';
	int_x = atoi(char_x);
	
	return int_x;
}

int getLastIntFromString(char *dest_str, int first_index)
{
	/*
	Description: get last integer in a string with the structure: "X + Y". convert from string to int.
	parameters:
			 - char *dest_str - string to look for the int in
			 - int last_index - opertor index to initialize the first search index.
	Returns: int.
	*/

	int int_x = -1, last_i = 0, iter = 0;
	char char_x[MAX_SON_ARG_INPUT_LEN];

	for (int i = first_index; dest_str[i]!='\0'; i++)
	{
		char_x[iter] = dest_str[i];
		iter++;
	}

	char_x[iter] = '\0';
	int_x = atoi(char_x);
	return int_x;
}

void add_sub_string_to_string(char dest_str[], char sub_str[], int start_idx) {

	/*
	Description: add sub-string to given string, starting from a giving index.
	parameters:
			 - char dest_str[] - destanation string
			 - char sub_str[] - desired string to be added
			 - int start_idx - start index to add the substring in
	Returns: void
	*/

	int idx = 0;
	for (idx = 0; sub_str[idx] != END_OF_STR; idx++, start_idx++)
		dest_str[start_idx] = sub_str[idx];
	dest_str[start_idx] = END_OF_STR;

}

void replace_sub_string(char *string, int sub_str_start_idx, int sub_str_end_idx, char *new_sub_string) 
{
	/*
	Description: replace sub string in a given string, with a new substring.
	parameters:
			- char *string - main string.
			- int sub_str_start_idx - start index to replace
			- int sub_str_end_idx - end index to replace
			- char *new_sub_string - new string to insert
	Returns: void
	*/
	char string_start[MAX_FATHER_ARG_INPUT_LEN], string_end[MAX_FATHER_ARG_INPUT_LEN];

	strcpy_s(string_end, MAX_FATHER_ARG_INPUT_LEN, &string[sub_str_end_idx + 1]);
	string[sub_str_start_idx] = END_OF_STR;
	strcpy_s(string_start, MAX_FATHER_ARG_INPUT_LEN, string);
	sprintf_s(string, MAX_FATHER_ARG_INPUT_LEN, "%s%s%s", string_start, new_sub_string, string_end);
}

int strcatDynamic(const char first_source[], const char second_source[], char **buffer)
{
	int retVal1 = ERR, retVal2 = ERR, size_of_file_path = 0;

	size_of_file_path = (strlen(first_source) + strlen(second_source) + 1);
	*buffer = (char*)malloc(sizeof(char)*size_of_file_path);
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