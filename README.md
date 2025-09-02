# mglib

**mglib** is a work-in-progress Windows library that contains code that I don't want to rewrite over and over again, mostly for Red Team type development. It currently provides a set of utilities for working with Windows privileges, performing process injection, editing the registry, and computing lightweight hashes. The library can be linked statically (`.lib`) or used dynamically via a compiled `.dll`.

---

## Features

- **Privilege Manipulation**
  - Enable, disable, and adjust Windows process privileges.
  
- **Process Injection**
  - Basic routines for injecting code into other processes.
  
- **Registry Editing**
  - Modify Windows Registry keys and values programmatically.
  
- **Hashing Utilities**
  - Implements ROR13 hashing for:
    - ANSI strings
    - Wide-character strings
    - Unicode strings

- **NoAPI Functions**
  - Call Windows APIs *without directly linking* to them or importing their symbols.
  - Useful for stealth operations or avoiding detection in certain environments.
  - I've included the `_api_hashes.h` header with the hashes I've used. It might grow.

---

## Build and Usage

You can build both Debug and Release versions of the library using the provided `build.bat` script. Just run it from the root of the repository:

```
build.bat          :: Builds debug by default
build.bat release  :: Builds release if you specify as 1st argument
```

This script will generate the `.lib` and `.dll` outputs and place them in the appropriate `build`  directory.

### Static Linking

To use the library statically:

1. Include the headers in your project.
2. Link against the provided `.lib` file.

```cpp
#include "mglib.h"
#pragma comment(lib, "mglib.lib")
```

### Dynamic Linking

To use the DLL version:

1. Include the headers in your project.
2. Load the DLL at runtime or link against the import library.

```cpp
// Load at runtime
HMODULE hMod = LoadLibrary(L"mglib.dll");
```

---

## Work in Progress

This project is under active development. The current API surface may change, and additional features are being added. Planned improvements include:

- More injection techniques
- Extended registry handling
- Additional hash functions
- Expanded NoAPI functionality
- Unit tests and usage examples

---

## License

MIT License – see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions, suggestions, and issue reports are welcome!  
Please open an issue if you find a bug or want to suggest a feature.