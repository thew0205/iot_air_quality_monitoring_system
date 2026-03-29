#ifndef __TIME_H__
#define __TIME_H__

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
    uint32_t iaq_millis();
    void iaq_delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif