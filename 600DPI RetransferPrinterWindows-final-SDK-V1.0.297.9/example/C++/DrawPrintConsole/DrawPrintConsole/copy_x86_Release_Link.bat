
if exist ..\..\..\..\lib\libDSRetransfer600App.dll ( xcopy /F /Y ..\..\..\..\lib\libDSRetransfer600App.dll .\bin\Win32\Release  ) else ( xcopy /F /Y ..\..\..\..\libDSRetransfer600App\lib\Win32\Release\libDSRetransfer600App.dll .\bin\Win32\Release )
if exist ..\..\..\..\lib\Retransfer600_SDKCfg.xml ( xcopy /F /Y ..\..\..\..\lib\Retransfer600_SDKCfg.xml .\bin\Win32\Release  ) else ( xcopy /F /Y .\Retransfer600_SDKCfg.xml .\bin\Win32\Release )
if exist ..\..\..\..\lib\EWL ( xcopy /F /Y ..\..\..\..\lib\EWL .\bin\Win32\Release  ) else ( xcopy /F /Y .\EWL .\bin\Win32\Release )
if exist ..\..\..\..\lib\lcms2.dll ( xcopy /F /Y ..\..\..\..\lib\lcms2.dll .\bin\Win32\Release  ) else ( xcopy /F /Y ..\..\..\..\..\..\..\..\..\3PP\LCMS\lib\Windows\Win32\Release\lcms2.dll .\bin\Win32\Release  )
if exist ..\..\..\..\lib\DSRCPCLR_44_V2_1.icm ( xcopy /F /Y ..\..\..\..\lib\DSRCPCLR_44_V2_1.icm .\bin\Win32\Release  ) else ( xcopy /F /Y ..\..\..\..\..\..\..\..\..\3PP\LCMS\lib\Windows\Win32\Release\DSRCPCLR_44_V2_1.icm .\bin\Win32\Release  )
if exist ..\..\..\..\lib\dcrf32.dll ( xcopy /F /Y ..\..\..\..\lib\dcrf32.dll .\bin\Win32\Release  ) else ( xcopy /F /Y ..\..\..\..\..\..\..\..\..\3PP\CardRW\DeCard\dcrf\Windows\lib\Win32\dcrf32.dll .\bin\Win32\Release  )
if exist ..\..\..\..\lib\R600StatusReference ( xcopy /F /Y ..\..\..\..\lib\R600StatusReference .\bin\Win32\Release ) else ( xcopy /F /Y .\R600StatusReference .\bin\Win32\Release  )
xcopy /F /Y .\logo.png .\bin\Win32\Release
xcopy /F /Y .\��ǩ.png .\bin\Win32\Release
xcopy /F /Y .\ͷ��.png .\bin\Win32\Release
xcopy /F /Y .\ӡ��.png .\bin\Win32\Release
xcopy /F /Y .\���汳��.png .\bin\Win32\Release
xcopy /F /Y .\���汳��.jpg .\bin\Win32\Release
::pause