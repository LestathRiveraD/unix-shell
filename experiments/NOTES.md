## Make:
    1. A MakeFile is similar in spirit to a 'package.json' file (not literally, but is a nice comparison)
    2. It tracks dependencies and automatically runs commands.
    3. The basic syntax: file: prerequisites\n commands
    4. If file does not exist OR any of the prerequisites have changed since file was last compiled, it recompiles, else, it does not recompile. So, it compares who changed last to decide whether to recompile or not.

### Why can't I run Valgrind on the ASan build?
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

##  Memory

    1. malloc() allocates size bytes of memory and returns a pointer to the allocated memory 
    2. realloc() receives a pointer and resizes the memory block to byte size. If the new size is larger than the old size, the added memory will not be initialized.
    3. strdup() duplicates a string and returns a pointer to the duplicated string 
    4. getline() receives a pointer to a buffer pointer, a size pointer n to the buffer's allocated size, and a stream. It reads a line of text from stream into the buffer. If not enough memory is allocated for the string, getlines reallocs the memory block

### getline takes a char ** and a size_t* . Why pointers to pointers, and who owns the buffer?

    An array is just a contiguous block of bytes. We need a pointer to the first element to implement this behavior. A string is a char*. So, for a string array, we want a pointer to the first element of a char*, that's why we need a char**.
    The buffer is owned by the lineptr* that points to the memory allocated by getline, so every char* has its own allocated piece of memory and should be freed before terminating the program
    
### Valgrind checks    

```bash
lestat@msi:~/Projects/unix-shell/experiments/memory$ valgrind ./main --leak-check=full < /etc/passwd
==14090== Memcheck, a memory error detector
==14090== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==14090== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==14090== Command: ./main --leak-check=full
==14090== 
55: libvirt-dnsmasq:x:125:130:Libvirt Dnsmasq,,,:/var/lib/libvirt/dnsmasq:/usr/sbin/nologin
54: libvirt-qemu:x:64055:993:Libvirt Qemu,,,:/var/lib/libvirt:/usr/sbin/nologin
53: swtpm:x:124:128:virtual TPM software stack,,,:/var/lib/swtpm:/bin/false
52: mssql:x:997:984::/var/opt/mssql:/bin/bash
51: postgres:x:123:126:PostgreSQL administrator,,,:/var/lib/postgresql:/bin/bash
50: nvidia-persistenced:x:122:124:NVIDIA Persistence Daemon,,,:/nonexistent:/usr/sbin/nologin
49: lestat:x:1000:1000:lestat:/home/lestat:/bin/bash
48: nm-openvpn:x:121:122:NetworkManager OpenVPN,,,:/var/lib/openvpn/chroot:/usr/sbin/nologin
47: gdm:x:120:121:Gnome Display Manager:/var/lib/gdm3:/bin/false
46: gnome-initial-setup:x:119:65534::/run/gnome-initial-setup/:/bin/false
45: colord:x:118:120:colord colour management daemon,,,:/var/lib/colord:/usr/sbin/nologin
44: rtkit:x:117:119:RealtimeKit,,,:/proc:/usr/sbin/nologin
43: polkitd:x:987:987:User for polkitd:/:/usr/sbin/nologin
42: gnome-remote-desktop:x:988:988:GNOME Remote Desktop:/var/lib/gnome-remote-desktop:/usr/sbin/nologin
41: hplip:x:116:7:HPLIP system user,,,:/run/hplip:/bin/false
40: cups-browsed:x:115:114::/nonexistent:/usr/sbin/nologin
39: geoclue:x:114:117::/var/lib/geoclue:/usr/sbin/nologin
38: saned:x:113:116::/var/lib/saned:/usr/sbin/nologin
37: fwupd-refresh:x:989:989:Firmware update daemon:/var/lib/fwupd:/usr/sbin/nologin
36: cups-pk-helper:x:112:114:user for cups-pk-helper service,,,:/nonexistent:/usr/sbin/nologin
35: speech-dispatcher:x:111:29:Speech Dispatcher,,,:/run/speech-dispatcher:/bin/false
34: sssd:x:110:113:SSSD system user,,,:/var/lib/sss:/usr/sbin/nologin
33: tcpdump:x:109:112::/nonexistent:/usr/sbin/nologin
32: avahi:x:108:111:Avahi mDNS daemon,,,:/run/avahi-daemon:/usr/sbin/nologin
31: dnsmasq:x:999:65534:dnsmasq:/var/lib/misc:/usr/sbin/nologin
30: whoopsie:x:107:109::/nonexistent:/bin/false
29: kernoops:x:106:65534:Kernel Oops Tracking Daemon,,,:/:/usr/sbin/nologin
28: systemd-oom:x:990:990:systemd Userspace OOM Killer:/:/usr/sbin/nologin
27: tss:x:105:105:TPM software stack,,,:/var/lib/tpm:/bin/false
26: usbmux:x:104:46:usbmux daemon,,,:/var/lib/usbmux:/usr/sbin/nologin
25: uuidd:x:103:103::/run/uuidd:/usr/sbin/nologin
24: systemd-resolve:x:991:991:systemd Resolver:/:/usr/sbin/nologin
23: syslog:x:102:102::/nonexistent:/usr/sbin/nologin
22: messagebus:x:101:101::/nonexistent:/usr/sbin/nologin
21: dhcpcd:x:100:65534:DHCP Client Daemon,,,:/usr/lib/dhcpcd:/bin/false
20: systemd-timesync:x:996:996:systemd Time Synchronization:/:/usr/sbin/nologin
19: systemd-network:x:998:998:systemd Network Management:/:/usr/sbin/nologin
18: nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
17: _apt:x:42:65534::/nonexistent:/usr/sbin/nologin
16: irc:x:39:39:ircd:/run/ircd:/usr/sbin/nologin
15: list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
14: backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
13: www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
12: proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
11: uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
10: news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
9: mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
8: lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
7: man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
6: games:x:5:60:games:/usr/games:/usr/sbin/nologin
5: sync:x:4:65534:sync:/bin:/bin/sync
4: sys:x:3:3:sys:/dev:/usr/sbin/nologin
3: bin:x:2:2:bin:/bin:/usr/sbin/nologin
2: daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
1: root:x:0:0:root:/root:/bin/bash
==14090== 
==14090== HEAP SUMMARY:
==14090==     in use at exit: 0 bytes in 0 blocks
==14090==   total heap usage: 65 allocs, 65 frees, 9,626 bytes allocated
==14090== 
==14090== All heap blocks were freed -- no leaks are possible
==14090== 
==14090== For lists of detected and suppressed errors, rerun with: -s
==14090== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
lestat@msi:~/Projects/unix-shell/experiments/memory$ 
```

```bash
lestat@msi:~/Projects/unix-shell/experiments/memory$ make asan
cc main.c -std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer -o asan
.lestat@msi:~/Projects/unix-shell/experiments/memory$ ./asan 
123
123
123
a
4: a
3: 123
2: 123
1: 123
CTRL + D
```

#### For context:
1. After writing the initial version of this, I wrote a singular free (free(arr)) at the end of the program and compiled it. 
2. Valgrind flagged several bytes leaks. After testing with different inputs, I realized the amount of bytes leaked was proportional to the amount of allocated char*.
3. So I decided to free every char* in arr during the final output pass. 
4. There was a singular block being leaked after compiling, which I realized was the char*s allocated by getline(). After freeing that one too, there were no leaks.
5. ASan flagged something, which turned out to be an unncessary free(arr[size]). I thought it was necessary to free this block because it corresponded to the "separator" of the array, but ASan flagged it so my assumption was wrong.

### Additional experiments:

1. Freeing the same pointer twice:

    According to man: 

    ```bash 
if  ptr  has already been freed, undefined behavior occurs
    ```

    ASAN flagged the double free in my experiment though. See: ./memory/main-free-twice.md

2. What's the difference between "definitely lost" and "still reachable" in Valgrind's report?

    Definitely lost means the allocated memory is unreachable, nobody is pointing at it, whereas still reachable means that block it's still technically reachable by a pointer in a global variable that valgrind flags as reachable. 

3. What goes wrong if you write arr = realloc(arr, n) and realloc fails?

    According to man: 

    ```bash
On error, these functions return NULL and set errno.
    ```
    
    So the pointer `arr` will be set to NULL, and the memory referenced by pointer arr* will be unreachable, effectively causing a memory leak.

## References

1. Valgrind. (2022). *Valgrind User Manual*.
   https://valgrind.org/docs/manual/valgrind_manual.pdf

2. Clang. (n.d.). *AddressSanitizer*.
   https://clang.llvm.org/docs/AddressSanitizer.html

3. Linux manual page: malloc(3)
