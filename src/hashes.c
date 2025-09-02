#include "mglib.h"

// ROR13 hash for ANSI strings
uint32_t hash_ror13_ansi(const char* str) {
    uint32_t hash = 0;
    while (*str) {
        hash = (hash >> 13) | (hash << (32 - 13));
        hash += (BYTE)(*str++);
    }
    return hash;
}

// ROR13 hash for UNICODE_STRING
uint32_t hash_ror13_unicode(const PUNICODE_STRING us) {
    uint32_t hash = 0;
    if (!us || !us->Buffer || us->Length == 0)
        return 0;

    // Length is in bytes, convert to wchar count
    USHORT len = us->Length / sizeof(WCHAR);
    for (USHORT i = 0; i < len; i++) {
        hash = (hash >> 13) | (hash << (32 - 13));
        hash += (WORD)us->Buffer[i];
    }
    return hash;
}

// ROR13 hash for WSTR
uint32_t hash_ror13_wstr(const wchar_t *wstr) {
    uint32_t hash = 0;

    while (*wstr) {
        // Perform 32-bit rotate right by 13 bits
        hash = (hash >> 13) | (hash << (32 - 13));
        hash += (DWORD)(*wstr);
        wstr++;
    }

    return hash;
}