#include "mglib.h"
#pragma comment(lib, "advapi32.lib")

// RegSetValueExW(
//     _In_ HKEY hKey,
//     _In_opt_ LPCWSTR lpValueName,
//     _Reserved_ DWORD Reserved,
//     _In_ DWORD dwType,
//     _In_reads_bytes_opt_(cbData) CONST BYTE* lpData,
//     _In_ DWORD cbData
//     );

// BOOL CreateRegistryKeyValueString(LPCSTR IMAGE_KEY, LPCSTR ValueName) {
//     BOOL success = FALSE;
//     HKEY hKey    = NULL;

//     if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, IMAGE_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
//         printf("Error creating key: %s\n", IMAGE_KEY);
//         goto cleanup;
//     }

//     if (RegSetValueEx(hKey, "debugger", 0, REG_SZ, PROCESSBLOCKER, sizeof(PROCESSBLOCKER)) != ERROR_SUCCESS)
//     {
//         printf("Error setting debugger value for key: %s\n", IMAGE_KEY);
//         goto cleanup;
//     }

//     success = TRUE;
//     cleanup:
//     if (hKey)
//         RegCloseKey(hKey);
//     return success;
// }

// Example: DeleteRegistryKey(HKEY_LOCAL_MACHINE, "SomeKeyValue")
BOOL DeleteRegistryKey(HKEY HIVE, LPCSTR IMAGE_KEY) {
    BOOL success = FALSE;

    if (RegDeleteKeyEx(HIVE, IMAGE_KEY, KEY_WOW64_32KEY, 0) == ERROR_SUCCESS) {
        dprintf("Deleted 32-bit Registry Key: %s\n", IMAGE_KEY);
        success = TRUE;
    }
    if (RegDeleteKeyEx(HIVE, IMAGE_KEY, KEY_WOW64_64KEY, 0) == ERROR_SUCCESS) {
        dprintf("Deleted 64-bit Registry Key: %s\n", IMAGE_KEY);
        success = TRUE;
    }
    return success;
}