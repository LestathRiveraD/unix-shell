## Make:
    1. A MakeFile is similar in spirit to a 'package.json' file (not literally, but is a nice comparison)
    2. It tracks dependencies and automatically runs commands.
    3. The basic syntax: file: prerequisites\n commands
    4. If file does not exist OR any of the prerequisites have changed since file was last compiled, it recompiles, else, it does not recompile. So, it compares who changed last to decide whether to recompile or not.

## Why can't I run Valgrind on the ASan build?
```bash
lestat@msi:~/Projects/unix-shell$ valgrind ./asan
==192989== Memcheck, a memory error detector
==192989== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==192989== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==192989== Command: ./asan
==192989== 
==192989==ASan runtime does not come first in initial library list; you should either link runtime to your application or manually preload it with LD_PRELOAD.
```

```bash
lestat@msi:~/Projects/unix-shell$ ./asan
main.c:6:56: runtime error: index 3 out of bounds for type 'int [3]'
main.c:6:5: runtime error: load of address 0x76efe310002c with insufficient space for an object of type 'int'
0x76efe310002c: note: pointer points here
  03 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  00 80 0f e3 ef 76 00 00  00 00 00 00 00 00 00 00
              ^ 
=================================================================
==193131==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x76efe310002c at pc 0x60edefc1342d bp 0x7ffe3a0ee140 sp 0x7ffe3a0ee130
```

Can't run valgrind and ASan together because they both try to detect memory errors with different mechanisms, so their runtimes may conflict.

References

1. Valgrind. (2022). *Valgrind User Manual*.
   https://valgrind.org/docs/manual/valgrind_manual.pdf

2. Clang. (n.d.). *AddressSanitizer*.
   https://clang.llvm.org/docs/AddressSanitizer.html

##  Memory

    1. malloc() allocates size bytes of memory and returns a pointer to the allocated memory 
    2. realloc() receives a pointer and resizes the memory block to byte size. If the new size is larger than the old size, the added memory will not be initialized.
    3. strdup() duplicates a string and returns a pointer to the duplicated string 
    4. getline() receives a pointer to a buffer pointer, a size pointer n to the buffer's allocated size, and a stream. It reads a line of text from stream into the buffer. If not enough memory is allocated for the string, getlines reallocs the memory block
