main: main.c 
	cc main.c -std=c17 -Wall -Wextra -Wpedantic -g -o main

asan: main.c
	cc main.c -std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined -fno-omit-frame-pointer -o asan


