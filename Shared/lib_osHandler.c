#include "lib_osHandler.h"

/**
* Demonstrates win32 process creation and termination.
*/
int CreateProcessSimpleMain(char *command)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	DWORD				errorMessageID;
	int					return_value = ERR;
	retVal = CreateProcessSimple(command, &procinfo);


	if (retVal == 0)
	{
		raiseError(7, __FILE__, __func__, __LINE__, "Process Creation Failed!\n");
		return ERR;
	}


	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 10 secs for the process to end */

	switch (waitcode)
	{
		case WAIT_TIMEOUT:
			raiseError(11, __FILE__, __func__, __LINE__, "RunTimeError: The Process 'TestGrade' was Timed out!\n");
			break;
		case WAIT_FAILED:
			errorMessageID = GetLastError();
			raiseError(12, __FILE__, __func__, __LINE__, "RunTimeError: The Process wait has been faild!\nError code:");
			printf("%d\n", errorMessageID);
			break;
		case WAIT_ABANDONED:
			raiseError(13, __FILE__, __func__, __LINE__, "RunTimeError: The specified object is a mutex object that was not released by the thread that owned the mutex object before the owning thread terminated!\n");
			break;
		case WAIT_OBJECT_0:
			return_value = IS_TRUE;
			break;
		default:
			raiseError(14, __FILE__, __func__, __LINE__, "RunTimeError: wait code has an unknwon value!\n");
			printf("waitcode value:  0x%x", waitcode);
			break;
	}
	if (return_value == ERR) /* Process is still alive */
	{
		TerminateProcess(procinfo.hProcess, BRUTAL_TERMINATION_CODE); /* Terminating process with an exit code of -10 */
		Sleep(10); /* Waiting a few milliseconds for the process to terminate */
		return ERR;
	}

	retVal = GetExitCodeProcess(procinfo.hProcess, &exitcode);
	if (exitcode != 0) {
		return ERR;
	}
	if (retVal == 0)
	{
		raiseError(10, __FILE__, __func__, __LINE__, "OS ERROR: The program failed to extract the process exit code!\n");
		return ERR;
	}

	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
	return IS_TRUE;
}

// oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO /

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}