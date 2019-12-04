#pragma once
#ifdef _MSC_VER
#endif

#include "../Shared/hardCodedData.h"
#include "../Shared/lib_errorHandler.h"


BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
int CreateProcessSimpleMain(char *command, char *id);