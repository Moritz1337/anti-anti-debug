#pragma once
#include <windows.h>
#include <iostream>

namespace utils
{
	HANDLE get_process_handle(int pid);
	bool patch_is_debugger_present(HANDLE pid);
}

