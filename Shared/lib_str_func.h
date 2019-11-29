/*
====================================================================================================================
Description:
this lib holds caluclation functions.
====================================================================================================================
*/

#pragma once
#ifdef _MSC_VER
//#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>


int getCharindex(char desired_char, char *dest_str);

int getFirstIntFromString(char *dest_str, int last_index);

int getLastIntFromString(char *dest_str, int first_index);

void replace_sub_string(char *string, int sub_str_start_idx, int sub_str_end_idx, char *new_sub_string);

void add_sub_string_to_string(char dest_str[], char sub_str[], int start_idx);