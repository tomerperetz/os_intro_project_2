/*
====================================================================================================================
Description:
This lib contains all functions related to string handeling
====================================================================================================================
*/

#pragma once
#ifdef _MSC_VER
#endif

// Includes ---------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "../Shared/lib_errorHandler.h"
#include "../Shared/hardCodedData.h"

// Declerations ------------------------------------------------------------------------------------------------------

int strcatDynamic(const char first_source[], const char second_source[], char **buffer);

void str_safe_free(char *ptr);