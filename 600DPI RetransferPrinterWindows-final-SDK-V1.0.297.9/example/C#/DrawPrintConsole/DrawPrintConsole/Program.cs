using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;

namespace DrawPrintConsole
{
    class Program
    {
        static UInt32 outputError(UInt32 code)
        {           
            StringBuilder str = new StringBuilder(500);
            int len = 500;
            MethodGroup.R600GetErrorOuterInfo(code, str, ref len);         
            Console.WriteLine("出现错误：返回值：{0}, {1}\n", code, str);
            return code;
        }

        static UInt32 DrawPrint()
        {
            UInt32 RE = 0;            
            int prtNum = 0;
            int isDoubleFace = 0;
            Console.WriteLine("输入打印次数--");
            string str = Console.ReadLine();
            prtNum = int.Parse(str);

            Console.WriteLine("是否双面打印？0：否，1：是");
            str = Console.ReadLine();
            isDoubleFace = int.Parse(str);

            int i = 0;
            while(i < prtNum)
            {            
                //进卡前判断状态
                //判断打印机内部（打印位、读卡器）是否有卡
                byte flagPrt = 0;
                RE = MethodGroup.R600IsPrtHaveCard(ref flagPrt);
                if(RE != 0)
                {
                    return outputError(RE);
                }
                if(flagPrt != 0)
                {
                    Console.WriteLine("打印位有卡，正在左方排卡");
                    RE = MethodGroup.R600CardEject(0);
                    if(RE != 0)
                    {
                        return outputError(RE);
                    }
                }
                Console.WriteLine("开始套打正面");
                //准备画布
                RE = MethodGroup.R600PrepareCanvas(0, 0); //彩色（YMC）打印模式     单色（K）打印模式
                if(RE != 0)
                {
                    return outputError(RE);
                }
                //添加图片
                RE = MethodGroup.R600DrawImage(0, 0, 85.6, 53.98, "正面背景.png", 1);
                RE = MethodGroup.R600DrawImage(3, 4, 20, 4, "logo.png", 1);
                RE = MethodGroup.R600DrawImage(61, 4, 22, 15, "标签.png", 1);
                RE = MethodGroup.R600DrawImage(4, 13, 20, 25, "头像.png", 1);
                RE = MethodGroup.R600DrawImage(2, 39, 10, 10, "印章.jpg", 1);

                //添加文本 打印K色
                RE = MethodGroup.R600SetFont("黑体", 7);
                RE = MethodGroup.R600SetTextIsStrong(1);  //加粗
                RE = MethodGroup.R600DrawText(26, 5, 52, 6, "XX市转印机制造有限公司", 1);
                RE = MethodGroup.R600SetFont("宋体", 8);  //宋体；8号
                RE = MethodGroup.R600DrawText(28, 16, 30, 6, "姓名  张三李四", 0);
                RE = MethodGroup.R600DrawText(28, 21, 48, 6, "健康服务号码  430124198901285617", 0);
                RE = MethodGroup.R600DrawText(28, 25.5, 48, 6, "健康服务卡号  K00012728", 0);
                RE = MethodGroup.R600DrawText(28, 30, 46, 6, "发卡日期  2019年12月", 0);
                RE = MethodGroup.R600SetTextIsStrong(0);  //不加粗
                RE = MethodGroup.R600SetFont("黑体", 12);  //黑体
                RE = MethodGroup.R600DrawText(28, 43, 28, 6, "XX市公司WWWW", 1);
                RE = MethodGroup.R600SetFont("宋体", 5);  //宋体；5号
                RE = MethodGroup.R600SetTextIsStrong(1);  //加粗
                RE = MethodGroup.R600DrawText(2, 51, 80, 6, "服务电话（转印机服务保障：12333  XX公司：95566）", 0);
                //添加条码 不打印K色
                RE = MethodGroup.R600DrawBarCode(28, 34, 32, 5, "430124198901285617", 1);

                StringBuilder frontPath = new StringBuilder(200);
                int frontPathLen = 200;
                RE = MethodGroup.R600CommitCanvas(frontPath, ref frontPathLen);
                if(RE != 0)
                {
                    return outputError(RE);
                }

                StringBuilder backPath = new StringBuilder(200);
	            int backPathLen = 200;
	            if(1 == isDoubleFace)  //双面打印
	            {
                    Console.WriteLine("开始套打背面");
		            //准备画布
                    RE = MethodGroup.R600PrepareCanvas(0, 0);  //彩色（YMC）打印模式     单色（K）打印模式
                    if (RE != 0)
                    {
                        return outputError(RE);
                    }
		            //添加图片
                    RE = MethodGroup.R600DrawImage(0, 0, 85.6, 53.98, "背面背景.jpg", 1);
		            //结束背面
                    RE = MethodGroup.R600CommitCanvas(backPath, ref backPathLen);
                    if (RE != 0)
                    {
                        return outputError(RE);
                    }
	            }


                //StringBuilder backPath = new StringBuilder(200);
                //int backPathLen = 200;
                //RE = MethodGroup.R600CommitCanvas(backPath, ref backPathLen);
                //if (RE != 0)
                //{
                //    return outputError(RE);
                //}

                //判断打印机卡槽是否空
                int feederIsNoEmpty = 0;
                RE = MethodGroup.R600IsFeederNoEmpty(ref feederIsNoEmpty);
                if(RE != 0) 
                { 
                    return outputError(RE); 
                }
                if (0 == feederIsNoEmpty) 
                { 
                    Console.WriteLine("卡槽为空，装卡后");                    
                    Console.ReadLine();
                    continue;
                }
                Console.WriteLine("进卡到打印位");                  
                RE = MethodGroup.R600CardInject(0); //进卡到打印位置
                if(RE != 0) 
                { 
                    return outputError(RE); 
                }

                Console.WriteLine("开始打印");
                
                IntPtr frontStr = Marshal.StringToHGlobalAnsi(frontPath.ToString());                
                
                if(1 == isDoubleFace)
                {
                    IntPtr backStr = Marshal.StringToHGlobalAnsi(backPath.ToString());
                    RE = MethodGroup.R600PrintDraw(frontStr, backStr); //打印正面
                    Marshal.FreeHGlobal(backStr);
                }
                else
                {
                    RE = MethodGroup.R600PrintDraw(frontStr, new IntPtr(0)); //打印正面
                }                
                Marshal.FreeHGlobal(frontStr);
                if (RE != 0)
                {
                    return outputError(RE);
                }
                Console.WriteLine("打印完成");
                
                ////判断打印过程中机器是否出现警告或报错、、
                //int k = 0;
                //do
                //{
                //    UInt32 mainStatus = 0;      //主状态
                //    UInt32 errorStatus = 0;     //错误状态
                //    UInt32 warningStatus = 0;   //警告状态
                //    Int16 pChassisTemp = 0;
                //    Int16 pPrintheadTemp = 0;
                //    Int16 pHeaterTemp = 0;
                //    UInt32 pSubStatus = 0;
                //    byte pMainCode = 0;
                //    byte pSubCode = 0;
                  
                //    MethodGroup.R600QueryPrtStatus(ref pChassisTemp, ref pPrintheadTemp, ref pHeaterTemp, ref mainStatus, ref pSubStatus, ref errorStatus, ref warningStatus, ref pMainCode, ref pSubCode);
                //    if(errorStatus != 3001) //打印机报错
                //    {
                //        Console.WriteLine("打印机报错---请清缓存重启,重启完成后");
                //        Console.Read();
                //        goto Again;
                //    }                   
                //    if(mainStatus == 1002) //正常状态
                //    {
                //        break;
                //    }
                //    Thread.Sleep(1000);
                //    k++;

                //}while (k < 35);

                RE = MethodGroup.R600CardEject(0); //0：左方出卡
                if (RE != 0)
                { 
                    return outputError(RE); 
                }
                Console.WriteLine("成功排卡");
                i++;
            }
            return 0;
        }

        static UInt32 PrtIO()
        {
            UInt32 RE = 0;
            StringBuilder enumList = new StringBuilder(500);
            UInt32 listLen = 500;
            int num = 10;
            //枚举打印机
            RE = MethodGroup.R600EnumUsbPrt(enumList, ref listLen, ref num);
            if (RE != 0) 
            { 
                return outputError(RE); 
            }
            //设置usb超时时间
            RE = MethodGroup.R600UsbSetTimeout(3000, 3000);
            if (RE != 0) 
            { 
                return outputError(RE); 
            }
            //选择打印机
            RE = MethodGroup.R600SelectPrt(enumList);
            if (RE != 0) 
            { 
                return outputError(RE); 
            }

            return 0;
        }

        static void Main(string[] args)
        {            
            UInt32 RE = 0;
            //初始化动态库
            RE = MethodGroup.R600LibInit();
            if (RE != 0)
            { 
                outputError(RE); 
                goto End;
            }
Restart:
            //打印机IO操作（包括枚举打印机、设置usb超时时间、选择打印机）
            PrtIO();


            //判断是否机器故障
	        UInt32 mainStatus = 0;      //主状态
            UInt32 errorStatus = 0;     //错误状态
            UInt32 warningStatus = 0;   //警告状态
            Int16 pChassisTemp = 0;
            Int16 pPrintheadTemp = 0;
            Int16 pHeaterTemp = 0;
            UInt32 pSubStatus = 0;
            byte pMainCode = 0;
            byte pSubCode = 0;
                  
            MethodGroup.R600QueryPrtStatus(ref pChassisTemp, ref pPrintheadTemp, ref pHeaterTemp, ref mainStatus, ref pSubStatus, ref errorStatus, ref warningStatus, ref pMainCode, ref pSubCode);                    
	        if (RE != 0) 
            { 
                outputError(RE); 
                goto End; 
            }
	        if(errorStatus != 3001)
	        {
                Console.WriteLine("打印机报错---请清缓存重启,重启完成后");
                Console.Read();
		        goto Restart;
	        }

            //打印机套打操作（包括绘图动作、打印动作）
            DrawPrint();

            End:
            //卸载动态库
            RE = MethodGroup.R600LibClear();

            Console.WriteLine("请输入任意键退出");
            Console.ReadLine();
            
        }
    }
}
