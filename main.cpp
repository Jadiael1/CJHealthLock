#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

DWORD getProcessPidByName(const wchar_t *processName)
{
    PROCESSENTRY32W entry;
    entry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        printf("Unable to create process snapshot.\n");
        return 0;
    }

    if (Process32FirstW(snapshot, &entry))
    {
        do
        {
            if (_wcsicmp(entry.szExeFile, processName) == 0)
            {
                DWORD pid = entry.th32ProcessID;
                CloseHandle(snapshot);
                return pid;
            }
        }
        while (Process32NextW(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return 0;
}

int main()
{
    SHORT keyF3;
    bool active = false;
    DWORD PIDGTASA = getProcessPidByName(L"gta_sa.exe");
    if (PIDGTASA == 0)
    {
        printf("Nao foi possivel encontrar o processo gta_sa.exe.\n");
        return 1;
    }
    float healthValue = 180.0f;
    DWORD cbNeeded;

    HANDLE processGTASA = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, PIDGTASA);
    HMODULE moduleGTASA = NULL;
    MODULEINFO moduleInfo;

    if (processGTASA == NULL)
    {
        printf("It was not possible to open the process");
        exit(1);
    }

    WINBOOL hasEnumModule = EnumProcessModules(processGTASA, &moduleGTASA, sizeof(moduleGTASA), &cbNeeded);
    if (!hasEnumModule)
    {
        printf("Unable to enumerate modules.");
        exit(1);
    }

    WINBOOL hasModule = GetModuleInformation(processGTASA, moduleGTASA, &moduleInfo, sizeof(moduleInfo));
    if (!hasModule)
    {
        printf("Unable to obtain module.");
        exit(1);
    }
    printf("Programa iniciado.\n");
    printf("Pressione F3 para ativar/desativar.\n");

    uintptr_t memoryBase = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
    printf("memoryBase: %p\n", (void *)memoryBase);

    uintptr_t memoryOffset = 0x007FB7D8;
    printf("memoryOffset: %p\n", (void *)memoryOffset);

    uintptr_t memoryBaseAddress = memoryBase + memoryOffset;
    printf("memoryBaseAddress: %p\n", (void *)memoryBaseAddress);

    DWORD memoryBaseAddressValue;

    WINBOOL hasValue = ReadProcessMemory(processGTASA, (LPCVOID)memoryBaseAddress, &memoryBaseAddressValue, sizeof(memoryBaseAddressValue), NULL);
    if (!hasValue)
    {
        printf("Unable to get memoryBaseAddress value.");
        exit(1);
    }
    printf("memoryBaseAddressValue: %X\n", (unsigned int)memoryBaseAddressValue);

    uintptr_t finalAddr = (uintptr_t)memoryBaseAddressValue + 0x540;
    uintptr_t ptr = finalAddr;


    DWORD lastTick = GetTickCount();
    const DWORD WRITE_INTERVAL = 200;

    for (;;)
    {
        keyF3 = GetAsyncKeyState(VK_F3);
        if (keyF3 & 0x0001)
        {
            active = !active;
            printf("Status: %s.\n", active ? "ATIVADO" : "DESATIVADO");
        }

        if(active)
        {
            DWORD now = GetTickCount();
            if (now - lastTick >= WRITE_INTERVAL)
            {
                lastTick = now;
                float health = 0.0f;
                SIZE_T bytesRead = 0;
                WINBOOL hasValue = ReadProcessMemory(processGTASA, (LPCVOID)ptr, &health, sizeof(health), &bytesRead) && bytesRead == sizeof(health);
                if (!hasValue)
                {
                    printf("Failed to read float in %p. erro=%lu\n", (void *)ptr, GetLastError());
                    break;
                }

                printf("health (float) = %f\n", health);
                WINBOOL wrote = WriteProcessMemory(processGTASA, (LPVOID)ptr, &healthValue, sizeof(healthValue), NULL);
                if (!wrote)
                {
                    printf("WriteProcessMemory failed.\n");
                    break;
                }
            }
        }
        Sleep(10);
    }
}
