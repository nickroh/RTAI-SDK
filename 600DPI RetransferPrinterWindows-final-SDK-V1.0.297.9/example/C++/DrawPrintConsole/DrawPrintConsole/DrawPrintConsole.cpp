// DrawPrintConsole.cpp : 定义控制台应用程序的入口点。
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

struct prtConfig
{
	int prtNumber;   //打印次数
	int isTwoSided;  //是否双面
}PrtConfig;


unsigned int DrawPrint()
{
	unsigned int RE = 0;
	int input = 0;
	std::cout << "输入打印次数--" << std::endl;
	std::cin >> input;
	PrtConfig.prtNumber = input;

	std::cout << "是否双面打印--0：否 1：是" << std::endl;
	std::cin >> input;
	PrtConfig.isTwoSided = input;

	int i = 0;
	while (i < PrtConfig.prtNumber)
	{
Again:
		//////进卡前判断状态
		//判断打印机内部（打印位、读卡器）是否有卡
		int flagPrt = 0; 
		RE = R600GetCardPos(&flagPrt);
		if(RE) { return outputError(RE); }
		if(flagPrt != 0) 
		{
			std::cout << "打印位有卡，正在左侧排卡" << std::endl;
			RE = R600CardEject(0);
			if(RE) { return outputError(RE); }
		}

		std::cout << "开始套打正面" << std::endl;
		//准备画布
		RE = R600PrepareCanvas(0, 0);  //彩色（YMC）打印模式     单色（K）打印模式
		if(RE) { return outputError(RE); }
	
		//添加图片
		R600DrawImage(0, 0, 85.6, 53.98, "正面背景.png", true);
		R600DrawImage(3, 4, 20, 4, "logo.png", true);
		R600DrawImage(61, 4, 22, 15, "标签.png", true);
		R600DrawImage(4, 13, 20, 25, "头像.png", true);
		R600DrawImage(2, 39, 10, 10, "印章.png", true);
		//添加文本 打印K色
		R600SetFont(L"黑体", 7);
		R600SetTextIsStrong(1);  //加粗
		R600DrawText(26, 5, 52, 6, L"XX市转印机制造有限公司", 1);
		R600SetFont(L"宋体", 8);  //宋体；8号
		R600DrawText(28, 16, 30, 6, L"姓名  张三李四", 0);
		R600DrawText(28, 21, 48, 6, L"健康服务号码  430124198901285617", 0);
		R600DrawText(28, 25.5, 48, 6, L"健康服务卡号  K00012728", 0);
		R600DrawText(28, 30, 46, 6, L"发卡日期  2019年12月", 0);
		R600SetTextIsStrong(0);  //不加粗
		R600SetFont(L"黑体", 12);  //黑体
		R600DrawText(28, 43, 28, 6, L"XX市公司WWWW", 1);
		R600SetFont(L"宋体", 5);  //宋体；5号
		R600SetTextIsStrong(1);  //加粗
		R600DrawText(2, 51, 80, 6, L"服务电话（转印机服务保障：12333  XX公司：95566）", 0);
		//添加条码 不打印K色
		R600DrawBarCode(28, 34, 32, 5, "430124198901285617", 1);
		//结束正面
		char frontPath[200] = { 0 };
		int frontPathLen = 200;
		RE = R600CommitCanvas(frontPath, &frontPathLen);
		if(RE) { return outputError(RE); }

		char backPath[200] = { 0 };
		int backPathLen = 200;
		if(PrtConfig.isTwoSided)  //双面打印
		{
			std::cout << "开始套打背面" << std::endl;
			//准备画布
			RE = R600PrepareCanvas(0, 0);  //彩色（YMC）打印模式     单色（K）打印模式
			if(RE) { return outputError(RE); }
			//添加图片
			R600DrawImage(0, 0, 85.6, 53.98, "背面背景.jpg", true);
			//结束背面
			RE = R600CommitCanvas(backPath, &backPathLen);
			if(RE) { return outputError(RE); }
		}

		//判断打印机卡槽是否空
		int feederIsNoEmpty = 0;
		RE = R600IsFeederNoEmpty(&feederIsNoEmpty);
		if(RE) { return outputError(RE); }
		if(!feederIsNoEmpty) 
		{ 
			std::cout << "卡槽为空，装卡后" << std::endl; 
			system("pause");
			continue;
		}

		std::cout << "\n进卡到打印位" << std::endl;
		RE = R600CardInject(0); //进卡到打印位置
		if(RE) { return outputError(RE); }
		

		if(PrtConfig.isTwoSided)  //双面打印
		{
			std::cout << "开始打印" << std::endl;
			RE = R600PrintDraw(frontPath, backPath); //打印双面
			if (RE){ return outputError(RE); }
		}
		else
		{
			std::cout << "开始打印" << std::endl;
			RE = R600PrintDraw(frontPath, 0); //打印正面
			if (RE){ return outputError(RE); }
		}

		RE = R600CardEject(0); //0：左侧出卡
		if (RE){ return outputError(RE); }

		i++;
	}
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


int main()
{
	unsigned int RE = 0;
	//初始化动态库
	RE = R600LibInit();
	if (RE) {  outputError(RE); goto End; }

Restart:
	//打印机IO操作（包括枚举打印机、设置usb超时时间、选择打印机）
	PrtIO();

	//判断是否机器故障
	unsigned int errorStatus = 0;     //错误状态
	RE = R600QueryPrtStatus(0, 0, 0, 0, 0, &errorStatus, 0, 0, 0);
	if (RE) { outputError(RE); goto End; }
	if(errorStatus != 3001)
	{
		std::cout << "打印机报错---请清缓存重启,重启完成后" << std::endl;
		system("pause");
		goto Restart;
	}
	
	//打印机套打操作（包括绘图动作、打印动作）
	DrawPrint();

End:
	//卸载动态库
	RE = R600LibClear();
	system("pause");
    return 0;
}

