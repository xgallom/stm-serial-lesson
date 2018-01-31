#ifndef DEFINES_H_
#define DEFINES_H_

#include "discover_board.h"

#define BUF_SIZE 256

#ifdef bool
#undef bool
#endif
#define bool uint8_t


#define false FALSE
#define true TRUE

#ifndef STM32
#define STM32
#endif

#endif
