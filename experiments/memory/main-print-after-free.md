Prediction: I'm expecting this to actually print the last line read by getline, but I think this is undefined behavior, and the block pointed to by s* may be corrupt in some way because nobody owns it.

```bash
lestat@msi:~/Projects/unix-shell/experiments/memory$ ./main
123
abc
!au�
2: abc
1: 123
lestat@msi:~/Projects/unix-shell/experiments/memory$ valgrind ./main
==15564== Memcheck, a memory error detector
==15564== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==15564== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==15564== Command: ./main
==15564== 
123
123
a
==15564== Invalid read of size 1
==15564==    at 0x484F226: strlen (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15564==    by 0x4901CDC: puts (ioputs.c:35)
==15564==    by 0x10931D: main (main-print-after-free.c:29)
==15564==  Address 0x4a90090 is 0 bytes inside a block of size 120 free'd
==15564==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15564==    by 0x109311: main (main-print-after-free.c:28)
==15564==  Block was alloc'd at
==15564==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15564==    by 0x4900E64: getdelim (iogetdelim.c:65)
==15564==    by 0x1092FF: main (main-print-after-free.c:18)
==15564== 
==15564== Invalid read of size 1
==15564==    at 0x484F234: strlen (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15564==    by 0x4901CDC: puts (ioputs.c:35)
==15564==    by 0x10931D: main (main-print-after-free.c:29)
==15564==  Address 0x4a90091 is 1 bytes inside a block of size 120 free'd
==15564==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15564==    by 0x109311: main (main-print-after-free.c:28)
==15564==  Block was alloc'd at
==15564==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15564==    by 0x4900E64: getdelim (iogetdelim.c:65)
==15564==    by 0x1092FF: main (main-print-after-free.c:18)
==15564== 
==15564== Invalid read of size 1
==15564==    at 0x490F74B: _IO_default_xsputn (genops.c:399)
==15564==    by 0x490F74B: _IO_default_xsputn (genops.c:370)
==15564==    by 0x490DBC9: _IO_new_file_xsputn (fileops.c:1265)
==15564==    by 0x490DBC9: _IO_file_xsputn@@GLIBC_2.2.5 (fileops.c:1197)
==15564==    by 0x4901D3C: puts (ioputs.c:40)
==15564==    by 0x10931D: main (main-print-after-free.c:29)
==15564==  Address 0x4a90090 is 0 bytes inside a block of size 120 free'd
==15564==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15564==    by 0x109311: main (main-print-after-free.c:28)
==15564==  Block was alloc'd at
==15564==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==15564==    by 0x4900E64: getdelim (iogetdelim.c:65)
==15564==    by 0x1092FF: main (main-print-after-free.c:18)
==15564== 
a

3: a
2: 123
1: 123
==15564== 
==15564== HEAP SUMMARY:
==15564==     in use at exit: 0 bytes in 0 blocks
==15564==   total heap usage: 9 allocs, 9 frees, 2,239 bytes allocated
==15564== 
==15564== All heap blocks were freed -- no leaks are possible
==15564== 
==15564== For lists of detected and suppressed errors, rerun with: -s
==15564== ERROR SUMMARY: 5 errors from 3 contexts (suppressed: 0 from 0)
lestat@msi:~/Projects/unix-shell/experiments/memory$    cc main-print-after-free.c -std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer -o asan
lestat@msi:~/Projects/unix-shell/experiments/memory$ ./asan
123
123
123
final
=================================================================
==15599==ERROR: AddressSanitizer: heap-use-after-free on address 0x50c000000040 at pc 0x7091c3c6cf85 bp 0x7ffe50159620 sp 0x7ffe50158dc8
READ of size 2 at 0x50c000000040 thread T0
    #0 0x7091c3c6cf84 in puts ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:1236
    #1 0x5af2b232c6be in main /home/lestat/Projects/unix-shell/experiments/memory/main-print-after-free.c:29
    #2 0x7091c302a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x7091c302a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #4 0x5af2b232c2e4 in _start (/home/lestat/Projects/unix-shell/experiments/memory/asan+0x22e4) (BuildId: fc49adb17ae61bea0b088ccac0df94bf238d71d6)

0x50c000000040 is located 0 bytes inside of 120-byte region [0x50c000000040,0x50c0000000b8)
freed by thread T0 here:
    #0 0x7091c3cfc4d8 in free ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:52
    #1 0x5af2b232c67b in main /home/lestat/Projects/unix-shell/experiments/memory/main-print-after-free.c:28
    #2 0x7091c302a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x7091c302a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #4 0x5af2b232c2e4 in _start (/home/lestat/Projects/unix-shell/experiments/memory/asan+0x22e4) (BuildId: fc49adb17ae61bea0b088ccac0df94bf238d71d6)

previously allocated by thread T0 here:
    #0 0x7091c3cfd9c7 in malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:69
    #1 0x7091c3086e64 in __GI___getdelim libio/iogetdelim.c:65
    #2 0x7091c3c9a102 in getline ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:5317
    #3 0x5af2b232c642 in main /home/lestat/Projects/unix-shell/experiments/memory/main-print-after-free.c:18
    #4 0x7091c302a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #5 0x7091c302a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #6 0x5af2b232c2e4 in _start (/home/lestat/Projects/unix-shell/experiments/memory/asan+0x22e4) (BuildId: fc49adb17ae61bea0b088ccac0df94bf238d71d6)

SUMMARY: AddressSanitizer: heap-use-after-free ../../../../src/libsanitizer/sanitizer_common/sanitizer_common_interceptors.inc:1236 in puts
Shadow bytes around the buggy address:
  0x50bffffffd80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x50bffffffe00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x50bffffffe80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x50bfffffff00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x50bfffffff80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x50c000000000: fa fa fa fa fa fa fa fa[fd]fd fd fd fd fd fd fd
  0x50c000000080: fd fd fd fd fd fd fd fa fa fa fa fa fa fa fa fa
  0x50c000000100: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x50c000000180: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x50c000000200: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x50c000000280: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==15599==ABORTING
```

So the pointer was in fact pointing to a block that is now presumably owned by somebody else.
What I can get out of this is that I underestimated how powerful ASan and Valgrind are, I did not expect to flag the actual error by name here: heap-use-after-free 
