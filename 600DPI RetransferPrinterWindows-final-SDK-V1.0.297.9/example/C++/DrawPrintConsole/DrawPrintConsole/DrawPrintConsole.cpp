// DrawPrintConsole.cpp : �������̨Ӧ�ó������ڵ㡣
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

struct prtConfig
{
	int prtNumber;   //��ӡ����
	int isTwoSided;  //�Ƿ�˫��
}PrtConfig;


unsigned int DrawPrint()
{
	unsigned int RE = 0;
	int input = 0;
	std::cout << "�����ӡ����--" << std::endl;
	std::cin >> input;
	PrtConfig.prtNumber = input;

	std::cout << "�Ƿ�˫���ӡ--0���� 1����" << std::endl;
	std::cin >> input;
	PrtConfig.isTwoSided = input;

	int i = 0;
	while (i < PrtConfig.prtNumber)
	{
Again:
		//////����ǰ�ж�״̬
		//�жϴ�ӡ���ڲ�����ӡλ�����������Ƿ��п�
		int flagPrt = 0; 
		RE = R600GetCardPos(&flagPrt);
		if(RE) { return outputError(RE); }
		if(flagPrt != 0) 
		{
			std::cout << "��ӡλ�п�����������ſ�" << std::endl;
			RE = R600CardEject(0);
			if(RE) { return outputError(RE); }
		}

		std::cout << "��ʼ�״�����" << std::endl;
		//׼������
		RE = R600PrepareCanvas(0, 0);  //��ɫ��YMC����ӡģʽ     ��ɫ��K����ӡģʽ
		if(RE) { return outputError(RE); }
	
		//���ͼƬ
		R600DrawImage(0, 0, 85.6, 53.98, "���汳��.png", true);
		R600DrawImage(3, 4, 20, 4, "logo.png", true);
		R600DrawImage(61, 4, 22, 15, "��ǩ.png", true);
		R600DrawImage(4, 13, 20, 25, "ͷ��.png", true);
		R600DrawImage(2, 39, 10, 10, "ӡ��.png", true);
		//����ı� ��ӡKɫ
		R600SetFont(L"����", 7);
		R600SetTextIsStrong(1);  //�Ӵ�
		R600DrawText(26, 5, 52, 6, L"XX��תӡ���������޹�˾", 1);
		R600SetFont(L"����", 8);  //���壻8��
		R600DrawText(28, 16, 30, 6, L"����  ��������", 0);
		R600DrawText(28, 21, 48, 6, L"�����������  430124198901285617", 0);
		R600DrawText(28, 25.5, 48, 6, L"�������񿨺�  K00012728", 0);
		R600DrawText(28, 30, 46, 6, L"��������  2019��12��", 0);
		R600SetTextIsStrong(0);  //���Ӵ�
		R600SetFont(L"����", 12);  //����
		R600DrawText(28, 43, 28, 6, L"XX�й�˾WWWW", 1);
		R600SetFont(L"����", 5);  //���壻5��
		R600SetTextIsStrong(1);  //�Ӵ�
		R600DrawText(2, 51, 80, 6, L"����绰��תӡ�������ϣ�12333  XX��˾��95566��", 0);
		//������� ����ӡKɫ
		R600DrawBarCode(28, 34, 32, 5, "430124198901285617", 1);
		//��������
		char frontPath[200] = { 0 };
		int frontPathLen = 200;
		RE = R600CommitCanvas(frontPath, &frontPathLen);
		if(RE) { return outputError(RE); }

		char backPath[200] = { 0 };
		int backPathLen = 200;
		if(PrtConfig.isTwoSided)  //˫���ӡ
		{
			std::cout << "��ʼ�״���" << std::endl;
			//׼������
			RE = R600PrepareCanvas(0, 0);  //��ɫ��YMC����ӡģʽ     ��ɫ��K����ӡģʽ
			if(RE) { return outputError(RE); }
			//���ͼƬ
			R600DrawImage(0, 0, 85.6, 53.98, "���汳��.jpg", true);
			//��������
			RE = R600CommitCanvas(backPath, &backPathLen);
			if(RE) { return outputError(RE); }
		}

		//�жϴ�ӡ�������Ƿ��
		int feederIsNoEmpty = 0;
		RE = R600IsFeederNoEmpty(&feederIsNoEmpty);
		if(RE) { return outputError(RE); }
		if(!feederIsNoEmpty) 
		{ 
			std::cout << "����Ϊ�գ�װ����" << std::endl; 
			system("pause");
			continue;
		}

		std::cout << "\n��������ӡλ" << std::endl;
		RE = R600CardInject(0); //��������ӡλ��
		if(RE) { return outputError(RE); }
		

		if(PrtConfig.isTwoSided)  //˫���ӡ
		{
			std::cout << "��ʼ��ӡ" << std::endl;
			RE = R600PrintDraw(frontPath, backPath); //��ӡ˫��
			if (RE){ return outputError(RE); }
		}
		else
		{
			std::cout << "��ʼ��ӡ" << std::endl;
			RE = R600PrintDraw(frontPath, 0); //��ӡ����
			if (RE){ return outputError(RE); }
		}

		RE = R600CardEject(0); //0��������
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


int main()
{
	unsigned int RE = 0;
	//��ʼ����̬��
	RE = R600LibInit();
	if (RE) {  outputError(RE); goto End; }

Restart:
	//��ӡ��IO����������ö�ٴ�ӡ��������usb��ʱʱ�䡢ѡ���ӡ����
	PrtIO();

	//�ж��Ƿ��������
	unsigned int errorStatus = 0;     //����״̬
	RE = R600QueryPrtStatus(0, 0, 0, 0, 0, &errorStatus, 0, 0, 0);
	if (RE) { outputError(RE); goto End; }
	if(errorStatus != 3001)
	{
		std::cout << "��ӡ������---���建������,������ɺ�" << std::endl;
		system("pause");
		goto Restart;
	}
	
	//��ӡ���״������������ͼ��������ӡ������
	DrawPrint();

End:
	//ж�ض�̬��
	RE = R600LibClear();
	system("pause");
    return 0;
}

