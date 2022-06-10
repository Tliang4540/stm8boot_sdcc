## 项目简介 ##
  1.使用SDCC生成BOOT程序，BOOT可以使用串口烧录程序  
  2.移植[Zepan][1] 的hairBoot程序，小于512字节  
  3.在STM8L151K4上实测  

## 上位机 ##
  使用MVSC 2012 编写，源码和例程在exe文件夹中  
  在cmd命令行中输入"boot_app COMx filepath"即可进行下载  
  只做了解析hex文件，其他文件暂不支持  
  没做选择内存模型，默认每块128字节  

## 测试1.hex文件 ##
  RTC定时1S中断控制PA6翻转  
  USART1循环打印Tliang  
  源码[stm8_sdcc][3]  

## BOOT源码 ##

```C
#include "stm8l15x.h"
#include "funcopy.h"

const char __at(0x8200-6) _version[] = "V0.01\0";

//sdcc会自动生成中断向量表，使用下面这个覆盖SDCC生成的
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
//定义最后一个中断,SDCC会生成整个中断向量表
void reserved_interrupt29(void) __interrupt(29){}

//在STM8L151K4上实测，中内存模型应该通用，一块128字节，使用小或大内存模型需要修改
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

  //开启串口发送
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
    funcopy(fun_buf);       //复制函数到RAM
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
          if(verify == UART1_RcvB())  //通信校验成功
          {
              (*(flash_write)fun_buf)(buf, addr);   //运行RAM中的写flash函数
              //verify flash 
              for(i = 0; i < BLOCK_BYTES; i++)   
              {
                  verify -= *addr++;
              }
              if(verify == 0)  //写入校验成功
              {
                  USART1->DR = (BOOT_OK);
                  break;
              }
              //else,写入校验失败，可能是flash损坏
          }
      default: //上面校验失败的情况也会到这里来
          USART1->DR = (BOOT_ERR);
          break;
      }
    }
  }
}

```

项目源码：[GitHub][2]

  [1]: https://github.com/Zepan/hairBoot
  [2]: https://github.com/Tliang4540/stm8boot_sdcc
  [3]: https://github.com/Tliang4540/stm8_sdcc