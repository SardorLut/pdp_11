#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"

Command cmd[] = {
        {0070000, 0010000, "mov", HAS_SS | HAS_DD, do_mov},
        {0170000, 0060000, "add", HAS_DD | HAS_SS, do_add},
        {0177000, 0077000, "sob", HAS_R | HAS_NN, do_sob},
        {0077700, 0005000, "clr", HAS_DD, do_clr},
        {0177777, 0000257, "ccc", 0, do_ccc},
        {0177777, 0000277, "scc", 0, do_scc},
        {0177777, 0000270, "sen", 0, do_sen},
        {0177777, 0000250, "cln", 0, do_cln},
        {0177777, 0000244, "clz", 0, do_clz},
        {0177777, 0000242, "clv", 0, do_clv},
        {0177777, 0000241, "clc", 0, do_clc},
        {0177777, 0000250, "cln", 0, do_cln},
        {0177777, 0000264, "sez", 0, do_sez},
        {0177777, 0000262, "sev", 0, do_sev},
        {0177777, 0000261, "sec", 0, do_sec},
        {0177400, 0000400, "br", HAS_XX, do_br},
        {0177400, 0001400, "beq", HAS_XX, do_beq},
        {0177777, 000000,  "halt", 0, do_halt},
        {0000000, 000000,  "unknown command", 0, do_nothing}
};

extern word reg[];
extern byte mem[];
extern Arg ss, dd, b_flag, r, nn, xx;
extern char N_flag, Z_flag, V_flag, C_flag;

Arg get_modereg(word w) {
    Arg res;
    int r = w & 7;
    int mode = (w >> 3) & 7;
    switch (mode) {
        case 0:             // Rn
            res.adr = r;
            res.val = reg[r];
            trace("R%o ", r);
            break;
        case 1:             //  (Rn)
            res.adr = reg[r];
            res.val = w_read(res.adr);
            trace("(R%o) ", r);
            break;
        case 2:
            res.adr = reg[r];
            res.val = w_read(res.adr);      //  #nn
            if (b_flag.val && r != 7 && r != 6)
                reg[r] += 1;
            else
                reg[r] += 2;
            if (r == 7)
                trace("#%o ", res.val);
            else
                trace("(R%o)+ ", r);
            break;
        case 3:
            res.adr = reg[r];
            res.adr = w_read(res.adr);      //  @(Rn)+
            word tmp_for_trace = res.adr;
            res.val = w_read(res.adr);
            reg[r] += 2;
            if (r == 7)
                trace("@#%o ", tmp_for_trace);
            else
                trace("@(R%o)+ ", r);
            break;
        case 4:
            if (b_flag.val && r != 7 && r != 6)
                reg[r] -= 1;
            else
                reg[r] -= 2;
            res.adr = reg[r];
            res.val = w_read(res.adr);      //  -(Rn)
            trace("-(R%o) ", r);
            break;
        case 5:
            reg[r] -= 2;
            res.adr = reg[r];
            res.adr = w_read(res.adr);      //  @-(Rn)
            res.val = w_read(res.adr);
            trace("@-(R%o) ", r);
            break;
        default:
            trace("mode %o not implemented yet", mode);
            exit(1);
    }
    return res;
}

void set_C(int val) {
    if ((val >> LEN_WORD) && (val > 0))
        do_sec();
    else
        do_clc();
}

void set_NZ(word val) {
    if (val == 0) {
        do_cln();
        do_sez();
    } else if ((val >> (LEN_WORD - 1)) & 1) {
        do_sen();
        do_clz();
    } else {
        do_cln();
        do_clz();
    }
}

void do_halt() {
    trace("\n----------halted------------\n");
    print_reg();
    exit(0);
}

void do_mov() {
    if (b_flag.val) {
        b_write(dd.adr, (byte) ss.val);
        set_NZ(ss.val << LEN_BYTE);
    } else {
        w_write(dd.adr, ss.val);
        set_NZ(ss.val);
    }
}
void do_add() {
    word res = dd.val + ss.val;
    w_write(dd.adr, res);
    set_C((int) ss.val + (int) dd.val);
    set_NZ(res);
}

void do_nothing() {
    trace("unknown command\n");
    print_reg();
    exit(1);
}

void do_sob() {
    if ( --reg[r.val] != 0) {
        pc = pc - 2 * nn.val;
    }
}

void do_clr() {
    if (b_flag.val) {
        b_write(dd.adr, 0);
    } else {
        w_write(dd.adr, 0);
    }
}

void do_ccc() {
    do_cln();
    do_clz();
    do_clv();
    do_clz();
}

void do_scc() {
    do_sen();
    do_sez();
    do_sev();
    do_sec();
}

void do_cln() {
    N_flag = 0;
}

void do_clz() {
    Z_flag = 0;
}

void do_clv() {
    V_flag = 0;
}

void do_clc() {
    C_flag = 0;
}

void do_sen() {
    N_flag = 1;
}

void do_sez() {
    Z_flag = 1;
}

void do_sev() {
    V_flag = 1;
}

void do_sec() {
    C_flag = 1;
}

void do_br() {
    if (xx.val >> (LEN_BYTE - 1))
        xx.val = xx.val - 0400;
    pc = pc + xx.val * 2;
}

void do_beq() {
    if (Z_flag)
        do_br();
}

void run() {
    trace("----------running----------\n");
    pc = 01000;
    int i;
    while (1) {
        word w = w_read(pc);
        trace("%06o %06o ", pc, w);
        pc += 2;
        i = 0;
        while ((w & cmd[i].mask) != cmd[i].opcode) {
            i++;
        }
        trace("%s   ", cmd[i].name);
        if (((w >> POSITION_B) == 1) && ((cmd[i].mask >> POSITION_B) == 0))
            b_flag.val = 1;
        else
            b_flag.val = 0;
        if (cmd[i].params & HAS_SS) {
            ss = get_modereg(w>>LEN_DD);
        }
        if (cmd[i].params & HAS_DD) {
            dd = get_modereg(w);
            w = w >> LEN_DD;
            if (cmd[i].params & HAS_SS) {
                w = w >> LEN_SS;
            }
        }
        if (cmd[i].params & HAS_NN) {
            nn.val = (w & 077);
            w = w >> LEN_NN;
            trace("%o ", pc - 2 * nn.val);
        }
        if (cmd[i].params & HAS_R) {
            r.val = (w & 07);
            w = w >> LEN_R;
            trace("R%o ", r.val);
        }
        if (cmd[i].params & HAS_XX) {
            xx.val = (w & 0377);
            trace("%o ", pc + xx.val * 2);
        }
        cmd[i].do_func();
        trace("\n");
    }
}