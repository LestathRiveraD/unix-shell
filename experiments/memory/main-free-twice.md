Prediction: According to logs, undefined behavior will occur, so at least I'm expecting valgrind/ASan to not flag it, not necessarily. 

```bash
lestat@msi:~/Projects/unix-shell/experiments/memory$ cc main-free-twice.c -std=c17 -Wall -Wextra -Wpedantic -g -o main
lestat@msi:~/Projects/unix-shell/experiments/memory$ valgrind ./main
==17719== Memcheck, a memory error detector
==17719== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==17719== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==17719== Command: ./main
==17719== 
123
123
123
a
==17719== Invalid free() / delete / delete[] / realloc()
==17719==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17719==    by 0x1092FD: main (main-free-twice.c:29)
==17719==  Address 0x4a90090 is 0 bytes inside a block of size 120 free'd
==17719==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17719==    by 0x1092F1: main (main-free-twice.c:28)
==17719==  Block was alloc'd at
==17719==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==17719==    by 0x4900E64: getdelim (iogetdelim.c:65)
==17719==    by 0x1092DF: main (main-free-twice.c:18)
==17719== 
4: a
3: 123
2: 123
1: 123
==17719== 
==17719== HEAP SUMMARY:
==17719==     in use at exit: 0 bytes in 0 blocks
==17719==   total heap usage: 10 allocs, 11 frees, 2,244 bytes allocated
==17719== 
==17719== All heap blocks were freed -- no leaks are possible
==17719== 
==17719== For lists of detected and suppressed errors, rerun with: -s
==17719== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
lestat@msi:~/Projects/unix-shell/experiments/memory$ nvim main-free-twice.c
lestat@msi:~/Projects/unix-shell/experiments/memory$ man 3 malloc
lestat@msi:~/Projects/unix-shell/experiments/memory$ cc main-free-twice.c -std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer -o asan
lestat@msi:~/Projects/unix-shell/experiments/memory$ ./asan
123
123
123
d
=================================================================
==15187==ERROR: AddressSanitizer: attempting double-free on 0x50c000000040 in thread T0:
    #0 0x7b1f134fc4d8 in free ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:52
    #1 0x6208b849268a in main /home/lestat/Projects/unix-shell/experiments/memory/main-free-twice.c:29
    #2 0x7b1f1282a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x7b1f1282a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #4 0x6208b84922c4 in _start (/home/lestat/Projects/unix-shell/experiments/memory/asan+0x22c4) (BuildId: 8aabbce8839a1eb0dedec72a0225ec881291d120)

0x50c000000040 is located 0 bytes inside of 120-byte region [0x50c000000040,0x50c0000000b8)
freed by thread T0 here:
    #0 0x7b1f134fc4d8 in free ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:52
    #1 0x6208b849265b in main /home/lestat/Projects/unix-shell/experiments/memory/main-free-twice.c:28
    #2 0x7b1f1282a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x7b1f1282a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #4 0x6208b84922c4 in _start (/home/lestat/Projects/unix-shell/experiments/memory/asan+0x22c4) (BuildId: 8aabbce8839a1eb0dedec72a0225ec881291d120)

previously allocated by thread T0 here:
    #0 0x7b1f134fd9c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x7b1f12886e64 in __GI___getdelim libio/iogetdelim.c:65
    #2 0x7b1f1349a102 in getline ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:5317
    #3 0x6208b8492622 in main /home/lestat/Projects/unix-shell/experiments/memory/main-free-twice.c:18
    #4 0x7b1f1282a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #5 0x7b1f1282a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #6 0x6208b84922c4 in _start (/home/lestat/Projects/unix-shell/experiments/memory/asan+0x22c4) (BuildId: 8aabbce8839a1eb0dedec72a0225ec881291d120)

SUMMARY: AddressSanitizer: double-free ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:52 in free
==15187==ABORTING
```

My prediction was wrong, both ASan and Valgrind did flag the double free.
