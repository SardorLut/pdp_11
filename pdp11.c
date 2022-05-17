#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>
#include "pdp11.h"


byte mem[MEMSIZE];
word reg[8];
Arg ss, dd, b_flag, r, nn, xx;
char N_flag, Z_flag, V_flag, C_flag;
int trace_flag = 0;

byte b_read (adr a) {
    if (a <= 7)
        return (byte) reg[a];
    else
        return mem[a];
}

void b_write (adr a, byte val) {
    if (a > 7) {
        mem[a] = val;
    } else {
        if (val >> (LEN_BYTE - 1))
            reg[a] = val + 0xff00;
        else
            reg[a] = val;
    }
}

word w_read(adr a) {
    word w = ((word) mem[a + 1]) << LEN_BYTE;
    w = w | mem[a];
    return w;
}

void w_write (adr a, word val) {
    if (a <= 7) {
        reg[a] = val;
    } else {
        mem[a + 1] = (byte) (val >> LEN_BYTE);
        mem[a] = (byte) (val);
    }
}

void print_bin(unsigned char x, char end){
    char a[LEN_BYTE+1] = {};

    for(int i = LEN_BYTE-1; x > 0 && i >= 0; i--) {
        a[i] = x % 2;
        x = x / 2;
    }
    for(int i = 0; i < LEN_BYTE; i++)
        printf("%d", a[i]);
    if (end)
        printf("%c", end);
}

void load_file(const char *file) {
    adr a, n;
    byte k;

    FILE *f = fopen(file, "r");
    if (f == NULL) {
        perror(file);
        exit(1);
    }

    while (fscanf(f, "%hx%hx", &a, &n) != EOF) {
        for (adr i = 0; i < n; i++) {
            fscanf(f, "%hhx", &k);
            b_write(a + i, k);
        }
    }
    fclose(f);
}


void mem_dump(adr start, word n) {
    for (int i = 0; i < n; i += 2) {
        printf("%06o : %06o\n", start + i, w_read(start + i));
    }
}

void usage(const char *progname) {
    fprintf(stderr, "USAGE: %s [-t] FILE\n ", progname);
    fprintf(stderr, "\t-t - trace on\n ");
}

void trace(char * format, ...) {
    if (trace_flag) {
        va_list ptr;
        va_start(ptr, format);
        vfprintf(stderr, format, ptr);
        va_end(ptr);
    }
}

void print_reg() {
    for (int i = 0; i < 8; i++) {
        trace("r%o = %06o ;\n", i, reg[i]);
    }
}

int main(int argc, char * argv[]) {
    if (argc == 1) {
        usage(argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 't')
                trace_flag = 1;
            else {
                usage(argv[0]);
                return 1;
            }
        } else
            load_file(argv[i]);
    }
    trace("\n");
    run();
    return 0;
}