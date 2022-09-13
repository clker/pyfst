#include <stdio.h>
#include <stdbool.h>
#include "fstapi.h"

#define FST_TS_S    0
#define FST_TS_MS  -3
#define FST_TS_US  -6
#define FST_TS_NS  -9
#define FST_TS_PS  -12

#define MAX_SIGNAL_NUM 100

void* fst;

typedef struct {
    fstHandle signal;
    char * name;
    int bits;
}st_signal;

st_signal signal_list[MAX_SIGNAL_NUM];
int cur_signal_num = 0;
int cur_tick = 0;


void set_timescale(int timescale){
    if( (timescale == FST_TS_S) ||
        (timescale == FST_TS_MS) ||
        (timescale == FST_TS_US) ||
        (timescale == FST_TS_NS) ||
        (timescale == FST_TS_PS) ){
        fstWriterSetTimescale(fst, timescale);
    }
}

void init(char * fn){
    fst =fstWriterCreate(fn, /* use_compressed_hier */ 1);
    fstWriterSetPackType(fst, FST_WR_PT_LZ4); 
    fstWriterSetScope(fst, FST_ST_VCD_PROGRAM, "top", NULL);
    set_timescale(FST_TS_NS);
}

void add_signal(char * name, int bits){
    signal_list[cur_signal_num].name = name;
    signal_list[cur_signal_num].bits = bits;
    signal_list[cur_signal_num].signal = fstWriterCreateVar(fst, FST_VT_VCD_INTEGER, FST_VD_OUTPUT, bits, name, /* alias */ 0);
    cur_signal_num += 1;
}

void change_signal(char *name, uint64_t val){
    int i;
    for(i=0; i<cur_signal_num; i++){
        if(strcmp(name,signal_list[i].name) == 0){
            if(signal_list[i].bits > 32){
                fstWriterEmitValueChange64(fst, signal_list[i].signal, signal_list[i].bits, val);
            }else{
                fstWriterEmitValueChange32(fst, signal_list[i].signal, signal_list[i].bits, (uint32_t)val);
            }
        }
    }
}

void dump(){
    fstWriterEmitTimeChange(fst, cur_tick);
    cur_tick++;
}

void flush(){
    fstWriterFlushContext(fst);
}

void close_wave(){
    fstWriterClose(fst);
}


