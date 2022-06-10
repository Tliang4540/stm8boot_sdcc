#include "stm8l15x.h"
#include "funcopy.h"

const char __at(0x8200-6) _version[] = "V0.01\0";

//sdcc���Զ������ж�������ʹ�������������SDCC���ɵ�
const unsigned long __at(0x8004) _vect[] = {
  0x82008204,
  0x82008000,
  0x8200820C,
  0x82008210,
  0x82008214,
  0x82008218,
  0x8200821C,
  0x82008220,
  0x82008224,
  0x82008228,
  0x8200822C,
  0x82008230,
  0x82008234,
  0x82008238,
  0x8200823C,
  0x82008240,
  0x82008244,
  0x82008248,
  0x8200824C,
  0x82008250,
  0x82008254,
  0x82008258,
  0x8200825C,
  0x82008260,
  0x82008264,
  0x82008268,
  0x8200826C,
  0x82008270,
  0x82008274,
  0x82008278,
  0x8200827C
};
//�������һ���ж�,SDCC�����������ж�������
void reserved_interrupt29(void) __interrupt(29){}

//��STM8L151K4��ʵ�⣬���ڴ�ģ��Ӧ��ͨ�ã�һ��128�ֽڣ�ʹ��С����ڴ�ģ����Ҫ�޸�
//device information
#define  INIT_PAGE      0x04
#define  BLOCK_BYTES    128
#define	 BLOCK_SHIFT		7
#define  FLASH_START    0x008000
#define  FLASH_END      0x00BFFF

//cmd code
#define BOOT_OK	        0xa0
#define BOOT_ERR        0xa1
#define BOOT_HEAD       0xa5
#define BOOT_READ       0xa6
#define BOOT_WRITE      0xa7
#define BOOT_VERIFY     0xa8
#define BOOT_GO         0xa9
/*
void UART1_SendB(u8 ch)
{
    while (!(USART1->SR & (u8)USART_FLAG_TXE));
    USART1->DR = ch;    
}*/

u8 UART1_RcvB(void)
{
     while(!(USART1->SR & (u8)USART_FLAG_RXNE));
     return ((uint8_t)USART1->DR);
}
u16 tryCnt = 65535;
u8 ch, page;
u8 verify;
u8* addr;
uint8_t fun_buf[256];
uint8_t buf[BLOCK_BYTES];
void main(void)
{
  u8 i = 10;  //0.5s
  CLK->CKDIVR = (uint8_t)(CLK_SYSCLKDiv_1);

  //�������ڷ���
  /*GPIOC->DDR |= GPIO_Pin_3;
  GPIOC->ODR |= GPIO_Pin_3;
  GPIOC->CR1 |= (GPIO_Pin_2 | GPIO_Pin_3);*/

  CLK->PCKENR1 = 1 << 5;
  USART1->CR1 = 0x00;
  USART1->CR3 = 0x00;

  USART1->BRR2 = 0x0B;
  USART1->BRR1 = 0x08;

  USART1->CR2 = 0x0C;

  while(i)
  {
    if(USART1->SR & (u8)USART_FLAG_RXNE)    //wait for head 
    {
        ch = (uint8_t)USART1->DR;
        if(ch == BOOT_HEAD) break;
    }
    tryCnt--;
    if(tryCnt == 0) i--;
  }

  if(i == 0)
  {
    __asm__("JP 0x8200");
  }
  else
  {
    funcopy(fun_buf);       //���ƺ�����RAM
    FLASH->PUKR = 0x56;
    FLASH->PUKR = 0xAE;
    USART1->DR = (0xa0|INIT_PAGE);
    while(1)
    {
      ch = UART1_RcvB();
      switch(ch)
      {
      case BOOT_GO:
          FLASH->IAPSR &= 0xfd;
          __asm__("JP 0x8200");
          break;
      case BOOT_WRITE:
          page = UART1_RcvB();
          addr = (u8*)(FLASH_START + (page << BLOCK_SHIFT));
          verify = 0;
          for(i = 0; i < BLOCK_BYTES; i++)   
          {
              buf[i] = UART1_RcvB();
              verify += buf[i];
          }
          if(verify == UART1_RcvB())  //ͨ��У��ɹ�
          {
              (*(flash_write)fun_buf)(buf, addr);   //����RAM�е�дflash����
              //verify flash 
              for(i = 0; i < BLOCK_BYTES; i++)   
              {
                  verify -= *addr++;
              }
              if(verify == 0)  //д��У��ɹ�
              {
                  USART1->DR = (BOOT_OK);
                  break;
              }
              //else,д��У��ʧ�ܣ�������flash��
          }
      default: //����У��ʧ�ܵ����Ҳ�ᵽ������
          USART1->DR = (BOOT_ERR);
          break;
      }
    }
  }
}
