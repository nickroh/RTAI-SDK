/**
 *
 * @file libDSRetransfer600App.h
 * @brief 600DPI transfer printer interface head file
 * @author SDK Group
 * @date 2020-5-20
 *
 *
 */

#ifndef __libDSRetransfer600App_H__
#define __libDSRetransfer600App_H__

#ifndef DSSDK
#define DSSDK __stdcall
#endif


#ifndef MakeStatus(code)
#define MakeStatus(code) ( (code & 0xff) << 24 | (code & 0xff00) << 8 | (code & 0xff0000) >> 8 | (code & 0xff000000) >> 24 )
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup DynamicLibraryRelatived Dynamic library correlation
 *  @brief Dynamic library related interfaces, such as initialization, resource cleaning, and so on
 *
 *  @{
 *
 */

/**
 * @brief  Dynamic library initialization
 * @par    Description£º
 * To initialize SDK library resource, it must be called with R600LibClear.Note£ºThis initialization function must be called to use this library!!!
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600LibInit();


/**
 * @brief   Dynamic library cleaning
 * @par     Description£º
 * Dynamic libraries must be called before they can be unloaded
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600LibClear();



/**
 *
 * @}
 */


/** @defgroup PrinterControl PrinterControl
 *  @brief Printer enumerations, connection timeout Settings, choose printers, and so on
 *
 *  @{
 *
 */

/**
 * @brief  enumerate USB printer
 * @par    Description£º
 * Make sure that USB printer is connected
 * @param [out] szEnumList Enumerated lists,if there're multiple printers, they should be separated by "\\n"
 * @param [out] pEnumListLen Enumeration list length
 * @param [out] pNum the number of Enumerating USB printers
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600EnumUsbPrt(char *szEnumList, unsigned int *pEnumListLen, int *pNum);	


/**
 * @brief  Set the USB port timeout
 * @par    Description£º
 * the default value is 3000ms, if it has not been set up
 * @param [in] nReadTimeout IO read timeout(unit: MS)
 * @param [in] nWriteTimeout IO write timeout £¨(unit: MS)
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600UsbSetTimeout(int nReadTimeout, int nWriteTimeout);


/**
 * @brief  enumerate TCP printer
 * @par    Description£º
 *  Make sure that TCP printer is connected
 * @param [out] szEnumList if there're multiple printers, they should be separated by "\\n"
 * @param [out] pEnumListLen Enumeration list length
 * @param [out] pNum the number of Enumerating TCP printers
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600EnumTcpPrt(char *szEnumList, unsigned int *pEnumListLen, int *pNum);


/**
 * @brief  Set the TCP port timeout
 * @par     Description£º
 * the default value is 3000ms, if it has not been set up
 * @param [in] nReadTimeout IO read timeout(unit: MS)
 * @param [in] nWriteTimeout IO write timeout £¨(unit: MS)
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600TcpSetTimeout(int nReadTimeout, int nWriteTimeout);


/**
 * @brief  choose printer
 * @par    Description£º
 * This interface can support both USB port and network port
 * @param [in] szPrt Enumerate the name of a single printer which has been enumerated interface
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600SelectPrt(const char *szPrt);	


/**
 * @brief  @brief  printer reset
 * @par    Description£º
 * Clears current error. Cannot clean cache
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600PrtReset();


/**
 * @brief  soft restart printer
 * @par    Description£º
 * To enable the machine to restart without power, including clearing the cache
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600PrtRestart();

/**
 *
 * @}
 */


/** @defgroup InfoQuery Query the printer information
 *  @brief Get the printer serial number, get the equipment manufacturing information, get the printer information, query the status of the printer, query whether there is a card inside the printer, etc
 *
 *  @{
 *
 */

 /**
 * @brief  Check whether the card slot is empty
 * @par    Description£º
 * Check whether the card slot is empty\n
 * @param [out] pFlag 0£ºno card on card slot£»1£ºcards on the card slot
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600IsFeederNoEmpty(int *pFlag);

/**
 * @brief  get the equipment manufacturing information
 * @par    Description£º
 * Query the printer name, manufacturer name,printer serial number, manufacture date, FW version and so on\n
 * If the above information is not needed, the buffer and the pointer variable describing the buffer length will be NULL.
 * @param [out] szPrtName printer name
 * @param [in,out] pPrtNameLen the length of printer name
 * @param [out] szManuf Manufacuturer name
 * @param [in,out] pManufLen the length of Manufacuturer name
 * @param [out] szPrtSN printer serial number
 * @param [in,out] pPrtSNLen the length of printer serial number
 * @param [out] szDate Manufacture date
 * @param [in,out] pDateLen the length of Manufacture date
 * @param [out] szMainFWVer main FW version
 * @param [in,out] pMainFWVerLen the length of main FW version
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600GetPrtManufInfo(char *szPrtName
										, int *pPrtNameLen
										, char *szManuf
										, int *pManufLen
										, char *szPrtSN
										, int *pPrtSNLen
										, char *szDate
										, int *pDateLen
										, char *szMainFWVer
										, int *pMainFWVerLen);


/**
 * @brief  get the printer information
 * @par    Description£º
 * Query the printing times of print head, the prinring time of cards, main FW version, FPGA version number\n
 * If the following parameters are not needed, just enter NULL.
 * @param [out] pPrintheadPrintCount  printing times of print head
 * @param [out] pCardPrintCount card printer printing times
 * @param [out] pMainFWVer main FW version, for example, the main FW version is 10.07.00, then the value of pMainFWVer should be 0x100700.
 * @param [out] pSubFWVer Sub FW version, if the sub FW version is 01.0f.00.00, the value of pSubFWVer should be 0x010f0000.
 * @param [out] pFPGAVer FPGAversion, if FPGA version is 01.08.42, the value of pFPGAVer should be 0x2a0801. * @return Zero means success, non-zero means failure
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600GetPrtInfo(unsigned int *pPrintheadPrintCount, unsigned int *pCardPrintCount, unsigned int *pMainFWVer, unsigned int *pSubFWVer, unsigned int *pFPGAVer);


/**
 * @brief  Query the printer status
 * @par    Description£º
 * If the following parameters are not needed, just enter NULL.
 * @param [out] pChassisTemp Chassis temperature
 * @param [out] pPrintheadTemp print head temperature
 * @param [out] pHeaterTemp heater temperature
 * @param [out] pMainStatus main status of printer
 * @param [out] pSubStatus sub status of printer
 * @param [out] pErrorStatus error status of printer
 * @param [out] pWarningStatus printer warning status
 * @param [out] pMainCode main error code
 * @param [out] pSubCode Sub error code
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600QueryPrtStatus(short *pChassisTemp
										, short *pPrintheadTemp
										, short *pHeaterTemp
										, unsigned int *pMainStatus
										, unsigned int *pSubStatus
										, unsigned int *pErrorStatus
										, unsigned int *pWarningStatus
										, unsigned char *pMainCode
										, unsigned char *pSubCode);


/**
 * @brief  Query printer ribbon balance
 * @par    Description£º
 * Query interface
 * @param [out] pRbnRemaining ribbon balance(unit£º%)
 * @param [out] pFilmRemaining film balance(unit£º%)
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600GetRbnAndFilmRemaining(unsigned short* pRbnRemaining, unsigned short* pFilmRemaining);


/**
* @brief  Gets an error code interpretation
* @par    Description£º
*Gets the Chinese interpretation of the corresponding error according to the error code
* @param [in] errcode error code
*@param [out] outputstr Output string buffer
*@param [in,out] len Specifies the length of the buffer, which returns the length of the string written to the buffer
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600GetErrorOuterInfo(unsigned int errcode, char*outputstr, int*len);


/**
* @brief  Gets the status code interpretation
* @par    Description£º
*Get the Chinese interpretation of the corresponding state according to the status code
* @param [in] statusCode status code
*@param [in] outputstr Output string buffer
*@param [in,out] len Specifies the length of the buffer, which returns the length of the string written to the buffer
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600GetCheckStatusMsg(unsigned int statusCode, char*outputstr, int*len);



/**
 *
 * @}
 */


 /** @defgroup PrinterSettingsRW Printer Settings read and write
  *  @brief ribbon setting,printer setting, card setting.
  *
  *  @{
  *
  */

/**
 * @brief  Read/set the ribbon type and balance notification values
 * @par    Description£º
 * Query the interface
 * @param [in] nMode Mode¡£ 0£ºread£»1£ºwrite¡£
 * @param [out] pRibbonType RibbonType£¬¡¾read¡¿
 * @param [out] pFilmType FilmType£¬¡¾read¡¿
 * @param [in,out] pRibbonNearEnd ribbon balance notification values(unit:%)
 * @param [in,out] pFilmNearEnd film balance notification values(unit:%)
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600RibbonSettingsRW(int nMode, unsigned char* pRibbonType, unsigned char* pFilmType, unsigned char* pRibbonNearEnd, unsigned char* pFilmNearEnd);


/**
 * @brief  read/set card settings
 * @par    Description£º
 * read/set card type, normal card ejectment, abnormal card ejectment, card loading
 * @param [in] nMode mode¡£ 0£ºread£»1£ºwrite¡£
 * @param [in,out] pCardType card type¡£
 * @n 0£ºPVC£»
 * @n 1£ºPET£»
 * @n 2£ºPET-G£»
 * @n 3£ºC1¡£
 * @n 4£ºC2¡£
 * @n 5£ºC3¡£
 * @param [in,out] pFontOrthTime Frontal orthopedic time 0~5
 * @param [in,out] pBackOrthTime   Back side orthopedic time 0~5
 * @param [in,out] pStdCardExitPos standard card exit position¡£ eject card on left side:0x00£»eject card on the right lower side:0x01£»eject card on right top side£º0x02£»Vertical slot for card ejectment£º0x03£»
 * @param [in,out] pErrorCardExitPos error card exit position¡£  eject card on left side:0x00£»eject card on the right side:0x01£»eject card on the Right horizontal position£º0x02£»Vertical slot for card ejectment£º0x03£»
 * @param [in,out] pSetCardPos  The card box enters the card automatically:0x00£»Manual entry on the right side:0x01£»Automatic entry on the right side:0x02£»Automatic entry on left side£º0x03£»
 * @param [in,out] pCardEjectSide automation:0x00£»positive upward:0x01£»back up:0x02£»
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600CardSettingsRW(int nMode, unsigned short* pCardType, unsigned short* pFontOrthTime, unsigned short* pBackOrthTime, unsigned short* pStdCardExitPos,
	unsigned short* pErrorCardExitPos, unsigned short* pSetCardPos, unsigned short* pCardEjectSide);


/**
* @brief  read/set network related settings
* @par    Description£º
*
* @param [in] nMode mode¡£ 0£ºread£»1£ºwrite¡£
* @param [in,out] pPrinterID printer ID¡£
* @param [in,out] pIPAddressType IPStatic/dynamic fetch
* @param [in,out] pStaticIPAddress Static IP address
* @param [in,out] pGatewayIPAddress GatewayIPAddress
* @param [in,out] pSubnetMask SubnetMask
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600EthernetSetup(int nMode
									, unsigned char *pPrinterID
									, unsigned char *pIPAddressType
									, unsigned int *pStaticIPAddress
									, unsigned int *pGatewayIPAddress
									, unsigned int *pSubnetMask);

/**
* @brief  Gets the printer or print head serial number
* @par     Description£º
*
* @param [in] Dev mode¡£ 2£ºprinter£»5£ºprint head¡£
* @param [out] szSN serial number buffer
* @param [in,out] szSNLen lenghth of input serial number buffer lenghth of output serial number buffer
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600GetSN(char Dev, char*szSN, int*szSNLen);

/**
 *
 * @}
 */


 /** @defgroup Printing printing function
  *  @brief text¡¢barcode,QR code, graphic combined-printing
  *
  *  @{
  *
  */


  /** @defgroup Canvas Canvas
   *  @ingroup Printing
   *  @brief Create the canvas, clear the canvas, and submit the canvas
   *
   *  @{
   *
   */

  /**
  * @brief  Create the canvas
  * @par    Description£º
  * start drawing.Note: This function is thread-related, meaning it is only valid on the calling thread
  * @param [in] nChromaticMode Color (YMC) print mode¡£
  * @n 0 600 * 600dpi
  * @param [in] nMonoChroMode Monochrome (K) print mode¡£
  * @n 0 600 * 600dpi
  * @return Zero means success, non-zero means failure
  */
unsigned int DSSDK R600PrepareCanvas(int nChromaticMode, int nMonoChroMode);


/**
* @brief  clear the canvas
* @par    Description£º
* Clears the drawing content so that it can be redrawn.Note: This function is thread-related, meaning it is only valid on the calling thread
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600ClearCanvas();


/**
 * @brief  submit the canvas
 * @par    Description£º
 * To end the drawing, return the description information of the related image data generated by the drawing, which is used for the incoming parameter of R600PrintDraw.
 * The storage limit is only 2 canvases, if the canvas is submitted multiple times, the last 2 canvases will be taken.Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [out] szImgInfo Description information of related image data generated by this painting
 * @param [in,out] pImgInfoLen Description information buffer length information of relevant image data generated by this painting
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600CommitCanvas(char *szImgInfo, int *pImgInfoLen);


/**
* @brief  This paramter uis used for setting the size of card and the offset in the canvas (canvas size fixed, 2112*1324),the default is the size of CR80 card £¨2022*1275 85.6mm*53.98mm£©,located in the center of the canvas(the default offset is xoffset=45£¬yoffset=25).
* @par    Description:
* Once set, this function takes effect globally!The function also wroks in two ways: when printing over the entire background image of card,if there are white edges, you can set xoffset=45-15=30£¬yoffset=25-15=10£¬cardSizeWidth=2022+15*2=2052
* cardSizeHeight=1275+15*2=1305,the width and height of the print range are increased by 30 points,1.27mm,the range is changed from 85.6mm*53.98mm to 86.87mm*55.25mm.
* Of course, the above is just for reference. The offset value and card size can be adjusted flexibly,be careful not to go beyond the canvas (2112*1324, or 89.408mm*56.049mm).Second, it can not be set to greater than Cr80Extra
* Any card type, such as Cr79 (because a card canvas larger than Cr80Extra won't fit)
* @param [in] xoffset  X-axis offset (per pixel)
* @param [in] yoffset  y-axis offset (per pixel)
* @param [in] cardSizeWidth  Card width (per pixel)
* @param [in] cardSizeHeight  Area height (per pixel)
* @return Zero means success, non-zero means failure
*/

unsigned int DSSDK R600CardSizeAndOffsetSetting(int xoffset, int yoffset, int cardSizeWidth, int cardSizeHeight);


/**
* @brief  set the print area of H layer or I layer of YMCKH or YMCKI
* @par    Description£º
* @When x,y,width and height are all 0, it means to clear all the areas set before. For YMCKI, because the rectangular area to be shredded needs to print I layer, that is, the YMC layer and K layer should not be printed where the I layer is printed, so in this case, the isMeansErase of YMC and K region must be contrary to the isMeansErase of O, otherwise the setting is invalid!
* @param [in] x  The x axis starting point£¨unit mm£©
* @param [in] y  The y axis starting point£¨unit mm£©
* @param [in] width  width£¨unit mm£©
* @param [in] height height£¨unit mm£©
* @param [in] isFront  1 means front£¬0 means back
* @param [in] isMeansErase  1 is currently set as a hollowed area and 0 is currently set as a filled area. (Note: the two areas cannot exist at the same time)
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetCoatRgn(double x, double y, double width, double height, byte isFront, byte isMeansErase);


/**
* @brief  set the printing area of YMC
* @par    Description£º
* @if x,y,width,height is 0, means all the area are clear.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] x  The x axis starting point£¨unit mm£©
* @param [in] y  The y axis starting point£¨unit mm£©
* @param [in] width  width  width£¨unit mm£©
* @param [in] height height height£¨unit mm£©
* @param [in] isFront  1 means 0 means back
* @param [in] isMeansErase  1 is currently set as a hollowed area and 0 is currently set as a filled area. (Note: the two areas cannot exist at the same time)
* @param [in] NotPrintMixBlack 1 means yMC area can not use the black mixted by YMC, for the area can use both yMC and K, just remove he black mixted by YMC, default value is 0.
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetYmcRgn(double x, double y, double width, double height, byte isFront, byte isMeansErase , byte  NotPrintMixBlack);

/**
* @brief  set the printing area of K
* @par    Description£º
* @if x,y,width,height is 0, means all the area are clear.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] x  The x axis starting point£¨unit mm£©
* @param [in] y  The y axis starting point£¨unit mm£©
* @param [in] width  width  width  width£¨unit mm£©
* @param [in] height height height height£¨unit mm£©
* @param [in] isFront  1 means front£¬0 means back
* @param [in] isMeansErase   1 is currently set as a hollowed area and 0 is currently set as a filled area. (Note: the two areas cannot exist at the same time)
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetKRgn(double x, double y, double width, double height, byte isFront, byte isMeansErase);

/**
* @brief  offset of half grid ribbon
* @par    Description£º
* @param [in] offset Half grid ribbon lateral offset, unit mm, greater than or equal to 0 mm, not more than 43.0106 mm
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetHalfRibbonOffset(double offset);

/**
 *
 * @}
 */


 /** @defgroup DrawSettings DrawSettings
  *  @ingroup Printing
  *  @brief Bar code drawing Settings, text drawing Settings
  *
  *  @{
  *
  */


/**
 * @brief  Set the barcode \ QR code color
 * @par    Description£º
 * When not called, it defaults to black.Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [in] nR R value 0~255
 * @param [in] nG G value 0~255
 * @param [in] nB N value 0~255
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600SetCodeColor(int nR, int nG, int nB);


/**
* @brief  Set bar code parameters
* @par    Description£º
* Default CODE128 when not called; No comments added; Not rotating.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] nSymbology barcode type
* @n 8	CODE39
* @n 16	EAN128
* @n 20	CODE128
* @n 25	CODE93
* @n 34	UPCA
* @n 37	UPCE
* @n 55	PDF417
* @n 72	EAN14
* @param [in] nHumanRead Whether to add comments or not. 0: No comments added; 1: Add comments.
* @param [in] nRotation rotation£¬0£¬90£¬180£¬270
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetBarCodePara(int nSymbology, int nHumanRead, int nRotation);


/**
* @brief  set the parameter of QR bar
* @par    Description£º
* When not called, the default level is 4.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] iErrorCorrectLevel Fault tolerance level. Level range 1 to 4
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetQRCodePara(int iErrorCorrectLevel);


/**
 * @brief  set the font type \ size ~4
 * @par    Description£º
 * When not called, the default is song; 12.Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [in] szFontName  set the font type such as: "song" "black"
 * @param [in] fSize set font size
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600SetFont(const wchar_t *szFontName, float fSize);


/**
 * @brief  set the font color
 * @par    Description£º
 * When not called, it defaults to black.Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [in] nR R value range 0~255
 * @param [in] nG G value range 0~255
 * @param [in] nB N value range 0~255
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600SetTextColor(int nR, int nG, int nB);


/**
* @brief  Sets whether the font is bold.
* @par    Description£º
* combined-printing interface.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] nStrong whether the font is bold 0£ºNo£»1£ºYes
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetTextIsStrong(int nStrong);


/**
* @brief  Sets the font boldness
* @par    Description£º
* combined-printing interface.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] nFontWeights Degree of boldness. The range is -3~5, and the normal size is 0
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetTextFontWeight(int nFontWeights);

/**
* @brief  Sets whether the font is italic
* @par    Description£º
* When not called, italics are not used by default.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] nItalic whether the font is italic 0£ºNo£»1£ºYes
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetTextIsItalic(int nItalic);


/**
* @brief  Sets whether the font is underlined
* @par    Description£º
* When not called, no underscore is used by default.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] nUnderline whether the font is underlined 0£ºNo£»1£ºYes
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetTextIsUnderline(int nUnderline);


/**
 * @brief  Set the font decoration
 * @par    Description£º
 * combined-printing interface.Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [in] nLandScape Whether it's horizontal or not. 0£ºNo£»1£ºYes
 * @param [in] nReverseSequence Whether in reverse order.0£ºNo£»1£ºYes
 * @param [in] nAutoLineFeed Whether line feed automatically. 0£ºNo£»1£ºYes
 * @param [in] nLayDown Whether to lie flat font. 0: Non-flat font; 1: Flat font
 * @returnZero means success, non-zero means failure
 */
unsigned int DSSDK R600SetTextDecorate(int nLandScape, int nReverseSequence, int nAutoLineFeed, int nLayDown);


/**
 * @brief  Set font spacing \ line spacing
 * @par    Description£º
 * combined-printing interface.Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [in] dLineSpacing Row spacing (in pounds)
 * @param [in] dCharSpacing Font size (in pounds)
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600SetTextSpacing(double dLineSpacing, double dCharSpacing);


/**
* @brief  Set font rotation
* @par    Description£º
* When not called, the default is not to rotate.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] nRotation Rotation£¬0£¬90£¬180£¬270
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetTextRotation(int nRotation);


/**
* @brief  Set the add picture parameters
* @par     Description£º
* combined-printing interface.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] nWhiteTransparent Whether white is transparent or not. 0: White opacity; 1: White transparent.
* @param [in] nRotation Rotation£¬0<x<360
* @param [in] fScale Magnification. Value range: greater than or equal to 0.
* @n At this time, dWidth and dHeight will be ignored. The target width and height will be obtained by scaling the original image width and height with fScale. When fScale is 1, the original image will be represented.
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetImagePara(int nWhiteTransparent, int nRotation, float fScale);



/**
* @brief  Set watermark parameters
* @par    Description£º
* combined-printing interface.
* @param [in] nRotation Rotation£¬0<x<360
* @param [in] isNeedMirror whether it needs to mirror transformation¡£
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetWaterMarkParam(int nRotation, unsigned char isNeedMirror);


/**
* @brief  Set watermark threshold
* @par     Description £º
* Set watermark threshold. Get rid off the parti-colour.
* @param[in] Threshold 0-255
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetWaterMarkThreshold(int Threshold);


/**
* @brief  Image processing dispose Halftone by threshold, ordered dither and error diffusion; ;
* @par     Description £º
* When invoked, the default normal mode. Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param[in] nMode
* @n  1: colour error diffusion;
* @n  2: colour ordered dithering;
* @n  3: colour threshold;
* @param[in] nValueThreshold Value of threshold
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetAddImageMode(int nMode, int nValueThreshold);


/**
 *
 * @}
 */



/** @defgroup Drawing Drawing
 *  @ingroup Printing
 *  @brief Bar code drawing, two-dimensional code drawing, picture drawing, text drawing
 *
 *  @{
 *
 */


/**
 * @brief   Bar code drawing
 * @par     Description£º
 * combined-printing interface Bar code type/whether to add comments/rotate/color parameters in R600SetBarCodePara/R600SetCodeColor place Settings.Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [in] dX coordinate x (unit:MM)
 * @param [in] dY coordinate y(unit:MM)
 * @param [in] dWidth Width of Bar code(unit:MM)
 * @param [in] dHeight Height of Bar code(unit:MM)
 * @param [in] szData bar code data
 * @param [in] nSetNoAbsoluteBlack Does not use K color. 0: Print in K color; 1: Do not print in K color [recommended].
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600DrawBarCode(double dX, double dY, double dWidth, double dHeight, const char *szData, int nSetNoAbsoluteBlack);


/**
 * @brief  two-dimensional code drawing
 * @par    Description£º
 * combined-printing interface Sets the fault tolerance level/color parameter on R600SetQRCodePara/R600SetCodeColor.Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [in] dX coordinate x (unit:MM)
 * @param [in] dY  coordinate y(unit:MM)
 * @param [in] dWidth Width of  two-dimensional code (unit:MM)
 * @param [in] dHeight Height of two-dimensional code (unit:MM)
 * @param [in] szData data of two-dimensional code
 * @param [in] nSetNoAbsoluteBlack Does not use K color. 0: Print in K color; 1: Do not print in K color [recommended].
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600DrawQRCode(double dX, double dY, double dWidth, double dHeight, const char *szData, int nSetNoAbsoluteBlack);


/**
 * @brief   picture drawing
 * @par     Description£º
 * When x,y coordinates and width and height are 0, the default is the background image size.
 * The white transparency/rotation/magnification parameter is set at R600SetImagePara.
 * When dX¡¢dY¡¢dWidth¡¢dHeight is set to 0,means add the background
 * Supported image formats: .bmp .wbmp(1bit) .gif .jpg .jpeg .jpe .jfif .png .mng .jng .ico .cur .tif .tiff .tga .pcx
 * .j2k .jp2 .j2c .jpc .pgx(grey-scale image) .ras .pnm .pgm .ppm .ska .wmf .emf .psd
 * supported Lossless image format: .mrw .dng .cr2 .nef .crw .raf .pef .mef .raw .orf .sr2
 * Note: This function is thread-related, meaning it is only valid on the calling thread.
 * @param [in] dX coordinate x (unit:MM)
 * @param [in] dY coordinate y(unit:MM)
 * @param [in] dWidth The width drawn to the canvas (unit:MM)
 * @param [in] dHeight The height drawn to the canvas (unit:MM)
 * @param [in] szImgFilePath graphic path
 * @param [in] nSetNoAbsoluteBlack Does not use K color. 0: Print in K color; 1: Do not print in K color [recommended].
 * @n 0 Draws to the canvas with the width and height specified by dWidth and dHeight.
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600DrawImage(double dX
								, double dY
								, double dWidth
								, double dHeight
								, const char *szImgFilePath
								, int nSetNoAbsoluteBlack);


/**
 * @brief  text drawing
 * @par     Description£º
 * combined-printing interface
 * The font type/font parameter is set at R600SetFont.
 * Sets the font color parameter on R600SetTextColor
 * Sets the font bold/italic/underline parameters on R600SetTextIsStrong/R600SetTextIsItalic/R600SetTextIsUnderline
 * Sets the font font spacing/line spacing parameters on R600SetTextSpacing
 * Sets the font modifier parameters on R600SetTextDirection
 * Sets the font rotation parameters on R600SetTextRotation
 * Note: This function is thread-related, meaning it is only valid on the calling thread
 * @param [in] dX coordinate x (unit:MM)
 * @param [in] dY coordinate y(unit:MM)
 * @param [in] width Textbox width (unit:MM)
 * @param [in] height Textbox height (unit:MM)
 * @param [in] szText text,content
 * @param [in] nSetNoAbsoluteBlack Does not use K color. 0: Print in K color; 1: Do not print in K color [recommended].
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600DrawText(double dX, double dY, double width, double height, const wchar_t *szText, int nSetNoAbsoluteBlack);




/**
* @brief  watermark drawing
* @par    Description£º
* combined-printing interface
 * @param [in] dX coordinate x (unit:MM)
 * @param [in] dY coordinate y(unit:MM)
 * @param [in] width watermark width (unit:MM)
 * @param [in] height watermark height (unit:MM)
* @param [in] szImgFilePath the path of watermark 
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600DrawWaterMark(double dX, double dY, double width, double height, const char *szImgFilePath);

/**
 *
 * @}
 */


/**
* @brief  Print the image data generated by the drawing content
* @par    Description£º
* This function interface is used to print the image data generated by the nesting drawing. It cannot be used to print the image file.
* Parameters in error diffusion/ordered dither/threshold mode are set at R600SetAddImageMode.Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param [in] szImgInfoFront Combined-print and create the description information of related image data£¨from R600CommitCanvas return parameter szImgInfo£©£¬Used for printing on the front of CARDS
* @param [in] szImgInfoBack Combined-print and create the description information of related image data£¨from R600CommitCanvasreturn parameter szImgInfo£©£¬Used for printing on the back of CARDS
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600PrintDraw(const char *szImgInfoFront, const char *szImgInfoBack);



/**
* @brief  Gets the path where the preview is
* @par    Description£º
* Gets the path where the preview is.Note: This function is thread-related, meaning it is only valid on the calling thread
* @param [in] szImgInfo Combined-print and create the description information of related image data£¨from R600CommitCanvas return parameter szImgInfo£©
* @param [out] preview Preview path
* @param [in,out] previewLen the length of Previewing path
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600GetPreviewImg(const char* szImgInfo, char* preview, int* previewLen);


/**
* @brief  Set the ribbon-related parameter
* @par    Description£º
* Set the parameters related to the ribbon. At present, only the value of key can only be taken as 0, 
* and other values will be supplemented by iteration in the future
*
* <table>
* <tr>
* <th><em>key</em></th>
* <th><em>value</em></th>
* <th><em>explanation</em></th>
* </tr>
*
* <tr>
* <td>0</td>
* <td>"0"</td>
* <td>Indicates that the printing mode of YMCS ribbon is blank transfer and then printing </td>
* </tr>
*
* <tr>
* <td>0</td>
* <td>"1"</td>
* <td>Indicates that the printing mode of YMCS ribbon is print first and then blank transfer</td>
* </tr>
*
* <tr>
* <td>0</td>
* <td>"2"</td>
* <td>disabled</td>
* </tr>
*
* <tr>
* <td>1</td>
* <td>"1-255"</td>
* <td>thickness of watermark</td>
* </tr>
*
* </table>
* @param [in] isWrite 0£ºread£»1£ºwrite¡£
* @param [in] key keyValue
* @param [in,out] value value£¬It is an outgoing parameter when it is obtained and an incoming parameter when it is set
* @param [in] valueLen value's size
* @return zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetRibbonOpt(unsigned char isWrite, unsigned int key, char* value, unsigned int valueLen);



/**
 *
 * @}
 */


 /** @defgroup CardMoveAndPos Card movement control
  *  @brief Enter card, move card, arrange card, get card position, etc
  *
  *  @{
  *
  */


/**
 * @brief  load cards into printer
 * @par    Description£º
 * his interface will integrate into the target position of the card, the current status of the printer, whether there is a card inside the printer (including the card reader), whether there is a card in the card box, and the completion of the actual card arrangement£»\n
 * If the current printer reports an error, the card box is empty, and there is a card inside the printer (including the card reader), the card entry fails and the corresponding error code is returned.
 * @param [in] ucDestPos target,location
 * @n  0 - Loading card to transfer position
 * @n  1 - Loading card to 3 in 1 stop position
 * @n  2 - Loading card to 3 in 1 contacting position
 * @n  3 - Loading card to 3 in 1 non-contacting position
 * @n  4 - Loading card to 3 in 1 magnetic card position
 * @n  5 - Loading card to non-contacting card reader  
 * @n  6 - Loading card to contacting card reader
 * @n  7 - The card is flipped to the transfer position by the flipper
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600CardInject(unsigned char ucDestPos); 


/**
 * @brief  The card moves inside (including the reader)
 * @par    Description£º
 * The interface can judge the current status of the printer, the current position of the card inside the printer, the target position of the mobile operation and other factors.\n
 * If some conditions are not met, the card is removed and the appropriate error code is returned.
 * @param [in] ucDestPos target position
 * @n  0 - 3 in 1 move to transfer position
 * @n  1 - move to 3 in 1 contacting position
 * @n  2 - move 3 in 1 non-contacting position
 * @n  3 - move to 3 in 1 magnetic card position
 * @n  4 - The non-connecting card reader is disconnected to the transfer position
 * @n  5 - Loading card to non-contacting card reader
 * @n  6 - Loading card to contacting card reader
 * @n  7 - Contacting card reader contact displacement to transfer position
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600CardMove(unsigned char ucDestPos);


/**
 * @brief  printer ejects cards
 * @par    Description£º
 * The interface can judge the current status of the printer, whether there is a card inside the printer (including the card reader), the completion of the actual card arrangement and other factors.\n
 * If card ejectment fails, an error code will return.
 * @param [in] ucDestPos target position
 * @n  0 - eject card on left side
 * @n  1 - eject card on right lower side
 * @n  2 - eject card on right top side
 * @n  3 - Vertical card slot out card
 * @n  4 - Set the card according to the panel
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600CardEject(unsigned char ucDestPos);


/**
 * @brief  Roll over module
 * @par    Description£º
 * Printer action
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600CardTurnover();


/**
 * @brief Gets the current position of the card
 * @par    Description£º
 * Gets the position of the card inside the printer
 * @param [out] pPos The position of the card inside the printer
 * @n 0 - The card is not inside the printer
 * @n 1 - The card reads the slot
 * @n 2 - The card is in the transfer position
 * @n 3 - The card is in the transfer completion position
 * @n 4 - The card is in the 3 in 1 card reader
 * @n 5 - Card position unknown (printer to support "business card printing" status)
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600GetCardPos(int *pPos);

/**
 *
 * @}
 */


 /** @defgroup CardReadWrite Card read-Write interaction
  *  @brief Gets the card type, serial number, initialization, M1 card, CPU card, magnetic strip card read and write, etc
  *
  *  @{
  *
  */

/**
 * @brief  Gets the card type
 * @par    Description£º
 * Card reader action
 * @param [out] pType card type
 * @n  0x00 - Indicates that the card cannot be detected.
 * @n  0x01 - Indicates no card in the device
 * @n  0x11 - Type A CPU Card.
 * @n  0x13 - Represents Type A Mifare S50
 * @n  0x14 - Represents Type A Mifare S70
 * @n  0x15 - Type A Mifare Ultralight
 * @n  0x21 - Type B CPU Card
 * @n  0x22 - Type B memory card
 * @n  0x31 - contact T=0 CPU Card¡£
 * @n  0x32 - contact T=1 CPU Card¡£
 * @n  0x41 - 4442 Card¡£
 * @n  0x42 - 4428 Card¡£
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600GetCardType(int *pType);


/**
 * @brief  M1 card gets the card serial number
 * @par    Description£º
 * Card reader action
 * @param [in] ucMode mode 0x00: Means to operate on the free card; 0x01: Represents the operation on all CARDS
 * @param [out] pSnrstr Returns the serial number of the card in hex.¡¾It's essentially a string¡¿
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600M1CardGetSN(unsigned char ucMode, unsigned char *pSnrstr);


/**
 * @brief  M1Card password authentication
 * @par    Description£º
 * Use the incoming password to verify the M1 card password
 * @param [in] nIsHexPwd Verify the password is in hexadecimal form. 0: Non-hexadecimal; 1: Hexadecimal;
 * @param [in] ucPwdType 0x00 means verify A password, and 0x04 means verify B password
 * @param [in] ucAddr The sector number of the password to verify
 * @param [in] szPwd password. Non-hexadecimal is fixed at 6 bytes, hexadecimal is fixed at 12 bytes
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600M1CardPwdAuth(int nIsHexPwd, unsigned char ucPwdType, unsigned char ucAddr, const char *szPwd);


/**
 * @brief  M1 card read
 * @par    Description£º
 * Card reader action
 * @param [in] nIsHexReading Whether to read in hexadecimal form. 0: Non-hexadecimal; 1: Hexadecimal;
 * @param [in] ucAdr address
 * @n M1¿¨ - S50block address£¨0~63£©£¬S70 block address£¨0~255£©¡£
 * @n ML¿¨ - page address£¨0~11£©¡£
 * @param [out] pData data¡£¡¾note¡¿Fixed return of 16 bytes of data, real data may be less than 16 bytes.
 * @returnZero means success, non-zero means failure
 */
unsigned int DSSDK R600M1CardRead(int nIsHexReading, unsigned char ucAdr, char *pData);


/**
 * @brief  write M1 card
 * @par    Description£º
 * card reader operation
 * @param [in] nIsHexWritting Whether to write in hexadecimal form
 * @param [in] ucAdr address
 * @n M1card - S50 block address£¨1~63£©£¬S70 block address£¨1~255£©¡£
 * @n MLcard - page address£¨2~11£©¡£
 * @param [in] pData data¡£¡¾note¡¿Fixed 16 bytes of data passed in, the real data may be less than 16 bytes.
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600M1CardWrite(int nIsHexWritting, unsigned char ucAdr, const char *pData);


/**
 * @brief  M1card stop operation
 * @par    Description£º
 *Make the card enter the termination state. At this time, you must move the card out of the induction zone and put it in the induction zone again to find the card
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600M1CardHalt();


/**
 * @brief  CPUcard reset
 * @par    Description£º
 * card reader operation
 * @param [in] nSlotNum Fill in 0 and interact with the user card. Fill in the card holder number and interact with the corresponding PSAM card
 * @n  0x0C - Attached card slot/contacting card slot
 * @n  0x0B - contacting CPU2card slot¡£
 * @n  0x0D - SAM1card slot¡£
 * @n  0x0E - SAM2card slot¡£
 * @n  0x0F - SAM3card slot¡£
 * @n  0x11 - SAM4card slot¡£
 * @n  0x12 - SAM5card slot¡£
 * @n  0x13 - SAM6card slot/ESAM chip¡£
 * @n  0x14 - SAM7card slot¡£
 * @n  0x15 - SAM8card slot
 * @param [out] pResetInfo reset information
 * @param [out] pResetInfolen the length of reset information
 * @param [out] pUid Returns the card serial number if the card is non-connecting
 * @param [out] pUidLen serial number
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600CPUCardReset(int nSlotNum, char *pResetInfo, unsigned char *pResetInfolen, char *pUid, unsigned char *pUidLen);


/**
 * @brief  CPU card interaction
 * @par    Description£º
 * Card reader action
 * @param [in] nSlotNum Fill in 0 and interact with the user card. Fill in the card holder number and interact with the corresponding PSAM card
 * @n  0x0C - Attached card slot/contacting card slot
 * @n  0x0B - contacting CPU2card slot¡£
 * @n  0x0D - SAM1card slot¡£
 * @n  0x0E - SAM2card slot¡£
 * @n  0x0F - SAM3card slot¡£
 * @n  0x11 - SAM4card slot¡£
 * @n  0x12 - SAM5card slot¡£
 * @n  0x13 - SAM6card slot/ESAM chip¡£
 * @n  0x14 - SAM7card slot¡£
 * @n  0x15 - SAM8card slot
 * @param [in] pSApdu Apdu instruction to send
 * @param [in] ucApduLen Length of apDU instruction sent
 * @param [out] pRApdu The buffer that receives the APDU
 * @param [in,out] pRApduLen Returns the length of the received APDU instruction
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600CPUCardExchangeAPDU(int nSlotNum, unsigned char *pSApdu, unsigned char ucApduLen, char *pRApdu, unsigned char *pRApduLen);


/**
 * @brief  Read the magnetic stripe card
 * @par    Description£º
 *Read magnetic strip card data, support 1, 2, 3 tracks
 * @param [in] ucTimeout Device timeout value (unit: second)
 * @param [out] pTrack1Data Returns 1 track of data in the format '\0' ending string
 * @param [out] pTrack1DataLen Returns the length of 1 track data, without '\0'
 * @param [out] pTrack2Data Returns 2 tracks of data in the form of a string ending '\0'
 * @param [out] pTrack2DataLen Returns the length of 2 track data, without '\0'
 * @param [out] pTrack3Data Returns 3 tracks of data in the form of a string ending '\0'
 * @param [out] pTrack3DataLen Returns the length of the 3-track data, without '\0'
 * @return Zero means success, non-zero means failure
 */
unsigned int DSSDK R600MAGCardRead(unsigned char ucTimeout
									, unsigned char *pTrack1Data
									, unsigned int *pTrack1DataLen
									, unsigned char *pTrack2Data
									, unsigned int *pTrack2DataLen
									, unsigned char *pTrack3Data
									, unsigned int *pTrack3DataLen);

/**
*
* @}
*/

/** @defgroup newOrUpdateInterface New Or Update Interface
*  @brief 
*
*  @{
*
*/



/**
* @brief  Image processing dispose Halftone by threshold, ordered dither and error diffusion; set YMC and K print or not print; 
* @par     Description £º 
* When invoked, the default normal mode. Note: This function is thread-related, meaning it is only valid on the calling thread.
* @param[in] nMode 
* @n  1: colour error diffusion;
* @n  2: colour ordered dithering;
* @n  3: colour threshold;
* @param[in] YMC means whether print ymc colour ribbon; true means print, false means not print
* @param[in] K means whether print k colour ribbon; true means print, false means not print
* @param[in] ImagePath means dispose Halftone picture path; NULL means undispose; not NULL means dispose; if set path, It will generate a picture called "cccccccH1.png" in the path of the current project.
* @param[in] nValueThreshold Value of threshold
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetAddImageMode_Rtai(int nMode, bool YMC, bool K, const char* ImagePath, int nValueThreshold);

 


/**
* @brief  LayerFluor drawing
* @par    Description£º
* combined-printing interface
* @param [in] dX coordinate x (unit:MM)
* @param [in] dY coordinate y(unit:MM)
* @param [in] width LayerFluor width (unit:MM)
* @param [in] height LayerFluor height (unit:MM)
* @param [in] szImgFilePath the path of LayerFluor
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600DrawLayerFluor(double dX, double dY, double width, double height, const char *szImgFilePath);


/**
* @brief  Set LayerFluor threshold
* @par     Description£º
* Set LayerFluor threshold. Get rid off the parti-colour.
* @param[in] Threshold 0-255
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600SetLayerFluorThreshold(int Threshold);



/**
* @brief  Set Split Print
* @par     Description£º
* Set K Color Ribbon Split Print
* @param[in] flag, set true means Split Print; set false means not Split Print;
* @return Zero means success, non-zero means failure
*/
unsigned int R600SetImgUtilIsNeedSet_K_ColorRibbonSplitPrint(bool flag);




/**
* @brief  Layer Black drawing
* @par    Description£º
* combined-printing interface
* @param [in] dX coordinate x (unit:MM)
* @param [in] dY coordinate y(unit:MM)
* @param [in] width LayerBlack width (unit:MM)
* @param [in] height LayerBlack height (unit:MM)
* @param [in] szImgFilePath the path of LayerBlack
* @return Zero means success, non-zero means failure
*/
unsigned int DSSDK R600DrawLayerBlack(double dX, double dY, double width, double height, const char *szImgFilePath);




/**
*
* @}
*/

#ifdef __cplusplus
}
#endif

#endif