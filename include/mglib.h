#ifndef MGLIB_H
#define MGLIB_H

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

#include "_api_hashes.h"

// DEBUG is currently only for debug printfs
#ifdef DEBUG
#define dprintf(...)  fprintf( stderr, __VA_ARGS__)
#define dwprintf(...) fwprintf( stderr, __VA_ARGS__)
#else
#define dprintf(...) do {} while(0)
#define dwprintf(...) do {} while(0)
#endif

#ifdef MGLIB_EXPORTS
    // Building the DLL: export the symbols
    #define MGLIB_API __declspec(dllexport)
#else
    // Building static lib: no decoration
    #define MGLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Structures
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

// Partial LDR_DATA_TABLE_ENTRY with just what we need
typedef struct _LDR_DATA_TABLE_ENTRY_PARTIAL {
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID      DllBase;
    PVOID      EntryPoint;
    ULONG      SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY_PARTIAL, *PLDR_DATA_TABLE_ENTRY_PARTIAL;

typedef struct _PEB_LDR_DATA {
    ULONG Length;
    BOOLEAN Initialized;
    PVOID SsHandle;
    LIST_ENTRY InLoadOrderModuleList;
    LIST_ENTRY InMemoryOrderModuleList;
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID EntryInProgress;
    BOOL ShutdownInProgress;
    HANDLE ShutdownThreadId;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

// Partial PEB with just what we need
typedef struct _PEB_PARTIAL {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
} PEB_PARTIAL, *PPEB_PARTIAL;

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;
    PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID {
    PVOID UniqueProcess;
    PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

// Privileges
MGLIB_API BOOL SetPrivilege(HANDLE, LPCTSTR, BOOL);

// Process Injection
MGLIB_API BOOL CreateShellcodeThreadInProcess(DWORD, unsigned char*, size_t);

// Registry
MGLIB_API BOOL DeleteRegistryKey(HKEY, LPCSTR);

// Hashes
MGLIB_API uint32_t hash_ror13_ansi(const char*);
MGLIB_API uint32_t hash_ror13_unicode(const PUNICODE_STRING);
MGLIB_API uint32_t hash_ror13_wstr(const wchar_t*);

// NoAPI
MGLIB_API void* NoAPIGetBaseAddress(DWORD);
MGLIB_API void* NoAPIGetProcAddress(void*, DWORD);

// Function typedefs
typedef NTSTATUS(NTAPI *f_NtCreateSection)(PHANDLE SectionHandle, ULONG DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PLARGE_INTEGER MaximumSize, ULONG PageAttributess, ULONG SectionAttributes, HANDLE FileHandle);
typedef NTSTATUS(NTAPI *f_NtMapViewOfSection)(HANDLE SectionHandle,	HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, DWORD InheritDisposition, ULONG AllocationType, ULONG Win32Protect);
typedef NTSTATUS(NTAPI *f_RtlCreateUserThread)(HANDLE ProcessHandle, PSECURITY_DESCRIPTOR SecurityDescriptor, BOOLEAN CreateSuspended, ULONG StackZeroBits, PULONG StackReserved, PULONG StackCommit, PVOID StartAddress, PVOID StartParameter, PHANDLE ThreadHandle, PCLIENT_ID ClientID);

#ifdef __cplusplus
}
#endif

#endif // MGLIB_H
