#include "delay.h"
#include <unistd.h>


void mdelay(uint32_t msecs) {
    usleep(msecs * 1000);
}
