#include"delay.h"

void DelayXms(unsigned int xms) {
    unsigned int i, j;
    for (i = 0; i < xms; i++) {
        for (j = 0; j < 124; j++);
    }
}