// QueryStatusConsole.cpp : 定义控制台应用程序的入口点。
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

unsigned int CheckStatus()
{
	unsigned int res = 0;

	short shellTemp = 0;              //机身温度
	short headTemp = 0;				  //打印头温度
	short heaterTemp = 0;			//加热辊温度
	unsigned int mainStatus = 0;      //主状态
	unsigned int errorStatus = 0;     //错误状态
	unsigned int warningStatus = 0;   //警告状态
	res = R600QueryPrtStatus(&shellTemp, &headTemp, &heaterTemp, &mainStatus, 0, &errorStatus, &warningStatus, 0, 0);
	if (res)
		return outputError(res);

	char output[512] = { 0 };
	sprintf_s(output, "机身温度：%.2f℃\r\n打印头温度：%.2f℃\r\n加热辊温度：%.2f℃\r\n主状态：%0d\r\n错误状态：%d\r\n警告状态：%d\r\n",
		((double)shellTemp) / 100, ((double)headTemp) / 100, ((double)heaterTemp) / 100, mainStatus, errorStatus, warningStatus);
	std::cout << std::string(output) << std::endl;

	return 0;
}

unsigned int CheckPrtInfo()
{
	unsigned int res = 0;

	char PrtName[20] = { 0 };      //打印机名
	int PrtNameLen = 20;
	char Manufacturer[20] = { 0 }; //制造商
	int ManufacturerLen = 20;
	char PrinterSN[20] = { 0 };    //打印机序列号
	int PrinterSNLen = 20;
	char Date[20] = { 0 };		   //生产日期
	int DateLen = 20;
	char FWVer[20] = { 0 };		   //程序主版本号
	int FWverLen = 20;
	res = R600GetPrtManufInfo(PrtName, &PrtNameLen, Manufacturer, &ManufacturerLen, PrinterSN, &PrinterSNLen, Date, &DateLen, FWVer, &FWverLen);
	if (res)
		return outputError(res);

	std::string output = "打印机名称： " + std::string(PrtName) + "\r\n制造商： " + std::string(Manufacturer) + "\r\n打印机序列号： "
		+ std::string(PrinterSN) + "\r\n生产日期： " + std::string(Date) + "\r\n程序版本号： " + std::string(FWVer);
	std::cout << output << std::endl;

	return 0;
}

unsigned int CheckVersion()
{
	unsigned int res = 0;

	unsigned int headPrtCount = 0;    //打印头打印次数
	unsigned int PrtCount = 0;        //打印总次数
	unsigned int FWMainVer = 0;           //FW主版本号
	unsigned int FWSubVer = 0;           //FW子版本号
	unsigned int FPGAVer = 0;		  //FPGA版本号
	res = R600GetPrtInfo(&headPrtCount, &PrtCount, &FWMainVer, &FWSubVer, &FPGAVer);
	if (res)
		return outputError(res);

	char output[512] = { 0 };
	sprintf_s(output, "打印头打印次数：%d\r\n打印总次数：%d\r\nFW主版本号：%02X.%02X.%02X.%02X\r\nFW子版本号：%02X.%02X.%02X.%02X\r\nFPGA版本号：%02d.%02d.%02d", 
		headPrtCount, PrtCount, FWMainVer & 0xff, FWMainVer >> 8 & 0xff, FWMainVer >> 16 & 0xff, FWMainVer >> 24 & 0xff, 
		FWSubVer & 0xff, FWSubVer >> 8 & 0xff, FWSubVer >> 16 & 0xff, FWSubVer >> 24 & 0xff, 
		FPGAVer >> 16 & 0xff, FPGAVer >> 8 & 0xff, FPGAVer & 0xff);
	std::cout << output << std::endl;

	return 0;
}

unsigned int CheckRibbon()
{
	unsigned int res = 0;

	unsigned short rbnSurplus = 0;   //色带剩余
	unsigned short filmSurplus = 0;   //转印膜剩余
	res = R600GetRbnAndFilmRemaining(&rbnSurplus, &filmSurplus);
	if (res)
		return outputError(res);

	unsigned char ribbonType = 0;   //色带类型
	unsigned char filmType = 0;   //转印膜类型
	unsigned char ribbonNearEnd = 0;      //色带余量通知值
	unsigned char filmNearEnd = 0;      //转印膜余量通知值
	res = R600RibbonSettingsRW(0, &ribbonType, &filmType, &ribbonNearEnd, &filmNearEnd);
	if (res)
		return outputError(res);

	char output[512] = { 0 };
	sprintf_s(output, "色带剩余量：%d\r\n色带类型：0x%02X\r\n色带余量通知值：%d", rbnSurplus, ribbonType, ribbonNearEnd);
	std::cout << output << std::endl;

	memset(output, 0, 512);
	sprintf_s(output, "转印膜剩余量：%d\r\n转印膜类型：0x%02X\r\n转印膜余量通知值：%d", filmSurplus, filmType, filmNearEnd);
	std::cout << output << std::endl;


	return 0;
}

unsigned int CheckHaveCard()
{
	unsigned int res = 0;

	int flag = 0;   //0：无卡；1：有卡
	res = R600GetCardPos(&flag);
	if (res)
		return outputError(res);

	if (flag != 0)
	{
		std::cout << "打印机内部有卡" << std::endl;
	}
	else
	{
		std::cout << "打印机内部无卡" << std::endl;
	}

	return 0;
}


void SelectMode()
{
	int Exit = 0;
	while(!Exit)
	{
		std::cout << "\n" << std::endl;
		std::cout << "查询打印机状态: ----------- 1 " << std::endl;
		std::cout << "查询打印机固件: ----------- 2 " << std::endl;
		std::cout << "查询打印机版本: ----------- 3 " << std::endl;
		std::cout << "查询打印机色带: ----------- 4 " << std::endl;
		std::cout << "查询打印机是否有卡: ------- 5 " << std::endl;		
		std::cout << "退出: --------------------- 6 " << std::endl;

		int input;
		std::cin >> input;
		if (std::cin.fail())
		{
			std::cout << "输入有误" << std::endl;
			Exit = 1;
			break;
		}
			

		switch (input)
		{
		case 1:
			CheckStatus(); //查询打印机状态
			break;
		case 2:
			CheckPrtInfo(); //查询打印机固件
			break;
		case 3:
			CheckVersion(); //查询打印机版本
			break;
		case 4:
			CheckRibbon(); //查询打印机色带
			break;
		case 5:
			CheckHaveCard(); //查询打印机是否有卡
			break;
		case 6:
			Exit = 1;
			break;
		}
	}
}


int main()
{
	unsigned int res = 0;
	//初始化动态库
	res = R600LibInit();
	if (res)
		outputError(res);

	char enumList[100] = { 0 };
	unsigned int listLen = 100;
	int num = 0;
	//枚举打印机
	res = R600EnumUsbPrt(enumList, &listLen, &num);
	if (res)
		outputError(res);
	//设置usb超时时间
	res = R600UsbSetTimeout(3000, 3000);
	if (res)
		outputError(res);
	//选择打印机
	res = R600SelectPrt(enumList);
	if (res)
		outputError(res);

	SelectMode();

	//卸载动态库
	R600LibClear();
	system("pause");
	return 0;
}

