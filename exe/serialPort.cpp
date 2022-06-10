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
 
//��ʼ�����ں���
bool CSerialPort::openPort(char *port, unsigned int baud, char parity, unsigned int databits, unsigned int stopsbits)
{
    /** ��ʱ����,���ƶ�����ת��Ϊ�ַ�����ʽ,�Թ���DCB�ṹ */
    char tmp_buf[50];

	//����COM9�Ĵ���Ҫ��\\.\COMxx
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
    /** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */
    m_hComm = CreateFileA(tmp_buf,		  /** �豸��,COM1,COM2�� */
        GENERIC_READ | GENERIC_WRITE, /** ����ģʽ,��ͬʱ��д */
        0,                            /** ����ģʽ,0��ʾ������ */
        NULL,                         /** ��ȫ������,һ��ʹ��NULL */
        OPEN_EXISTING,                /** �ò�����ʾ�豸�������,���򴴽�ʧ�� */
        0,
        0);

	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

    sprintf_s(tmp_buf, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);
 
    /** ���ô��ڵĳ�ʱʱ��,����Ϊ0,��ʾ��ʹ�ó�ʱ���� */
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

    //// ��ANSI�ַ���ת��ΪUNICODE�ַ���  
    //DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
    //wchar_t *pwText = new wchar_t[dwNum];
    //if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
    //{
    //    bIsSuccess = TRUE;
    //}
 
    /** ��ȡ��ǰ�������ò���,���ҹ��촮��DCB���� */
    if(GetCommState(m_hComm, &dcb) && BuildCommDCB(tmp_buf, &dcb) == false)
	{
		return false;
	}
    /** ����RTS flow���� */
    //dcb.fRtsControl = RTS_CONTROL_ENABLE;
 
    /** �ͷ��ڴ�ռ� */
    /*delete[] pwText;*/
    
    /** ʹ��DCB�������ô���״̬ */
    if(SetCommState(m_hComm, &dcb) == false)
	{
		return false;
	}
 
    /**  ��մ��ڻ����� */
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    return true;
}
 
//��ȡ���ڻ��������ֽ���
UINT CSerialPort::GetBytesInCOM()
{
    DWORD dwError = 0;  /** ������ */
    COMSTAT  comstat;   /** COMSTAT�ṹ��,��¼ͨ���豸��״̬��Ϣ */
    memset(&comstat, 0, sizeof(COMSTAT));
 
    UINT BytesInQue = 0;
    /** �ڵ���ReadFile��WriteFile֮ǰ,ͨ�������������ǰ�����Ĵ����־ */
    if (ClearCommError(m_hComm, &dwError, &comstat))
    {
        BytesInQue = comstat.cbInQue; /** ��ȡ�����뻺�����е��ֽ��� */
    }
 
    return BytesInQue;
}

//��ȡ���ڽ��ջ�������һ���ֽڵ�����
bool CSerialPort::ReadChar(unsigned char &cRecved)
{
    BOOL  bResult = TRUE;
    DWORD BytesRead = 0;
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
 
    /** �ӻ�������ȡһ���ֽڵ����� */
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
    if ((!bResult))
    {
        /** ��ȡ������,���Ը��ݸô�����������ԭ�� */
        DWORD dwError = GetLastError();
 
        /** ��մ��ڻ����� */
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
 
        return false;
    }
 
 
    return (BytesRead == 1);
}
 
// �򴮿�д����, ���������е�����д�뵽����
bool CSerialPort::WriteData(unsigned char *pData,  int length)
{
    BOOL   bResult = TRUE;
    DWORD  BytesToSend = 0;
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }
 
 
    /** �򻺳���д��ָ���������� */
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
    if (!bResult)
    {
        DWORD dwError = GetLastError();
        /** ��մ��ڻ����� */
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
 
        return false;
    }
 
    
    return true;
}