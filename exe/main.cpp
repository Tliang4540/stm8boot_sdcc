#include<iostream>
#include<Windows.h>
#include<string>
#include"serialPort.h"
using namespace std;

#define BLOCK_SIZE		128

int getLine(HANDLE m_hFile, char *buf)
{
	char ch;
	unsigned long size;
	int i = 0;
	while(1)
	{
		ReadFile(m_hFile, &ch, 1, &size, NULL);
		if(size)
		{
			if(((ch == '\r') || (ch == '\n')) && i)	//等于回车或换行，且已经读到数据则认为一行结束
			{
				*buf = 0;
				return i;	//返回读到的字节数
			}
			else if(ch >= ' ')	//有效的字符
			{
				*buf++ = ch;
				i++;
			}
		}
		else
		{
			*buf = 0;
			return i;	//返回读到的字节数
		}
	}
}

class hexline 
{
public:
	hexline(char *line)
	{
		string tmp;
		unsigned short x;
		unsigned char  chksum;
		int i,n;
		tmp = line[0];
		tmp += line[1];
		data_len = stoi(tmp, 0, 16);
		chksum = data_len;

		tmp = line[2];
		tmp += line[3];
		data_addr = stoi(tmp, 0, 16);
		chksum += data_addr;

		tmp = line[4];
		tmp += line[5];
		n = stoi(tmp, 0, 16);
		chksum += n;
		data_addr = (data_addr << 8) | n;

		tmp = line[6];
		tmp += line[7];
		data_type = stoi(tmp, 0, 16);
		chksum += data_type;
		
		data = new unsigned char[data_len];
		for(i = 0,n = 8;i < data_len;i++,n+=2)
		{
			tmp = line[n];
			tmp += line[n+1];
			data[i] = stoi(tmp, 0, 16);
			chksum+=data[i];
		}
		tmp = line[n];
		tmp += line[n+1];
		data_chksum = stoi(tmp, 0, 16);
		chksum = 256-chksum;
		chksum_ok = (data_chksum == chksum);
	}
	~hexline()
	{
		delete[] data;
	}

	unsigned int   data_len;
	unsigned int   data_addr;
	unsigned int   data_type;
	unsigned char   *data;
	unsigned char   data_chksum;
	bool chksum_ok;
};

//解析HEX文件到缓存
int HexToBin(HANDLE m_file, unsigned char *buf)
{
	char line_buf[64];		//hex文件一行好像不会超过44字节
	unsigned int buf_index = 0;
	unsigned int hex_haddr = 0;
	hexline *hexdata = NULL;
	int bin_len = 0;		//bin最长的地址
	while(1)
	{
		if(!getLine(m_file, line_buf))		//获取一列数据
		{
			break;
		}
		if(line_buf[0] != ':')				//第一个字符是‘:’
		{
			break;
		}
		hexdata = new hexline(line_buf+1);	//解析一列数据
		if(hexdata->chksum_ok == false)
		{
			break;
		}
		
		if(hexdata->data_type == 0)	//数据
		{
			buf_index = (hex_haddr + hexdata->data_addr) - 0x8000;
			if((hexdata->data_addr > 0xffff) || (hexdata->data_addr < 0))
			{
				break;
			}
			for(int i = 0;i < hexdata->data_len;i++)
			{
				buf[buf_index++] = hexdata->data[i];
			}
			if(buf_index > bin_len)bin_len = buf_index;
		}
		else if(hexdata->data_type == 1) //文件结束
		{
			delete hexdata;
			return bin_len;
		}
		else if(hexdata->data_type == 4) //设置高16位地址
		{
			hex_haddr = 0;
			for(int i = 0;i < hexdata->data_len;i++)
			{
				hex_haddr |= hexdata->data[i];
				hex_haddr <<= 8;
			}
			hex_haddr <<= 8;
		}
		delete hexdata;
	}
	delete hexdata;
	return -1;
}

int main(int argc, char *argv[])
{
	if(argc < 3)	//三个传参 exe COM file
	{
		cout << "please input COM and filepath. " << endl;
		//system("pause");
		return -1;
	}
	//cout << argv[0] << endl;
	cout << "COM  : " << argv[1] << endl;
	cout << "file : " << argv[2] << endl;
	
	CSerialPort serialPort;

	bool ret = serialPort.openPort(argv[1], 115200, 'N', 8, 1);
	HANDLE m_hFile = CreateFileA(argv[2], GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);

	if(ret == false)
	{
		cout << argv[1] << " open fail." << endl;
		//system("pause");
		return -2;
	}
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		cout << argv[2] << " open fail." << endl;
		//system("pause");
		return -3;
	}
	unsigned char chip_rom[65536] = {0};

	int bin_len = HexToBin(m_hFile, chip_rom);
	if(bin_len < 0)
	{
		cout << "hex file error!" << endl;
		//system("pause");
		return -4;
	}
	cout << "file byte : " << bin_len << endl;

	if(bin_len > 32768) bin_len = 65536;		//对齐到64K
	else if(bin_len > 16384) bin_len = 32768;	//对齐到32K
	else if(bin_len > 8192) bin_len = 16384;	//对齐到16K
	else bin_len = 8192;						//对齐到8K
	/*
	for (int i = 0x8000;i < 0x18000;i += 16)	//显示bin数据
	{
		cout.width(6);
		cout.fill('0');
		cout << hex << i << " : ";
		for(int j = 0; j < 16; j++)
		{
			cout.width(2);
			cout.fill('0');
			cout << hex << (unsigned int)chip_rom[(i-0x8000)+j] << " ";
		}
		cout << endl;
		if((i > 0x8000) && ((i % 1024) == 0))
		{
			system("pause");
		}
	}*/

	cout << "waiting connect..." << endl;
	unsigned char hard = 0xa5;
	unsigned char rece = 0;
	unsigned char page = 0; 
	int error = 0;

	while(1)
	{
		while(serialPort.GetBytesInCOM())
		{
			serialPort.ReadChar(rece);		//清空缓存
		}
		serialPort.WriteData(&hard, 1);
		Sleep(50);
		if(serialPort.GetBytesInCOM())
		{
			if(serialPort.ReadChar(rece))
			{
				if(rece == 0xa4)
				{
					page = rece & 0x0F;
					cout << "downloading..." << endl;
					break;
				}
			}
		}
	}
	
	hard = 0xa7;
	while(1)
	{
		serialPort.WriteData(&hard, 1);
		serialPort.WriteData(&page, 1);
		serialPort.WriteData(&chip_rom[page * BLOCK_SIZE], BLOCK_SIZE);
		rece = 0;
		for(int i = 0;i < BLOCK_SIZE;i++)
		{
			rece += chip_rom[(page * BLOCK_SIZE) + i];
		}
		serialPort.WriteData(&rece, 1);
		while(1)
		{
			Sleep(1);
			if(serialPort.GetBytesInCOM())
			{
				if(serialPort.ReadChar(rece))
				{
					if(rece == 0xa0)
					{
						error = 0;
						bin_len -= BLOCK_SIZE;
						page++;
						if(bin_len == 0)
						{
							cout << "download complete." << endl;
							hard = 0xa9;
							serialPort.WriteData(&hard, 1);
							Sleep(50);
							serialPort.~CSerialPort();
							//system("pause");
							return 0;
						}
					}
					else
					{
						error++;
						if(error > 3)
						{
							cout << "download fail." << endl;
							//system("pause");
							return -5;
						}
					}
					break;
				}
			}
		}
	}

	CloseHandle(m_hFile);
	//system("pause");
	return 0;
}