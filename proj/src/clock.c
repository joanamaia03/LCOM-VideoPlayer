#include "clock.h"

uint16_t rseg;
uint16_t lseg;
uint16_t rmin;
uint16_t lmin;

int(inicialize_clock)(){
    rseg,lseg,rmin,lmin = 0;
    return 0;
}
int(update_clock)(){
    int v = 0;
    if(rseg==9){
        rseg = 0;
        lseg++;
        v = 1;
    }
    if(lseg==6){
        lseg = 0;
        rseg = 0;
        rmin++;
        v++;
    }
    if(rmin==9){
        lseg = 0;
        rseg = 0;
        rmin = 0;
        lmin++;
        v++;
    }
    if(v!=0){
        lseg++;
    }
    return 0;
}