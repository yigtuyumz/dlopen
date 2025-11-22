# Dynamic Library Loading with dlopen

This project demonstrates dynamic library loading in C using the POSIX `dlopen` API. It includes a modular Makefile structure that supports multiple dynamic libraries.

## Table of Contents

- [What is dlopen?](#what-is-dlopen)
- [Dynamic vs Static Linking](#dynamic-vs-static-linking)
- [Project Structure](#project-structure)
- [Building the Project](#building-the-project)
- [Makefile Targets](#makefile-targets)
- [Creating Dynamic Libraries](#creating-dynamic-libraries)
- [Adding New Libraries](#adding-new-libraries)
- [Technical Details](#technical-details)

## What is dlopen?

`dlopen` is a POSIX API function that allows programs to load shared libraries (dynamic libraries) at runtime, rather than at compile time. This provides several advantages:

- **Plugin Architecture**: Load functionality on-demand
- **Modularity**: Separate concerns into independent libraries
- **Version Management**: Load different library versions dynamically
- **Reduced Memory Footprint**: Only load what you need
- **Hot Reloading**: Replace library code without restarting the program

### Key Functions

- `dlopen()`: Opens a dynamic library and returns a handle
- `dlsym()`: Retrieves the address of a symbol (function/variable) from the library
- `dlerror()`: Returns a string describing the last error
- `dlclose()`: Closes the dynamic library handle

## Dynamic vs Static Linking

### Static Linking
- Libraries are bundled into the executable at compile time
- Larger executable size
- No external dependencies at runtime
- Faster function calls (no indirection)

### Dynamic Linking
- Libraries are separate `.so` files (Shared Objects on Linux)
- Smaller executable size
- Libraries can be shared between multiple programs
- Libraries can be updated independently
- Slightly slower due to indirection

## Project Structure

```
dlopen/
├── README.md
└── src/
    ├── main.c              # Main program
    ├── dlopen.h            # Header file
    ├── Makefile            # Main Makefile
    └── dynlib/             # Dynamic library directory
        ├── dynlib.c        # Library source
        ├── dynlib.h        # Library header
        └── Makefile        # Library Makefile
```

## Building the Project

### Build Everything
```bash
cd src
make
```

This will:
1. Compile all dynamic libraries (`.so` files)
2. Compile the main program
3. Link the program with `-ldl` flag for dlopen support

### Build Without Dynamic Libraries
```bash
make nolibs
```

Creates `dlopen_nolibs` executable without compiling or linking any dynamic libraries. Useful for testing the main program independently.

### Clean Build
```bash
make clean    # Remove object files and executables
make fclean   # Full clean including library builds
make re       # Clean and rebuild
```

## Makefile Targets

### Primary Targets

| Target | Description |
|--------|-------------|
| `all` | Default target. Builds all libraries and the main executable (`dlopen`) |
| `nolibs` | Builds only the main program without any dynamic libraries (`dlopen_nolibs`) |
| `libs` | Builds all dynamic libraries specified in `LIB_NAMES` |
| `clean` | Removes object files and executables, cleans all library directories |
| `fclean` | Full clean - removes all build artifacts |
| `re` | Clean rebuild - runs `clean` then `all` |

### Library-Specific Targets

Each library has dedicated targets:

| Target | Description |
|--------|-------------|
| `dynlib/libdynlib.so` | Builds the dynlib shared library |
| `clean-dynlib` | Cleans the dynlib directory |
| `fclean-dynlib` | Full clean of dynlib directory |

## Creating Dynamic Libraries

### Compilation Requirements

Dynamic libraries require special compilation flags:

```makefile
CFLAGS := -Wall -Wextra -Werror -fPIC -std=c99
```

- **`-fPIC`**: Position Independent Code - required for shared libraries
  - Allows the library to be loaded at any memory address
  - Essential for shared libraries that may be loaded at different addresses in different processes

### Linking Shared Libraries

```makefile
$(NAME): $(OBJS)
    $(CC) -shared -o $@ $^
```

- **`-shared`**: Creates a shared library instead of an executable
- Output file naming convention: `lib<name>.so`

### Example: dynlib Makefile

```makefile
CC := gcc
CFLAGS := -Wall -Wextra -Werror -fPIC -std=c99
NAME := libdynlib.so

SRCS := dynlib.c
OBJS := $(SRCS:.c=.o)

$(NAME): $(OBJS)
    $(CC) -shared -o $@ $^

%.o: %.c dynlib.h
    $(CC) $(CFLAGS) -o $@ -c $<

clean:
    rm -f $(OBJS) $(NAME)

.PHONY: clean
```

## Adding New Libraries

The Makefile is designed to scale easily. To add a new library:

### Step 1: Add to Variables

Edit `src/Makefile`:

```makefile
LIB_DIRS := dynlib newlib          # Add directory name
LIB_NAMES := dynlib newlib         # Add library name
```

### Step 2: Add Build Rule

```makefile
newlib/libnewlib.so:
    $(MAKE) -C newlib
```

### Step 3: Add Clean Targets

```makefile
.PHONY: clean-newlib
clean-newlib:
    $(MAKE) -C newlib clean

.PHONY: fclean-newlib
fclean-newlib:
    $(MAKE) -C newlib fclean
```

### Step 4: Update Clean Dependencies

```makefile
clean: clean-dynlib clean-newlib
    $(RM) $(NAME) $(NAME_NOLIBS) $(OBJS)

fclean: fclean-dynlib fclean-newlib
    $(RM) $(NAME) $(NAME_NOLIBS) $(OBJS)
```

### Step 5: Create Library Directory

```bash
mkdir src/newlib
cd src/newlib
# Create newlib.c, newlib.h, and Makefile
```

## Technical Details

### Compiler and Flags

```makefile
CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c99
LDFLAGS := -ldl
```

- **`-Wall -Wextra -Werror`**: Enable all warnings and treat them as errors
- **`-std=c99`**: Use C99 standard
- **`-ldl`**: Link against libdl (required for dlopen, dlsym, dlclose, dlerror)

### Make Built-in Functions Used

The Makefile uses only Make built-in functions, no shell commands:

- **`$(foreach ...)`**: Iterates over library names to generate paths
- **`$(RM)`**: Built-in file removal (platform-independent)
- **`$(MAKE) -C dir`**: Recursive make in subdirectories
- **Pattern Rules `%.o: %.c`**: Automatic object file compilation

### Why No Shell Commands?

- **Portability**: Make built-ins work across different platforms
- **Reliability**: No dependency on shell features or external commands
- **Clarity**: Explicit rules are easier to understand and debug
- **Scalability**: Adding libraries doesn't require modifying shell loops

### Library Loading at Runtime

When using `dlopen`, the program must be linked with `-ldl`:

```makefile
$(NAME): $(OBJS) $(LIB_FILES)
    $(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)
```

Example C code:
```c
#include <dlfcn.h>

void *handle = dlopen("./dynlib/libdynlib.so", RTLD_LAZY);
if (!handle) {
    fprintf(stderr, "dlopen error: %s\n", dlerror());
    return 1;
}

void (*func)(void) = dlsym(handle, "function_name");
if (!func) {
    fprintf(stderr, "dlsym error: %s\n", dlerror());
    dlclose(handle);
    return 1;
}

func();  // Call the dynamically loaded function
dlclose(handle);
```

## Best Practices

1. **Error Checking**: Always check return values from `dlopen`, `dlsym`
2. **Memory Management**: Call `dlclose()` when done with a library
3. **Symbol Visibility**: Use `dlsym()` to get function pointers, never call directly
4. **Thread Safety**: Be aware that loading/unloading libraries may not be thread-safe
5. **Path Management**: Use absolute or relative paths carefully with `dlopen`

## References

- [dlopen(3) man page](https://man7.org/linux/man-pages/man3/dlopen.3.html)
- [Position Independent Code (PIC)](https://en.wikipedia.org/wiki/Position-independent_code)
- [GNU Make Manual](https://www.gnu.org/software/make/manual/)
- [Shared Libraries (TLDP)](https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html)