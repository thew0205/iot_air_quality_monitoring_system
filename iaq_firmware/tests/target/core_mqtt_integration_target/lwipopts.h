#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// Need more memory for TLS
#ifdef MQTT_CERT_INC
#define MEM_SIZE 8000
#endif

// Generally you would define your own explicit list of lwIP options
// (see https://www.nongnu.org/lwip/2_1_x/group__lwip__opts.html)
//
// This example uses a common include to avoid repetition
#include "lwipopts_examples_common.h"

#define MEMP_NUM_SYS_TIMEOUT        (LWIP_NUM_SYS_TIMEOUT_INTERNAL+1)



// This defaults to 4
#define MQTT_REQ_MAX_IN_FLIGHT 5

#endif
