#ifndef _FUNSEG_H_
#define _FUNSEG_H_

#include "stm8l15x.h"

typedef void(*flash_write)(uint8_t *, uint8_t *);
void _flash_write(uint8_t *buf, uint8_t *addr);

#endif