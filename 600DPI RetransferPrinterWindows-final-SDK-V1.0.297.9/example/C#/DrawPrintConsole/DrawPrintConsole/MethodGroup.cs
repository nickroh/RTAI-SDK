using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace DrawPrintConsole
{
    public static class MethodGroup
    {
        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600LibInit();

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600LibClear();

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600GetErrorOuterInfo(UInt32 errcode, StringBuilder outputstr, ref int len);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600EnumUsbPrt(StringBuilder szEnumList, ref uint pEnumListLen, ref int pNum);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600SelectPrt(StringBuilder szPrt);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600GetRibbonRemaining(ref int pRemaining);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600PrepareCanvas(int nChromaticMode, int nMonoChroMode);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600DrawImage(double dX, double dY, double dWidth, double dHeight, String szImgFilePath, int nSetNoAbsoluteBlack);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600SetFont(String szFontName, float fSize);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600DrawText(double dX, double dY, double width, double height, String szText, int nSetNoAbsoluteBlack);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600SetTextIsStrong(int nStrong);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600DrawBarCode(double dX, double dY, double dWidth, double dHeight, String szData, int nSetNoAbsoluteBlack);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600CommitCanvas(StringBuilder szImgInfo, ref int pImgInfoLen);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600IsFeederNoEmpty(ref int pFlag);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600CardEject(byte ucDestPos);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600CardInject(byte ucDestPos);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600CardMove(byte ucDestPos);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600GetCardPos(ref int pPos);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600UsbSetTimeout(int nReadTimeout, int nWriteTimeout);

        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600IsPrtHaveCard(ref byte pFlag);
        
        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600PrintDraw(IntPtr szImgInfoFront, IntPtr szImgInfoBack);
        
        [DllImport("libDSRetransfer600App.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern UInt32 R600QueryPrtStatus(ref Int16 pChassisTemp, ref Int16 pPrintheadTemp, ref Int16 pHeaterTemp, ref UInt32 pMainStatus,
            ref UInt32 pSubStatus, ref UInt32 pErrorStatus, ref UInt32 pWarningStatus, ref byte pMainCode, ref byte pSubCode);
        
        
    }
}
