// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <windows.h>
#include "../Shared/hardCodedData.h"
#include "../Shared/lib_fileHandler.h"

// Constants -------------------------------------------------------------------

#define NUM_THREADS 3
#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))

// Function Declarations -------------------------------------------------------

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id);


int main(int argc, char *argv[])
{
<<<<<<< HEAD
	HANDLE p_thread_handles[NUM_THREADS];
	DWORD p_thread_ids[NUM_THREADS];
	DWORD wait_code;
	BOOL ret_val;
	size_t i;
	char demo_file_path_1[76] = "c:/Users/Adital/Documents/Homework/IOS/HW2/EX2/test_files/ex1_123456789.txt";
	char demo_file_path_2[76] = "c:/Users/Adital/Documents/Homework/IOS/HW2/EX2/test_files/ex2_123456789.txt";

	int score_1 = -1;
	int score_2 = -1;

	printf("threads running BEFORE\n");

	// Create two threads, each thread performs on task.
	p_thread_handles[0] = CreateThreadSimple(readGradeFile(demo_file_path_1, &score_1), &p_thread_ids[0]);
	p_thread_handles[1] = CreateThreadSimple(readGradeFile(demo_file_path_2, &score_2), &p_thread_ids[1]);

	// Wait for IO thread to receive exit command and terminate
	wait_code = WaitForMultipleObjects(NUM_THREADS, p_thread_handles, TRUE, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting");
		return ERROR_CODE;
	}

	// Terminate the other thread
	// Normally, we would avoid terminating a thread so brutally,
	// because it might be in the middle of an operation that should not
	// be interrupted (like writing a file).
	// There are gentler ways of terminating a thread.
	ret_val = TerminateThread(p_thread_handles[1], BRUTAL_TERMINATION_CODE);
	if (false == ret_val)
	{
		printf("Error when terminating\n");
		return ERROR_CODE;
	}

	// Close thread handles
	for (i = 0; i < NUM_THREADS; i++)
	{
		ret_val = CloseHandle(p_thread_handles[i]);
		if (false == ret_val)
		{
			printf("Error when closing\n");
			return ERROR_CODE;
		}
	}

	printf("score 1: %d", score_1);
	printf("score 2: %d", score_2);

=======
	printf("%s\n", argv[1]);
>>>>>>> almost finished Manager
	return 0;
}

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread\n");
		printf("Received null pointer\n");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread\n");
		printf("Received null pointer\n");
		exit(ERROR_CODE);
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		NULL,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}



