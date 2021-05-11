#include "includes.h"

BOOLEAN WINAPI main()
{
	utils::patch_is_debugger_present(utils::get_process_handle(40916));

	while (1);
}