// QueryStatusConsole.cpp : �������̨Ӧ�ó������ڵ㡣
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
	std::cout << "���ִ��󣺷���ֵ" + std::to_string((long long)code) + std::string(output) << std::endl;
	return code;
}

unsigned int CheckStatus()
{
	unsigned int res = 0;

	short shellTemp = 0;              //�����¶�
	short headTemp = 0;				  //��ӡͷ�¶�
	short heaterTemp = 0;			//���ȹ��¶�
	unsigned int mainStatus = 0;      //��״̬
	unsigned int errorStatus = 0;     //����״̬
	unsigned int warningStatus = 0;   //����״̬
	res = R600QueryPrtStatus(&shellTemp, &headTemp, &heaterTemp, &mainStatus, 0, &errorStatus, &warningStatus, 0, 0);
	if (res)
		return outputError(res);

	char output[512] = { 0 };
	sprintf_s(output, "�����¶ȣ�%.2f��\r\n��ӡͷ�¶ȣ�%.2f��\r\n���ȹ��¶ȣ�%.2f��\r\n��״̬��%0d\r\n����״̬��%d\r\n����״̬��%d\r\n",
		((double)shellTemp) / 100, ((double)headTemp) / 100, ((double)heaterTemp) / 100, mainStatus, errorStatus, warningStatus);
	std::cout << std::string(output) << std::endl;

	return 0;
}

unsigned int CheckPrtInfo()
{
	unsigned int res = 0;

	char PrtName[20] = { 0 };      //��ӡ����
	int PrtNameLen = 20;
	char Manufacturer[20] = { 0 }; //������
	int ManufacturerLen = 20;
	char PrinterSN[20] = { 0 };    //��ӡ�����к�
	int PrinterSNLen = 20;
	char Date[20] = { 0 };		   //��������
	int DateLen = 20;
	char FWVer[20] = { 0 };		   //�������汾��
	int FWverLen = 20;
	res = R600GetPrtManufInfo(PrtName, &PrtNameLen, Manufacturer, &ManufacturerLen, PrinterSN, &PrinterSNLen, Date, &DateLen, FWVer, &FWverLen);
	if (res)
		return outputError(res);

	std::string output = "��ӡ�����ƣ� " + std::string(PrtName) + "\r\n�����̣� " + std::string(Manufacturer) + "\r\n��ӡ�����кţ� "
		+ std::string(PrinterSN) + "\r\n�������ڣ� " + std::string(Date) + "\r\n����汾�ţ� " + std::string(FWVer);
	std::cout << output << std::endl;

	return 0;
}

unsigned int CheckVersion()
{
	unsigned int res = 0;

	unsigned int headPrtCount = 0;    //��ӡͷ��ӡ����
	unsigned int PrtCount = 0;        //��ӡ�ܴ���
	unsigned int FWMainVer = 0;           //FW���汾��
	unsigned int FWSubVer = 0;           //FW�Ӱ汾��
	unsigned int FPGAVer = 0;		  //FPGA�汾��
	res = R600GetPrtInfo(&headPrtCount, &PrtCount, &FWMainVer, &FWSubVer, &FPGAVer);
	if (res)
		return outputError(res);

	char output[512] = { 0 };
	sprintf_s(output, "��ӡͷ��ӡ������%d\r\n��ӡ�ܴ�����%d\r\nFW���汾�ţ�%02X.%02X.%02X.%02X\r\nFW�Ӱ汾�ţ�%02X.%02X.%02X.%02X\r\nFPGA�汾�ţ�%02d.%02d.%02d", 
		headPrtCount, PrtCount, FWMainVer & 0xff, FWMainVer >> 8 & 0xff, FWMainVer >> 16 & 0xff, FWMainVer >> 24 & 0xff, 
		FWSubVer & 0xff, FWSubVer >> 8 & 0xff, FWSubVer >> 16 & 0xff, FWSubVer >> 24 & 0xff, 
		FPGAVer >> 16 & 0xff, FPGAVer >> 8 & 0xff, FPGAVer & 0xff);
	std::cout << output << std::endl;

	return 0;
}

unsigned int CheckRibbon()
{
	unsigned int res = 0;

	unsigned short rbnSurplus = 0;   //ɫ��ʣ��
	unsigned short filmSurplus = 0;   //תӡĤʣ��
	res = R600GetRbnAndFilmRemaining(&rbnSurplus, &filmSurplus);
	if (res)
		return outputError(res);

	unsigned char ribbonType = 0;   //ɫ������
	unsigned char filmType = 0;   //תӡĤ����
	unsigned char ribbonNearEnd = 0;      //ɫ������ֵ֪ͨ
	unsigned char filmNearEnd = 0;      //תӡĤ����ֵ֪ͨ
	res = R600RibbonSettingsRW(0, &ribbonType, &filmType, &ribbonNearEnd, &filmNearEnd);
	if (res)
		return outputError(res);

	char output[512] = { 0 };
	sprintf_s(output, "ɫ��ʣ������%d\r\nɫ�����ͣ�0x%02X\r\nɫ������ֵ֪ͨ��%d", rbnSurplus, ribbonType, ribbonNearEnd);
	std::cout << output << std::endl;

	memset(output, 0, 512);
	sprintf_s(output, "תӡĤʣ������%d\r\nתӡĤ���ͣ�0x%02X\r\nתӡĤ����ֵ֪ͨ��%d", filmSurplus, filmType, filmNearEnd);
	std::cout << output << std::endl;


	return 0;
}

unsigned int CheckHaveCard()
{
	unsigned int res = 0;

	int flag = 0;   //0���޿���1���п�
	res = R600GetCardPos(&flag);
	if (res)
		return outputError(res);

	if (flag != 0)
	{
		std::cout << "��ӡ���ڲ��п�" << std::endl;
	}
	else
	{
		std::cout << "��ӡ���ڲ��޿�" << std::endl;
	}

	return 0;
}


void SelectMode()
{
	int Exit = 0;
	while(!Exit)
	{
		std::cout << "\n" << std::endl;
		std::cout << "��ѯ��ӡ��״̬: ----------- 1 " << std::endl;
		std::cout << "��ѯ��ӡ���̼�: ----------- 2 " << std::endl;
		std::cout << "��ѯ��ӡ���汾: ----------- 3 " << std::endl;
		std::cout << "��ѯ��ӡ��ɫ��: ----------- 4 " << std::endl;
		std::cout << "��ѯ��ӡ���Ƿ��п�: ------- 5 " << std::endl;		
		std::cout << "�˳�: --------------------- 6 " << std::endl;

		int input;
		std::cin >> input;
		if (std::cin.fail())
		{
			std::cout << "��������" << std::endl;
			Exit = 1;
			break;
		}
			

		switch (input)
		{
		case 1:
			CheckStatus(); //��ѯ��ӡ��״̬
			break;
		case 2:
			CheckPrtInfo(); //��ѯ��ӡ���̼�
			break;
		case 3:
			CheckVersion(); //��ѯ��ӡ���汾
			break;
		case 4:
			CheckRibbon(); //��ѯ��ӡ��ɫ��
			break;
		case 5:
			CheckHaveCard(); //��ѯ��ӡ���Ƿ��п�
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
	//��ʼ����̬��
	res = R600LibInit();
	if (res)
		outputError(res);

	char enumList[100] = { 0 };
	unsigned int listLen = 100;
	int num = 0;
	//ö�ٴ�ӡ��
	res = R600EnumUsbPrt(enumList, &listLen, &num);
	if (res)
		outputError(res);
	//����usb��ʱʱ��
	res = R600UsbSetTimeout(3000, 3000);
	if (res)
		outputError(res);
	//ѡ���ӡ��
	res = R600SelectPrt(enumList);
	if (res)
		outputError(res);

	SelectMode();

	//ж�ض�̬��
	R600LibClear();
	system("pause");
	return 0;
}

