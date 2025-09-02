#include "mglib.h"

void* NoAPIGetBaseAddress(DWORD target_dll_hash) {
    #ifdef _M_X64
    PPEB_PARTIAL pPeb = (PPEB_PARTIAL)__readgsqword(0x60);  // x64 TEB -> PEB
    #else
    PPEB_PARTIAL pPeb = (PPEB_PARTIAL)__readfsdword(0x30);  // x86 equivalent
    #endif

    LIST_ENTRY* pList = &pPeb->Ldr->InMemoryOrderModuleList;
    LIST_ENTRY* pCurrent = pList->Flink;
    void* ntdllBase = NULL;

    while (pCurrent != pList) {
        PLDR_DATA_TABLE_ENTRY_PARTIAL pEntry =
            (PLDR_DATA_TABLE_ENTRY_PARTIAL)((BYTE*)pCurrent - offsetof(LDR_DATA_TABLE_ENTRY_PARTIAL, InMemoryOrderLinks));

        if (hash_ror13_wstr(pEntry->BaseDllName.Buffer) == target_dll_hash)
        {
            ntdllBase = pEntry->DllBase;
            dwprintf(L"Found %ls at: %p\n", pEntry->BaseDllName.Buffer, pEntry->DllBase);
            break;
        }

        pCurrent = pCurrent->Flink;
    }

    if (!ntdllBase)
        dprintf("Did not find load address for hash: %d : %x\n", target_dll_hash, target_dll_hash);

    return ntdllBase;
}

void* NoAPIGetProcAddress(void* module_base, DWORD target_hash) {
    if (!module_base)
        return NULL;

    BYTE* base = (BYTE*)module_base;
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

    IMAGE_DATA_DIRECTORY exportData = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (!exportData.VirtualAddress)
        return NULL;

    IMAGE_EXPORT_DIRECTORY* exports = (IMAGE_EXPORT_DIRECTORY*)(base + exportData.VirtualAddress);

    DWORD* nameRVAs = (DWORD*)(base + exports->AddressOfNames);
    WORD* ordinals = (WORD*)(base + exports->AddressOfNameOrdinals);
    DWORD* functions = (DWORD*)(base + exports->AddressOfFunctions);

    for (DWORD i = 0; i < exports->NumberOfNames; i++) {
        char* funcName = (char*)(base + nameRVAs[i]);
        DWORD hash = hash_ror13_ansi(funcName);

        if (hash == target_hash) {
            WORD ordinal = ordinals[i];
            DWORD funcRVA = functions[ordinal];
            dprintf("Found address: %p\n", (void*)(base + funcRVA));
            return (void*)(base + funcRVA);
        }
    }

    return NULL;
}