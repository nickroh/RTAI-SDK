// DeCard3In1Console.cpp : 定义控制台应用程序的入口点。
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
	int RWPos;       //读写位置0：3合1接触位 1：3合1非接位 2：3合1磁条位
	int isAllowPrt; //是否允许打印 
}RWConfig;


unsigned int CardRW(int pos)
{
	unsigned int RE = 0;
	switch(pos)
	{
	case 1: //接触式CPU卡
		{
			char pResetInfo[130] = { 0 }; //复位信息
			unsigned char pResetInfolen = 130; //复位信息长度
			RE = R600CPUCardReset(0x0C, pResetInfo, &pResetInfolen, 0, 0); //复位接触式CPU卡
			if(RE) { outputError(RE); }

			unsigned char pSApdu[5] = { 0x00, 0x84, 0x00, 0x00, 0x04 };  //发送交互指令
			char pRApdu[130] = { 0 };  //接收交互指令
			unsigned char pRApduLen = 130;  //允许接收长度
			RE = R600CPUCardExchangeAPDU(0x0C, pSApdu, 5, pRApdu, &pRApduLen);  //cpu卡交互
			if(RE) 
			{ 
				outputError(RE);
			}
			else
			{
				//控制台输出信息
				//std::string info = Hex2String__(pResetInfo, pResetInfolen);
				std::cout << "接触CPU复位信息： " + std::string(pResetInfo) << std::endl;
				std::cout << "发送交互指令：0x00, 0x84, 0x00, 0x00, 0x04" << std::endl;
				std::string infoRecv = Hex2String__(pRApdu, pRApduLen);
				std::cout << "接触CPU交互返回信息： " + infoRecv << std::endl;
			}	
		}
		break;
	case 2: //非接CPU卡或M1卡
		{
			//判断是CPU卡还是M1卡
			int Type = 0;
			RE = R600GetCardType(&Type);
			if(RE) { outputError(RE); }
			if(Type == 0x11) //表示Type A CPU Card
			{
				char pResetInfo[130] = { 0 };
				unsigned char pResetInfolen = 130;
				char pUid[10] = { 0 };
				byte pUidLen = 10;
				RE = R600CPUCardReset(0, pResetInfo, &pResetInfolen, pUid, &pUidLen);  //非接cpu卡复位
				if(RE) { return outputError(RE); }

				unsigned char pSApdu[5] = { 0x00, 0x84, 0x00, 0x00, 0x04 };  //发送交互指令
				char pRApdu[130] = { 0 };  //接收交互指令
				unsigned char pRApduLen = 130;  //允许接收长度
				RE = R600CPUCardExchangeAPDU(0, pSApdu, 5, pRApdu, &pRApduLen);  //非接cpu卡交互
				if(RE) 
				{ 
					outputError(RE);
				}
				else
				{
					//控制台输出信息
					std::cout << "uid序列号： " + std::string(pUid) << std::endl;
					std::cout << "非接CPU复位信息： " + std::string(pResetInfo) << std::endl;
					std::cout << "发送交互指令：0x00, 0x84, 0x00, 0x00, 0x04" << std::endl;
					std::string infoRecv = Hex2String__(pRApdu, pRApduLen);
					std::cout << "非接CPU交互返回信息： " + infoRecv << std::endl;
				}	
			}
			else if(Type == 0x13) //表示Type A Mifare S50
			{
				unsigned char pSnrstr[32] = { 0 };
				RE = R600M1CardGetSN(0x01, pSnrstr); //寻卡，返回序列号
				if(RE) { outputError(RE); }

				RE = R600M1CardPwdAuth(true, 0x00, 0, "FFFFFFFFFFFF"); //密码类型A 验证扇区0 密码FFFFFFFFFFFF 
				if(RE) { outputError(RE); }

				char pData[33] = {0};
				RE = R600M1CardRead(true, 0 * 4 + 0, pData); //读取第0区第0块内容
				if(RE)
				{ 
					outputError(RE);
				}
				else
				{
					//控制台输出
					std::cout << "卡序列号：" + std::string((char*)pSnrstr) << std::endl;
					std::cout << "密码类型A 验证扇区0 密码FFFFFFFFFFFF" << std::endl;
					std::cout << "读取扇区0 第0块内容：" + std::string(pData) << std::endl;
				}
			}
			else
			{
				std::cout << "不是非接CPU或Type A Mifare S50" << std::endl;
			}
		}
		break;
	case 3: //磁条卡
		{
			unsigned char ctimeout = 0;
			unsigned char pTrack1Data[100] = { 0 };
			unsigned int pTrack1Len = 0;
			unsigned char pTrack2Data[100] = { 0 };
			unsigned int pTrack2Len = 0;
			unsigned char pTrack3Data[100] = { 0 };
			unsigned int pTrack3Len = 0;
			RE = R600MAGCardRead(3, pTrack1Data, &pTrack1Len, pTrack2Data, &pTrack2Len, pTrack3Data, &pTrack3Len);  //读取磁条卡
			if(RE) 
			{ 
				outputError(RE); 
			}
			else
			{
				//控制台输出
				std::cout << "磁道1：" + std::string((char*)pTrack1Data) << std::endl;
				std::cout << "磁道2：" + std::string((char*)pTrack2Data) << std::endl;
				std::cout << "磁道3：" + std::string((char*)pTrack3Data) << std::endl;
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

unsigned int Printing()
{
	std::cout << "打印机开始打印" << std::endl;
	unsigned int RE = 0;
	//准备画布
	R600PrepareCanvas(0, 0);  //彩色（YMC）打印模式300*300dpi     单色（K）打印模式300*300dpi
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
	//	R600QueryPrtStatus(0, 0, &mainStatus, 0, &errorStatus, &warningStatus, 0, 0);
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

unsigned int Step1RWCard_Step2Print()
{
Step1RWCard_Step2Print:
	unsigned int RE = 0;
	//判断打印机内部（打印位、读卡器）是否有卡
	int flagReader = 0;
	RE = R600GetCardPos(&flagReader);
	if(RE) { return outputError(RE); }
	if(flagReader == 6)
	{ 
		std::cout << "读卡器有卡，正在前方排卡" << std::endl;
		RE = R600CardEject(0);
		if(RE) { return outputError(RE); }
	}

	int flagPrt = 0; 
	RE = R600GetCardPos(&flagPrt);
	if(RE) { return outputError(RE); }
	if(flagPrt != 0) 
	{
		std::cout << "打印位有卡，正在前方排卡" << std::endl;
		RE = R600CardEject(0); //移卡
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

	RE = R600CardInject(RWConfig.RWPos + 1); //进卡到指定位置
	if(RE) { return outputError(RE); }

	CardRW(RWConfig.RWPos); //读写卡

	//移卡到打印位
	std::cout << "\n移卡到打印位" << std::endl;
	RE = R600CardMove(0);
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
	//判断打印机内部（打印位、读卡器）是否有卡
	int flagReader = 0;
	RE = R600GetCardPos(&flagReader);
	if(RE) { return outputError(RE); }
	if(flagReader == 6)
	{ 
		std::cout << "读卡器有卡，正在前方排卡" << std::endl;
		RE = R600CardEject(0);
		if(RE) { return outputError(RE); }
	}

	int flagPrt = 0; 
	RE = R600GetCardPos(&flagPrt);
	if(RE) { return outputError(RE); }
	if(flagPrt != 0) 
	{
		std::cout << "打印位有卡，正在前方排卡" << std::endl;
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

	std::cout << "\n移卡到读卡器" << std::endl;
	RE = R600CardMove(RWConfig.RWPos); //移卡
	if(RE) { return outputError(RE); }

	CardRW(RWConfig.RWPos); //读写卡

	//左侧排卡
	std::cout << "\n左侧排卡" << std::endl;

	RE = R600CardEject(0);
	if(RE) { return outputError(RE); }

	return 0;
}

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

	std::cout << "读写模式--0：先进行卡交互再打印（打印位）1：先打印（打印位）再进行卡操作" << std::endl;
	std::cin >> input;
	RWConfig.RWMode = input;

	std::cout << "读写位置--1：接触位 2：非接位 3：磁条位" << std::endl;
	std::cin >> input;
	RWConfig.RWPos = input;

	std::cout << "是否允许打印--0：否 1：是" << std::endl;
	std::cin >> input;
	RWConfig.isAllowPrt = input;
	
Restart:
	//打印机IO操作（包括枚举打印机、设置usb超时时间、选择打印机）
	PrtIO();

	//模拟打印和3合1读卡器读写操作
	RE = CardOperation();
	unsigned int errorStatus = 0;
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