#ifndef __TIMER_H__
    #define __TIMER_H__
    
    #include "interrupts/isr.h"
    #include "interrupts/port.h"
    #include "utils/types.h"

void init_timer(uint32_t freq);

#endif
