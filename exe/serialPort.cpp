#include "SerialPort.h"  
 
CSerialPort::CSerialPort(void)
{
    m_hComm = INVALID_HANDLE_VALUE;
}
 
CSerialPort::~CSerialPort(void)
{
    if (m_hComm != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hComm);
        m_hComm = INVALID_HANDLE_VALUE;
    }
}
 
//初始化串口函数
bool CSerialPort::openPort(char *port, unsigned int baud, char parity, unsigned int databits, unsigned int stopsbits)
{
    /** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */
    char tmp_buf[50];

	//大于COM9的串口要加\\.\COMxx
	if(port[4] != 0)	
	{
		tmp_buf[0] = '\\';
		tmp_buf[1] = '\\';
		tmp_buf[2] = '.';
		tmp_buf[3] = '\\';
		tmp_buf[4] = 'C';
		tmp_buf[5] = 'O';
		tmp_buf[6] = 'M';
		tmp_buf[7] = port[3];
		tmp_buf[8] = port[4];
		tmp_buf[9] = 0;
	}
	else
	{
		tmp_buf[0] = 'C';
		tmp_buf[1] = 'O';
		tmp_buf[2] = 'M';
		tmp_buf[3] = port[3];
		tmp_buf[4] = 0;
	}
    /** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */
    m_hComm = CreateFileA(tmp_buf,		  /** 设备名,COM1,COM2等 */
        GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */
        0,                            /** 共享模式,0表示不共享 */
        NULL,                         /** 安全性设置,一般使用NULL */
        OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */
        0,
        0);

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

    sprintf_s(tmp_buf, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);
 
    /** 设置串口的超时时间,均设为0,表示不使用超时限制 */
    COMMTIMEOUTS  CommTimeouts;
    CommTimeouts.ReadIntervalTimeout = 0;
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.ReadTotalTimeoutConstant = 0;
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutConstant = 0;

    if(SetCommTimeouts(m_hComm, &CommTimeouts) == false)
	{
		return false;
	}
 
    DCB  dcb;

    //// 将ANSI字符串转换为UNICODE字符串  
    //DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
    //wchar_t *pwText = new wchar_t[dwNum];
    //if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
    //{
    //    bIsSuccess = TRUE;
    //}
 
    /** 获取当前串口配置参数,并且构造串口DCB参数 */
    if(GetCommState(m_hComm, &dcb) && BuildCommDCB(tmp_buf, &dcb) == false)
	{
		return false;
	}
    /** 开启RTS flow控制 */
    //dcb.fRtsControl = RTS_CONTROL_ENABLE;
 
    /** 释放内存空间 */
    /*delete[] pwText;*/
    
    /** 使用DCB参数配置串口状态 */
    if(SetCommState(m_hComm, &dcb) == false)
	{
		return false;
	}
 
    /**  清空串口缓冲区 */
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    return true;
}
 
//获取串口缓冲区的字节数
UINT CSerialPort::GetBytesInCOM()
{
    DWORD dwError = 0;  /** 错误码 */
    COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */
    memset(&comstat, 0, sizeof(COMSTAT));
 
    UINT BytesInQue = 0;
    /** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */
    if (ClearCommError(m_hComm, &dwError, &comstat))
    {
        BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */
    }
 
    return BytesInQue;
}

//读取串口接收缓冲区中一个字节的数据
bool CSerialPort::ReadChar(unsigned char &cRecved)
{
    BOOL  bResult = TRUE;
    DWORD BytesRead = 0;
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
 
    /** 从缓冲区读取一个字节的数据 */
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
    if ((!bResult))
    {
        /** 获取错误码,可以根据该错误码查出错误原因 */
        DWORD dwError = GetLastError();
 
        /** 清空串口缓冲区 */
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
 
        return false;
    }
 
 
    return (BytesRead == 1);
}
 
// 向串口写数据, 将缓冲区中的数据写入到串口
bool CSerialPort::WriteData(unsigned char *pData,  int length)
{
    BOOL   bResult = TRUE;
    DWORD  BytesToSend = 0;
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
 
 
    /** 向缓冲区写入指定量的数据 */
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
    if (!bResult)
    {
        DWORD dwError = GetLastError();
        /** 清空串口缓冲区 */
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
 
        return false;
    }
 
    
    return true;
}