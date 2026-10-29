Prediction: The program aparently runs and terminates with no problem, but both ASan (ASan has an experimental memory leak detector, see docs) and Valgrind will flag the memory leak.

```bash
lestat@msi:~/Projects/unix-shell/experiments/memory$    cc main-leak.c -std=c17 -Wall -Wextra -Wpedantic -g -o main
lestat@msi:~/Projects/unix-shell/experiments/memory$ ./maian
bash: ./maian: No such file or directory
lestat@msi:~/Projects/unix-shell/experiments/memory$ ./main
123
123
123
a
4: a
3: 123
2: 123
1: 123
lestat@msi:~/Projects/unix-shell/experiments/memory$ valgrind ./main
==14941== Memcheck, a memory error detector
==14941== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==14941== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==14941== Command: ./main
==14941== 
123
123
123
a
4: a
3: 123
2: 123
1: 123
==14941== 
==14941== HEAP SUMMARY:
==14941==     in use at exit: 120 bytes in 1 blocks
==14941==   total heap usage: 10 allocs, 9 frees, 2,244 bytes allocated
==14941== 
==14941== LEAK SUMMARY:
==14941==    definitely lost: 120 bytes in 1 blocks
==14941==    indirectly lost: 0 bytes in 0 blocks
==14941==      possibly lost: 0 bytes in 0 blocks
==14941==    still reachable: 0 bytes in 0 blocks
==14941==         suppressed: 0 bytes in 0 blocks
==14941== Rerun with --leak-check=full to see details of leaked memory
==14941== 
==14941== For lists of detected and suppressed errors, rerun with: -s
==14941== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
lestat@msi:~/Projects/unix-shell/experiments/memory$ nvim Makefile 
lestat@msi:~/Projects/unix-shell/experiments/memory$    cc main-leak.c -std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer -o asan
lestat@msi:~/Projects/unix-shell/experiments/memory$ ./asan
123
123
123
a
4: a
3: 123
2: 123
1: 123

=================================================================
==14971==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 120 byte(s) in 1 object(s) allocated from:
    #0 0x7ad784efd9c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x7ad784286e64 in __GI___getdelim libio/iogetdelim.c:65
    #2 0x7ad784e9a102 in getline ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:5317
    #3 0x5f2588c5561f in main /home/lestat/Projects/unix-shell/experiments/memory/main-leak.c:18
    #4 0x7ad78422a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #5 0x7ad78422a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #6 0x5f2588c552c4 in _start (/home/lestat/Projects/unix-shell/experiments/memory/asan+0x22c4) (BuildId: 125e6b69259089f5cf02260680ba8392989ccc8f)

SUMMARY: AddressSanitizer: 120 byte(s) leaked in 1 allocation(s).
```
