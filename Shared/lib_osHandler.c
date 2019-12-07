/*
====================================================================================================================
Description:
Operation System functions: Proccesses and Threads.
====================================================================================================================
*/

// Includes --------------------------------------------------------------------
#include "lib_osHandler.h"
#include "../TestGrade/studentGrade.h"
#include "lib_fileHandler.h"

// Defines --------------------------------------------------------------------
#define SUCCESS_CODE 0

/**
* Demonstrates win32 process creation and termination.
*/
int CreateProcessSimpleMain(char *command, char *id)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	DWORD				errorMessageID;
	int					return_value = ERR;
	int					retVal2 = 0;
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
		printf("Captain, were unable to calculate %s", id);
		return IS_FALSE;
	}
	if (retVal == 0)
	{
		raiseError(10, __FILE__, __func__, __LINE__, "OS ERROR: The program failed to extract the process exit code!\n");
		return ERR;
	}

	retVal = CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	retVal2 = CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
	if (retVal == 0 || retVal2 == 0) {
		raiseError(10, __FILE__, __func__, __LINE__, "OS ERROR: The program failed to close the Handle!\n");
		return ERR;
	}
	return IS_TRUE;
}

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


// Thread Function Definitions --------------------------------------------------------

void freeThreadParamMem(STUDENT_GRADE_TREAD_params_t **p_thread_params_arr)
{
	for (int i = 0; i < MAX_FILES; i++)
		free(p_thread_params_arr[i]);
	free(p_thread_params_arr);
}

int mainCreateReadGradesThreadSimple(char **files_list, int *grades_list)
{
	/*
	Description: read a list of files content into variables using threads.
	parameters:
			 - char **files_list - files list.
			 -  int *grades_list - empty grades array.
	Return: TRUE if succeded, ERR o.w
	*/

	HANDLE p_thread_handles[MAX_FILES];
	DWORD p_thread_ids[MAX_FILES];
	DWORD num_of_threads = MAX_FILES;
	DWORD wait_code;
	DWORD exit_code;
	BOOL ret_val;
	STUDENT_GRADE_TREAD_params_t **p_thread_params_arr;

	/*
		Allocate memory for thread parameters
		args array for threads function 
	*/
	
	p_thread_params_arr = (STUDENT_GRADE_TREAD_params_t **)malloc(MAX_FILES*sizeof(p_thread_params_arr));
	if (NULL == p_thread_params_arr)
	{
		raiseError(4, __FILE__, __func__, __LINE__, ERROR_ID_4_MEM_ALLOCATE);
		printf("Error when allocating memory");
		return ERR;
	}
	for (int i = 0; i < MAX_FILES; i++)
	{
		p_thread_params_arr[i] = (STUDENT_GRADE_TREAD_params_t *)malloc(sizeof(STUDENT_GRADE_TREAD_params_t));
		if (NULL == p_thread_params_arr[i])
		{
			raiseError(4, __FILE__, __func__, __LINE__, ERROR_ID_4_MEM_ALLOCATE);
			printf("Error when allocating memory");
			return ERR;
		}
	}
	/* Create thread */
	for (int i = 0; i < MAX_FILES; i++)
	{
		/* Prepare parameters for thread */
		p_thread_params_arr[i]->cur_grade = &grades_list[i];
		p_thread_params_arr[i]->cur_file = &files_list[i];
		p_thread_handles[i] = CreateThreadSimple(readGradeFileThread, p_thread_params_arr[i], &p_thread_ids[i]);
		
		if (NULL == p_thread_handles[i])
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("details: Error when creating thread\n");
			freeThreadParamMem(p_thread_params_arr);
			return ERR;
		}
	}


	/* Wait */
	wait_code = WaitForMultipleObjects(MAX_FILES, p_thread_handles, TRUE, INFINITE);
	
	/* WAIT CODE cases*/
	switch (wait_code)
	{
	
	case WAIT_TIMEOUT:
	{
		raiseError(6, __FILE__, __func__, __LINE__ ,ERROR_ID_6_THREADS);
		printf("details: Timeout error when waiting\n");
		freeThreadParamMem(p_thread_params_arr);
		return ERR;
	}
	
	case WAIT_FAILED:
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("details: Timeout error when waiting\n");
		freeThreadParamMem(p_thread_params_arr);
		return ERR;
	}
	
	case WAIT_ABANDONED_0:
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("details: WAIT ANDONED\n");
		freeThreadParamMem(p_thread_params_arr);
		return ERR;
	}
	default:
		break;
	}

	/* Any other case*/
	if (WAIT_OBJECT_0 != wait_code)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("details: Error when waiting. wait code: %d\n", wait_code);
		freeThreadParamMem(p_thread_params_arr);
		return ERR;
	}

	/* Free memory */
	freeThreadParamMem(p_thread_params_arr);

	/* Check the DWORD returned by readGradeFileThread */
	for (int i = 0; i < MAX_FILES; i++)
	{
		ret_val = GetExitCodeThread(p_thread_handles[i], &exit_code);
		if (FALSE == ret_val)
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("details: Error getting thread exit code\n");
			return ERR;
		}

		if (exit_code == ERR) 
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("Error in thread: %d. Exit code: %d\n", i, exit_code);
			return ERR;
		}

		if (SUCCESS_CODE != exit_code)
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("Details: Error in thread: %d. Exit code: %d\n", i, exit_code);
		}


		/* Close thread handle */
		ret_val = CloseHandle(p_thread_handles[i]);
		if (FALSE == ret_val)
		{
			raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
			printf("Details: Error when closing thread\n");
			return ERR;
		}
	}
	return STUDENT_GRADE_TREAD__CODE_SUCCESS;
}


static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id)
{
	/*
	Description: create thread with arg wrapper
	parameters:
			- LPTHREAD_START_ROUTINE p_start_routine
			- LPVOID p_thread_parameters
			- LPDWORD p_thread_id
	Return: thread_handle if succeded, ERR o.w
	*/

	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("Details: Error when creating a thread");
		printf("\tReceived null pointer");
		return NULL;
	}

	if (NULL == p_thread_id)
	{
		raiseError(6, __FILE__, __func__, __LINE__, ERROR_ID_6_THREADS);
		printf("Details: Error when creating a thread");
		printf("\tReceived null pointer");
		return NULL;
	}

	thread_handle = CreateThread(
		NULL,                /*  default security attributes */
		0,                   /*  use default stack size */
		p_start_routine,     /*  thread function */
		p_thread_parameters, /*  argument to thread function */
		0,                   /*  use default creation flags */
		p_thread_id);        /*  returns the thread identifier */

	return thread_handle;
}
