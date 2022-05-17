#ifndef MY_PDP_11_EMULATOR_DO_FUNCTIONS_H
#define MY_PDP_11_EMULATOR_DO_FUNCTIONS_H
#define pc reg[7]
#define LEN_DD 6
#define LEN_SS 6
#define LEN_NN 6
#define LEN_R 3
#define LEN_XX 8
#define HAS_SS (1<<1)
#define HAS_DD (1)
#define HAS_R (1<<3)
#define HAS_NN (1<<2)
#define HAS_XX (1<<4)
#define POSITION_B 15
#define LEN_BYTE 8
#define LEN_WORD 16
#define MEMSIZE 64*1024
#define LEN_BYTE 8
#define LEN_WORD 16
typedef unsigned char byte;
typedef unsigned short int word;
typedef word adr;


typedef struct {
    word mask;
    word opcode;
    char * name;
    word params;
    void (*do_func) (void);
} Command;

typedef struct
{
    word val;    // значение аргумента
    word adr;    // адрес аргумента
} Arg;

void do_halt();
void do_mov();
void do_add();
void do_nothing();
void do_sob();
void do_clr();

void do_ccc();
void do_scc();

void do_tst();
void do_cmp();

void do_cln();
void do_clz();
void do_clv();
void do_clc();

void do_sen();
void do_sez();
void do_sev();
void do_sec();

void set_C(int val);
void set_NZ(word val);

void do_br();
void do_beq();

void run();
void trace(char * format, ...);
byte b_read (adr a);
word w_read (adr a);
void b_write (adr a, byte val);
void w_write (adr a, word val);
void print_reg();
#endif //MY_PDP_11_EMULATOR_DO_FUNCTIONS_H
