// dllmain.cpp : Définit le point d'entrée de l'application DLL.
#include <Windows.h>
#include <stdint.h>
#include <fstream>
#include <string>
#include "detours.h"

typedef struct _COPY_MEMORY_BUFFER_INFO
{
    uint64_t case_number;
    uint64_t reserved;
    uint64_t source;
    uint64_t destination;
    uint64_t length;
}COPY_MEMORY_BUFFER_INFO, * PCOPY_MEMORY_BUFFER_INFO;

typedef bool (WINAPI* DEVICEIOCONTROL)(HANDLE,
    DWORD,
    LPVOID,
    DWORD,
    LPVOID,
    DWORD,
    LPDWORD,
    LPOVERLAPPED);

DEVICEIOCONTROL devicecontrol = (DEVICEIOCONTROL)DeviceIoControl;
int nmboftime = 0;

BOOL WINAPI pDeviceIoControl(HANDLE hDevice,
    DWORD dwIoControlCode,
    LPVOID lpInBuffer,
    DWORD nInBufferSize,
    LPVOID lpOutBuffer,
    DWORD nOutBufferSize,
    LPDWORD lpBytesReturned,
    LPOVERLAPPED lpOverlapped)
{
    if (nInBufferSize == sizeof(COPY_MEMORY_BUFFER_INFO)) // check if request have same size of copy memory buffer info
    {
        auto lol = *(COPY_MEMORY_BUFFER_INFO*)lpInBuffer; // read it

        if (lol.case_number = 0x33) // check if case_number is 0x33
        {
            /* time for dumpy dumpo */
            if (lol.length < 20000 || lol.length > 70000) // check if size is good (normally all drivers should pass this verification)
                return devicecontrol(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);

            /* create file and write to it */
            std::ofstream file;
            std::string str = "C:/"; str.append(std::to_string(nmboftime)); str.append(".sys");
            nmboftime++;
            file.open(str.c_str(), std::ios_base::binary);
            char* buffer = reinterpret_cast<char*>(lol.source);
            file.write(&buffer[0], lol.length);

        }
    }
    return devicecontrol(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);

}

auto SetupHook() -> LONG
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)devicecontrol, pDeviceIoControl);
    return DetourTransactionCommit();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SetupHook();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

