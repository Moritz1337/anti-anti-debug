#include "utils.h"

HANDLE utils::get_process_handle(int pid)
{
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (handle)
        printf_s("[+] obtained handle to target process -> %p\n", handle);
    else
        printf_s("[-] couldn't obtain handle to target process -> %p\n", handle);
    return handle;
}

bool utils::patch_is_debugger_present(HANDLE handle)
{
    if (!handle)
        return false;

    PVOID is_debugger_present = GetProcAddress(LoadLibrary("kernelbase.dll"), "IsDebuggerPresent");

    printf_s("[+] IsDebuggerPresent -> %p\n", is_debugger_present);

    if (!is_debugger_present)
        return false;

    BYTE orig[] = { 0x00, 0x00 ,0x00 ,0x00 ,0x00, 0x00, 0x00 ,0x00 ,0x00 ,0x00, 0x00, 0x00, 0x00 };

    BYTE shell_code[] = {
        0x65, 0x48, 0x8B, 0x04, 0x25, 0x60, 0x00, 0x00, 0x00, // mov rax,qword ptr gs:[60]
        0x0F, 0xB6, 0x40, 0x00, // movzx eax, byte ptr ds:[rax]
    };

    RtlSecureZeroMemory(&orig, sizeof(orig));
    memcpy(orig, &shell_code, sizeof(shell_code));

   if (WriteProcessMemory(handle, is_debugger_present, &orig, sizeof(orig), NULL))
   {
       printf_s("[+] patched out IsDebuggerPresent\n");
       return true;
   }
}