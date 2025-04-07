@echo off

cls
@echo loading.
set path=%path%;C:\WINDOWS\system32
cls
@echo loading..
xcopy ..\..\tools\gui\efont\exe\eFont.exe .\tools /s /e /y >nul 2>nul
xcopy ..\..\tools\gui\efont\exe\eFont_cmd.exe .\tools /s /e /y >nul 2>nul
cls
@echo loading...
xcopy ..\..\tools\gui\efs\exe\efs.exe .\tools /s /e /y >nul 2>nul
xcopy ..\..\tools\gui\eimage\exe\eImage_5.1.exe .\tools /s /e /y >nul 2>nul
cls
@echo loading....
xcopy ..\..\tools\gui\eimage\exe\eImage_5.2.exe .\tools /s /e /y >nul 2>nul
xcopy ..\..\tools\gui\elang\exe\elang.exe .\tools /s /e /y >nul 2>nul
cls
@echo loading.....
xcopy ..\..\tools\gui\ecompress\exe\ecompress.exe .\tools /s /e /y >nul 2>nul
xcopy ..\..\tools\gui\nv12otorgb\exe\YUVotoRGB.exe .\tools /s /e /y >nul 2>nul
cls
@echo loading......
xcopy ..\..\tools\gui\EUIEditor\exe\"*" .\tools /s /e /y >nul 2>nul
::if exist ".\tools\EUIEditor\config.ini" del .\tools\EUIEditor\config.ini

cls
cd .\tools\EUIEditor

@echo run EUIEditor......
start EUIEditor.exe

if not %errorlevel% == 0 (
@echo You need install NET Framework 4.0 !
@echo.
pause
)
