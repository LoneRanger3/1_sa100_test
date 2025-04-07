
@echo off

if exist "C:\tools" goto make

md C:\tools

:make

@echo loading.
set path=%path%;C:\WINDOWS\system32
cls
@echo loading..
xcopy ..\eFont.exe C:\tools /s /e /y > nul
xcopy ..\eFont_cmd.exe C:\tools /s /e /y > nul
xcopy ..\efs.exe C:\tools /s /e /y > nul
cls
@echo loading...
xcopy ..\eImage_5.1.exe C:\tools /s /e /y > nul
xcopy ..\eImage_5.2.exe C:\tools /s /e /y > nul
cls
@echo loading....
xcopy ..\elang.exe C:\tools /s /e /y > nul
xcopy ..\ecompress.exe C:\tools /s /e /y > nul
cls
@echo loading.....
xcopy ..\YUVotoRGB.exe C:\tools /s /e /y > nul
xcopy ..\MSVCR71.dll C:\tools /s /e /y > nul
::xcopy ..\"*" C:\tools /s /e /y

cls
@echo loading......
cls

set /p make_img=If the image is changed, you need input 'y'.(y/n): 
if %make_img% == y (
@echo **************** start image ******************************
for /f "delims=" %%i in ('dir /b /a-d /s ..\..\res\*.png') do C:\tools\eImage_5.2.exe -m BGRA8888 %%i
@echo **************** end image ********************************

@echo .
@echo .
@echo .

@echo **************** start compress eimage ********************
for /f "delims=" %%i in ('dir /b /a-d /s ..\..\res\*.BGRA8888') do C:\tools\ecompress.exe -e %%i
@echo **************** end  compress eimage *********************

) else (
@echo **************** start image ******************************
@echo **************** end image ********************************
)

@echo .
@echo .
@echo .

@echo **************** start nv12 *******************************
for /f "delims=" %%i in ('dir /b /a-d /s ..\..\res\nv12\*.jpg') do C:\tools\YUVotoRGB.exe -Y %%i -1 -1
@echo **************** end nv12 *********************************

@echo .
@echo .
@echo .

@echo **************** start compress nv12 **********************
for /f "delims=" %%i in ('dir /b /a-d /s ..\..\res\*.nv12') do C:\tools\ecompress.exe -e %%i
@echo **************** end  compress nv12 ***********************

@echo .
@echo .
@echo .

@echo **************** start language ***************************
C:\tools\elang.exe -m ..\..\res\language\lang.xls ..\..\includes\lang.h ..\..\res\language\lang.el
@echo **************** end language *****************************

@echo .
@echo .
@echo .

@echo **************** start create efont *********************
cd ..\..\out\res
C:\tools\efont_cmd.exe
@echo **************** end create efont ***********************

@echo .
@echo .
@echo .

@echo **************** start compress efont *********************
for /f "delims=" %%i in ('dir /b /a-d /s ..\..\res\*.edf') do C:\tools\ecompress.exe -e %%i
@echo **************** end compress efont ***********************

@echo .
@echo .
@echo .

@echo **************** start pack efs ***************************
cd ..\..\out\res
C:\tools\efs.exe -m ..\..\res .ez,.el,.json
@echo **************** end pack efs *****************************


pause
