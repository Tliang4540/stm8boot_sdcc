///////////////////////
////这是头文件的代码，主要是定义了一个类
///////////////////////////////

#ifndef SERIALPORT_H_
#define SERIALPORT_H_
#include <string>
#include <iostream>
#include <Windows.h>
using namespace std;

class CSerialPort
{
public:
    CSerialPort(void);
    ~CSerialPort(void);

public:

    bool openPort(char *port, UINT  baud, char  parity, UINT  databits, UINT  stopsbits);

    /** 向串口写数据
    *
    *  将缓冲区中的数据写入到串口
    *  @param:  unsigned char * pData 指向需要写入串口的数据缓冲区
    *  @param:  unsigned int length 需要写入的数据长度
    *  @return: bool  操作是否成功
    *  @note:   length不要大于pData所指向缓冲区的大小
    *  @see:
    */
    bool WriteData(unsigned char *pData, int length);

    /** 获取串口缓冲区中的字节数
    *
    *
    *  @return: UINT  操作是否成功
    *  @note:   当串口缓冲区中无数据时,返回0
    *  @see:
    */
    UINT GetBytesInCOM();

    /** 读取串口接收缓冲区中一个字节的数据
    *
    *
    *  @param:  char & cRecved 存放读取数据的字符变量
    *  @return: bool  读取是否成功
    *  @note:
    *  @see:
    */
    bool ReadChar(unsigned char &cRecved);
private:

    /** 串口句柄 */
    HANDLE  m_hComm;
};

#endif //SERIALPORT_H_