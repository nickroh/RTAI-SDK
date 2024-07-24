// ContactCardConsole.cpp : �������̨Ӧ�ó������ڵ㡣
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
	int RWNumber;    //��д����
	int RWMode;      //��дģʽ0���Ƚ��п������ٴ�ӡ����ӡλ��1���ȴ�ӡ����ӡλ���ٽ��п�����
	//int CardType;    //������--1���ǽ�CPU 2��Type A Mifare S50
	int isAllowPrt;  //�Ƿ������ӡ 
}RWConfig;

unsigned int PrtIO()
{
	unsigned int RE = 0;
	char enumList[100] = { 0 };
	unsigned int listLen = 100;
	int num = 0;
	//ö�ٴ�ӡ��
	RE = R600EnumUsbPrt(enumList, &listLen, &num);
	if (RE) { return outputError(RE); }
	//����usb��ʱʱ��
	RE = R600UsbSetTimeout(3000, 3000);
	if (RE) { return outputError(RE); }
	//ѡ���ӡ��
	RE = R600SelectPrt(enumList);
	if (RE) { return outputError(RE); }

	return 0;
}

unsigned int Printing()
{
	std::cout << "��ӡ����ʼ��ӡ" << std::endl;
	unsigned int RE = 0;
	//׼������
	R600PrepareCanvas(0, 0);  //��ɫ��YMC����ӡģʽ300*300dpi     ��ɫ��K����ӡģʽ300*600dpi
	//���ͼƬ
	R600DrawImage(0, 0, 0, 0, "����.jpg", true);
	char frontPath[200] = { 0 };
	int frontPathLen = 200;
	R600CommitCanvas(frontPath, &frontPathLen);
	RE = R600PrintDraw(frontPath, 0); //��ӡ����
	if (RE){ return outputError(RE); }

	////�жϴ�ӡ�����л����Ƿ���־���򱨴�
	//int k = 0;
	//do 
	//{
	//	unsigned int mainStatus = 0;      //��״̬
	//	unsigned int errorStatus = 0;     //����״̬
	//	unsigned int warningStatus = 0;   //����״̬
	//	R600QueryPrtStatus(0, 0, 0, &mainStatus, 0, &errorStatus, &warningStatus, 0, 0);
	//	if(errorStatus != 3001) //��ӡ������
	//	{
	//		std::cout << "��ӡ������---���建������" << std::endl;
	//		return 1;
	//	}
	//	if(mainStatus == 1002) //����״̬
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
	byte slotNum = 0xc;//������
	RE = R600CPUCardReset(slotNum, pResetInfo, &pResetInfolen, 0, 0);  //�Ӵ�ʽcpu����λ
	if(RE) { return outputError(RE); }

	unsigned char pSApdu[5] = { 0x00, 0x84, 0x00, 0x00, 0x04 };  //���ͽ���ָ��
	char pRApdu[130] = { 0 };  //���ս���ָ��
	unsigned char pRApduLen = 130;  //������ճ���
	RE = R600CPUCardExchangeAPDU(slotNum, pSApdu, 5, pRApdu, &pRApduLen);  //�Ӵ�ʽcpu������
	if(RE) 
	{ 
		return outputError(RE);
	}
	else
	{
		//����̨�����Ϣ		
		std::cout << "�Ӵ�ʽCPU��λ��Ϣ�� " + std::string(pResetInfo) << std::endl;
		std::cout << "���ͽ���ָ�0x00, 0x84, 0x00, 0x00, 0x04" << std::endl;
		std::string infoRecv = Hex2String__(pRApdu, pRApduLen);
		std::cout << "�Ӵ�ʽCPU����������Ϣ�� " + infoRecv << std::endl;
		return 0;
	}	
}


unsigned int Step1RWCard_Step2Print()
{
Step1RWCard_Step2Print:
	std::cout << "\n" << std::endl;
	std::cout << "�������Ӵ�ʽ�������Ӵ�λ" << std::endl;

	unsigned int RE = 0;
	//�жϴ�ӡ���ڲ�����ӡλ���Ƿ��п�
	int flagPrt = 0; 
	RE = R600GetCardPos(&flagPrt);
	if(RE) { return outputError(RE); }
	if(flagPrt != 0) 
	{
		std::cout << "��ӡλ�п�����������ſ�" << std::endl;
		RE = R600CardEject(0); //�ſ�
		if(RE) { return outputError(RE); }
	}

	//�жϴ�ӡ�������Ƿ��
	int feederIsNoEmpty = 0;
	RE = R600IsFeederNoEmpty(&feederIsNoEmpty);
	if(RE) { return outputError(RE); }
	if(!feederIsNoEmpty) 
	{ 
		std::cout << "����Ϊ�գ���װ��" << std::endl; 
		system("pause");
		goto Step1RWCard_Step2Print;
	}

	RE = R600CardInject(6); //�������Ӵ�ʽ�������Ӵ�λ
	if(RE) { return outputError(RE); }

	//��д��
	ReaderCPURW();

	//�ƿ�����ӡλ
	std::cout << "\n�ƿ�����ӡλ" << std::endl;
	RE = R600CardMove(4);
	if(RE) { return outputError(RE); }

	//�Ƿ��ӡ
	if(RWConfig.isAllowPrt)
	{
		RE = Printing();
		if(RE) { return outputError(RE); }
	}

	//����ſ�
	std::cout << "\n����ſ�" << std::endl;

	RE = R600CardEject(0);
	if(RE) { return outputError(RE); }

	return 0;
}

unsigned int Step1Print_Step2RWCard()
{
Step1Print_Step2RWCard:
	unsigned int RE = 0;
	//�жϴ�ӡ���ڲ�����ӡλ���Ƿ��п�

	int flagPrt = 0; 
	RE = R600GetCardPos(&flagPrt);
	if(RE) { return outputError(RE); }
	if(flagPrt != 0) 
	{
		std::cout << "��ӡλ�п�����������ſ�" << std::endl;
		RE = R600CardEject(0);
		if(RE) { return outputError(RE); }
	}

	//�жϴ�ӡ�������Ƿ��
	int feederIsNoEmpty = 0;
	RE = R600IsFeederNoEmpty(&feederIsNoEmpty);
	if(RE) { return outputError(RE); }
	if(!feederIsNoEmpty) 
	{ 
		std::cout << "����Ϊ�գ���װ��" << std::endl; 
		system("pause");
		goto Step1Print_Step2RWCard;
	}

	std::cout << "\n��������ӡλ" << std::endl;
	RE = R600CardInject(0); //��������ӡλ��
	if(RE) { return outputError(RE); }

	//�Ƿ��ӡ
	if(RWConfig.isAllowPrt)
	{
		RE = Printing();
		if(RE) { return outputError(RE); }
	}

	std::cout << "�ƿ����Ӵ�ʽ�������Ӵ�λ" << std::endl;
	RE = R600CardMove(6); //�ƿ�
	if(RE) { return outputError(RE); }

	//��д��
	ReaderCPURW();

	//����ſ�
	std::cout << "\n����ſ�" << std::endl;	

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
	//��ʼ����̬��
	RE = R600LibInit();
	if (RE) { return outputError(RE); }

	int input;
	std::cout << "�����д����--" << std::endl;
	std::cin >> input;
	RWConfig.RWNumber = input;

	//std::cout << "������--1���ǽ�CPU 2��Type A Mifare S50" << std::endl;
	//std::cin >> input;
	//RWConfig.CardType = input;

	std::cout << "��дģʽ--0���Ƚ��п������ٴ�ӡ����ӡλ��1���ȴ�ӡ����ӡλ���ٽ��п�����" << std::endl;
	std::cin >> input;
	RWConfig.RWMode = input;

	std::cout << "�Ƿ������ӡ--0���� 1����" << std::endl;
	std::cin >> input;
	RWConfig.isAllowPrt = input;

Restart:
	//��ӡ��IO����������ö�ٴ�ӡ��������usb��ʱʱ�䡢ѡ���ӡ����
	PrtIO();

	//ģ���ӡ�ͷǽӶ�������д����
	RE = CardOperation();

	//�ж��Ƿ��������
	unsigned int errorStatus = 0;     //����״̬
	if(RE)
	{
		//���ӿڷ��ش���ֵʱ���ж��Ƿ��������
		R600QueryPrtStatus(0, 0, 0, 0, 0, &errorStatus, 0, 0, 0);
		if(errorStatus != 3001)
		{
			std::cout << "��ӡ������---���建������,������ɺ�" << std::endl;
			system("pause");
			goto Restart;
		}
	}

	//ж�ض�̬��
	RE = R600LibClear();
	if (RE) { return outputError(RE); }
	system("pause");
	return 0;
}

