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


int strcatDynamic(const char first_source[], const char second_source[], char **buffer);

void str_safe_free(char *ptr);