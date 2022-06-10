#include "funseg.h"

#pragma codeseg FUN_SEG

//被copy到ram从ram运行的代码放在以下
void _flash_write(uint8_t *buf, uint8_t *addr)
{
  uint8_t i;
  FLASH->CR2 |= 0x01;
  for(i = 0;i < 128;i++)
  {
    *addr++ = *buf++;
  }
}