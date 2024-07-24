// ContactCardConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "libDSRetransfer600App.h"


unsigned int outputError(unsigned int code)
{
	char output[500] = { 0 };
	int len = 500;
	R600GetErrorOuterInfo(code, output, &len);
	std::cout << "出现错误：返回值" + std::to_string((long long)code) + std::string(output) << std::endl;
	return code;
}

std::string Hex2String__(const char* buffer, int bufferLen)
{
	std::string str;
	if (bufferLen <= 0) return std::string("");
	if (buffer == NULL) return std::string("");
	CStringA tempch;str = "";int f = 0;
	for (int i = 0;i<bufferLen;++i)
	{
		BYTE c = buffer[i];
		tempch.Format("%.2x", c);
		str += tempch;
		f++;
	}
	return str;
}

struct CardRWConfig
{
	int RWNumber;    //读写次数
	int RWMode;      //读写模式0：先进行卡交互再打印（打印位）1：先打印（打印位）再进行卡操作
	//int CardType;    //卡类型--1：非接CPU 2：Type A Mifare S50
	int isAllowPrt;  //是否允许打印 
}RWConfig;

unsigned int PrtIO()
{
	unsigned int RE = 0;
	char enumList[100] = { 0 };
	unsigned int listLen = 100;
	int num = 0;
	//枚举打印机
	RE = R600EnumUsbPrt(enumList, &listLen, &num);
	if (RE) { return outputError(RE); }
	//设置usb超时时间
	RE = R600UsbSetTimeout(3000, 3000);
	if (RE) { return outputError(RE); }
	//选择打印机
	RE = R600SelectPrt(enumList);
	if (RE) { return outputError(RE); }

	return 0;
}

unsigned int Printing()
{
	std::cout << "打印机开始打印" << std::endl;
	unsigned int RE = 0;
	//准备画布
	R600PrepareCanvas(0, 0);  //彩色（YMC）打印模式300*300dpi     单色（K）打印模式300*600dpi
	//添加图片
	R600DrawImage(0, 0, 0, 0, "背景.jpg", true);
	char frontPath[200] = { 0 };
	int frontPathLen = 200;
	R600CommitCanvas(frontPath, &frontPathLen);
	RE = R600PrintDraw(frontPath, 0); //打印正面
	if (RE){ return outputError(RE); }

	////判断打印过程中机器是否出现警告或报错
	//int k = 0;
	//do 
	//{
	//	unsigned int mainStatus = 0;      //主状态
	//	unsigned int errorStatus = 0;     //错误状态
	//	unsigned int warningStatus = 0;   //警告状态
	//	R600QueryPrtStatus(0, 0, 0, &mainStatus, 0, &errorStatus, &warningStatus, 0, 0);
	//	if(errorStatus != 3001) //打印机报错
	//	{
	//		std::cout << "打印机报错---请清缓存重启" << std::endl;
	//		return 1;
	//	}
	//	if(mainStatus == 1002) //正常状态
	//	{
	//		break;
	//	}
	//	Sleep(800);
	//	k++;
	//} while (k < 35);

	return RE;
}

unsigned int ReaderCPURW()
{
	unsigned int RE = 0;
	int Type = 0;
	char pResetInfo[130] = { 0 };
	unsigned char pResetInfolen = 130;		
	byte slotNum = 0xc;//主卡座
	RE = R600CPUCardReset(slotNum, pResetInfo, &pResetInfolen, 0, 0);  //接触式cpu卡复位
	if(RE) { return outputError(RE); }

	unsigned char pSApdu[5] = { 0x00, 0x84, 0x00, 0x00, 0x04 };  //发送交互指令
	char pRApdu[130] = { 0 };  //接收交互指令
	unsigned char pRApduLen = 130;  //允许接收长度
	RE = R600CPUCardExchangeAPDU(slotNum, pSApdu, 5, pRApdu, &pRApduLen);  //接触式cpu卡交互
	if(RE) 
	{ 
		return outputError(RE);
	}
	else
	{
		//控制台输出信息		
		std::cout << "接触式CPU复位信息： " + std::string(pResetInfo) << std::endl;
		std::cout << "发送交互指令：0x00, 0x84, 0x00, 0x00, 0x04" << std::endl;
		std::string infoRecv = Hex2String__(pRApdu, pRApduLen);
		std::cout << "接触式CPU交互返回信息： " + infoRecv << std::endl;
		return 0;
	}	
}


unsigned int Step1RWCard_Step2Print()
{
Step1RWCard_Step2Print:
	std::cout << "\n" << std::endl;
	std::cout << "进卡到接触式读卡器接触位" << std::endl;

	unsigned int RE = 0;
	//判断打印机内部（打印位）是否有卡
	int flagPrt = 0; 
	RE = R600GetCardPos(&flagPrt);
	if(RE) { return outputError(RE); }
	if(flagPrt != 0) 
	{
		std::cout << "打印位有卡，正在左侧排卡" << std::endl;
		RE = R600CardEject(0); //排卡
		if(RE) { return outputError(RE); }
	}

	//判断打印机卡槽是否空
	int feederIsNoEmpty = 0;
	RE = R600IsFeederNoEmpty(&feederIsNoEmpty);
	if(RE) { return outputError(RE); }
	if(!feederIsNoEmpty) 
	{ 
		std::cout << "卡槽为空，请装卡" << std::endl; 
		system("pause");
		goto Step1RWCard_Step2Print;
	}

	RE = R600CardInject(6); //进卡到接触式读卡器接触位
	if(RE) { return outputError(RE); }

	//读写卡
	ReaderCPURW();

	//移卡到打印位
	std::cout << "\n移卡到打印位" << std::endl;
	RE = R600CardMove(4);
	if(RE) { return outputError(RE); }

	//是否打印
	if(RWConfig.isAllowPrt)
	{
		RE = Printing();
		if(RE) { return outputError(RE); }
	}

	//左侧排卡
	std::cout << "\n左侧排卡" << std::endl;

	RE = R600CardEject(0);
	if(RE) { return outputError(RE); }

	return 0;
}

unsigned int Step1Print_Step2RWCard()
{
Step1Print_Step2RWCard:
	unsigned int RE = 0;
	//判断打印机内部（打印位）是否有卡

	int flagPrt = 0; 
	RE = R600GetCardPos(&flagPrt);
	if(RE) { return outputError(RE); }
	if(flagPrt != 0) 
	{
		std::cout << "打印位有卡，正在左侧排卡" << std::endl;
		RE = R600CardEject(0);
		if(RE) { return outputError(RE); }
	}

	//判断打印机卡槽是否空
	int feederIsNoEmpty = 0;
	RE = R600IsFeederNoEmpty(&feederIsNoEmpty);
	if(RE) { return outputError(RE); }
	if(!feederIsNoEmpty) 
	{ 
		std::cout << "卡槽为空，请装卡" << std::endl; 
		system("pause");
		goto Step1Print_Step2RWCard;
	}

	std::cout << "\n进卡到打印位" << std::endl;
	RE = R600CardInject(0); //进卡到打印位置
	if(RE) { return outputError(RE); }

	//是否打印
	if(RWConfig.isAllowPrt)
	{
		RE = Printing();
		if(RE) { return outputError(RE); }
	}

	std::cout << "移卡到接触式读卡器接触位" << std::endl;
	RE = R600CardMove(6); //移卡
	if(RE) { return outputError(RE); }

	//读写卡
	ReaderCPURW();

	//左侧排卡
	std::cout << "\n左侧排卡" << std::endl;	

	RE = R600CardEject(0);
	if(RE) { return outputError(RE); }

	return 0;
}

unsigned int CardOperation()
{
	unsigned int RE = 0;
	int i = 0;
	while(i < RWConfig.RWNumber)
	{
		if(RWConfig.RWMode == 0)
		{
			RE = Step1RWCard_Step2Print(); 
			if (RE) { return RE; }
		}
		else if(RWConfig.RWMode == 1)
		{
			RE = Step1Print_Step2RWCard();
			if (RE) { return RE; }
		}
		i++;
	}
	return 0;
}

int main()
{
	unsigned int RE = 0;
	//初始化动态库
	RE = R600LibInit();
	if (RE) { return outputError(RE); }

	int input;
	std::cout << "输入读写次数--" << std::endl;
	std::cin >> input;
	RWConfig.RWNumber = input;

	//std::cout << "卡类型--1：非接CPU 2：Type A Mifare S50" << std::endl;
	//std::cin >> input;
	//RWConfig.CardType = input;

	std::cout << "读写模式--0：先进行卡交互再打印（打印位）1：先打印（打印位）再进行卡操作" << std::endl;
	std::cin >> input;
	RWConfig.RWMode = input;

	std::cout << "是否允许打印--0：否 1：是" << std::endl;
	std::cin >> input;
	RWConfig.isAllowPrt = input;

Restart:
	//打印机IO操作（包括枚举打印机、设置usb超时时间、选择打印机）
	PrtIO();

	//模拟打印和非接读卡器读写操作
	RE = CardOperation();

	//判断是否机器故障
	unsigned int errorStatus = 0;     //错误状态
	if(RE)
	{
		//当接口返回错误值时，判断是否机器故障
		R600QueryPrtStatus(0, 0, 0, 0, 0, &errorStatus, 0, 0, 0);
		if(errorStatus != 3001)
		{
			std::cout << "打印机报错---请清缓存重启,重启完成后" << std::endl;
			system("pause");
			goto Restart;
		}
	}

	//卸载动态库
	RE = R600LibClear();
	if (RE) { return outputError(RE); }
	system("pause");
	return 0;
}

