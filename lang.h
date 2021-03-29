// char *c_compile_argv[] = {"gcc", "-DONLINE_JUDGE", "-O2", "-w", "-fmax-errors=3", "-std=c11", "real_main.c", "main.c", "-lm", "-o", "main", 0};
char *c_compile_argv[] = {"sh", "-c", "gcc -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c11 *.c -lm -o main", 0};
char *cpp_compile_argv[] = {"g++", "-DONLINE_JUDGE", "-O2", "-w", "-fmax-errors=3", "-std=c++17", "main.cpp", "-lm", "-o", "main", 0};

char *c_execution[] = {"./main", 0};
char *cpp_execution[] = {"./main", 0};
char *python3_execution[] = {"/usr/bin/python3", "main.py", 0};
