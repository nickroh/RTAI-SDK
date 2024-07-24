
if exist ..\..\..\..\lib\libDSRetransfer600App.dll ( xcopy /F /Y ..\..\..\..\lib\libDSRetransfer600App.dll .\bin\Win32\Debug  ) else ( xcopy /F /Y ..\..\..\..\libDSRetransfer600App\lib\Win32\Debug\libDSRetransfer600App.dll .\bin\Win32\Debug )
if exist ..\..\..\..\lib\Retransfer600_SDKCfg.xml ( xcopy /F /Y ..\..\..\..\lib\Retransfer600_SDKCfg.xml .\bin\Win32\Debug  ) else ( xcopy /F /Y .\Retransfer600_SDKCfg.xml .\bin\Win32\Debug )
if exist ..\..\..\..\lib\EWL ( xcopy /F /Y ..\..\..\..\lib\EWL .\bin\Win32\Debug  ) else ( xcopy /F /Y .\EWL .\bin\Win32\Debug )
if exist ..\..\..\..\lib\lcms2.dll ( xcopy /F /Y ..\..\..\..\lib\lcms2.dll .\bin\Win32\Debug  ) else ( xcopy /F /Y ..\..\..\..\..\..\..\..\..\3PP\LCMS\lib\Windows\Win32\Debug\lcms2.dll .\bin\Win32\Debug  )
if exist ..\..\..\..\lib\DSRCPCLR_44_V2_1.icm ( xcopy /F /Y ..\..\..\..\lib\DSRCPCLR_44_V2_1.icm .\bin\Win32\Debug  ) else ( xcopy /F /Y ..\..\..\..\..\..\..\..\..\3PP\LCMS\lib\Windows\Win32\Debug\DSRCPCLR_44_V2_1.icm .\bin\Win32\Debug  )
if exist ..\..\..\..\lib\dcrf32.dll ( xcopy /F /Y ..\..\..\..\lib\dcrf32.dll .\bin\Win32\Debug  ) else ( xcopy /F /Y ..\..\..\..\..\..\..\..\..\3PP\CardRW\DeCard\dcrf\Windows\lib\Win32\dcrf32.dll .\bin\Win32\Debug  )
if exist ..\..\..\..\lib\R600StatusReference ( xcopy /F /Y ..\..\..\..\lib\R600StatusReference .\bin\Win32\Debug ) else ( xcopy /F /Y .\R600StatusReference .\bin\Win32\Debug  )

::pause