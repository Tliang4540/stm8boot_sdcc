///////////////////////
////����ͷ�ļ��Ĵ��룬��Ҫ�Ƕ�����һ����
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

    /** �򴮿�д����
    *
    *  ���������е�����д�뵽����
    *  @param:  unsigned char * pData ָ����Ҫд�봮�ڵ����ݻ�����
    *  @param:  unsigned int length ��Ҫд������ݳ���
    *  @return: bool  �����Ƿ�ɹ�
    *  @note:   length��Ҫ����pData��ָ�򻺳����Ĵ�С
    *  @see:
    */
    bool WriteData(unsigned char *pData, int length);

    /** ��ȡ���ڻ������е��ֽ���
    *
    *
    *  @return: UINT  �����Ƿ�ɹ�
    *  @note:   �����ڻ�������������ʱ,����0
    *  @see:
    */
    UINT GetBytesInCOM();

    /** ��ȡ���ڽ��ջ�������һ���ֽڵ�����
    *
    *
    *  @param:  char & cRecved ��Ŷ�ȡ���ݵ��ַ�����
    *  @return: bool  ��ȡ�Ƿ�ɹ�
    *  @note:
    *  @see:
    */
    bool ReadChar(unsigned char &cRecved);
private:

    /** ���ھ�� */
    HANDLE  m_hComm;
};

#endif //SERIALPORT_H_