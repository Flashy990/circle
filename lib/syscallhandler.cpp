#include <circle/syscallhandler.h>
#include <circle/logger.h>
#include <circle/sched/scheduler.h>
#include <string.h>

int SyscallHandler(int arg1, int arg2, int arg3, int arg4) {
	register long syscall_no asm ("r7");
	// CLogger::Get ()->Write ("SyscallHandler", LogDebug, "syscall_no=0x%x.", syscall_no);

	switch (syscall_no) {
		case 0x0:
			{
				// int gettime()
				auto currTime = CTimer::Get()->GetTime();
				return currTime;
			}
			break;
		case 0x1:
			{
				// void get_task_name(char *arr, int arr_size)
				strncpy ((char *) arg1, CScheduler::Get ()->GetCurrentTask ()->GetName (), arg2);
			}
			break;
		case 0x2:
			{
				// void print(const char* s) 
				// use log debug for this or no?
				CLogger::Get ()->Write (NULL, LogDebug, (const char *) arg1);
			}
			break;
		case 0x3:
			{
				// void sleep(int seconds)
				CScheduler::Get ()->Sleep (arg1);
			}
			break;
		case 0x4:
			{
				// void exit()
				// CScheduler::Get ()->GetCurrentTask ()->Terminate ();
				DisableIRQs();
				CScheduler::Get ()->GetCurrentTask ()->m_State = TaskStateTerminated;
				CScheduler::Get ()->GetCurrentTask ()->m_Event.Set();
				CScheduler::Get()->Yield();



			}
			break;
		default:
			// CLogger::Get ()->Write ("Unknown syscall number!");
			CLogger::Get ()->Write ("SyscallHandler", LogDebug, "syscall_no=0x%x.", syscall_no);
	}

	return 0;
}
