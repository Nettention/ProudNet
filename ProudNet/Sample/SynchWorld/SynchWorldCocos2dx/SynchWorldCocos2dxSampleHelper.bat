@echo off

set PROJECT_NAME=SynchWorldCocos2dx
set COMMON_DIR=Common

echo �ڼ��� ������� ProudNet ������ �����Ͽ� �ֽñ� �ٶ��ϴ�.

:input
set COCOS2D_VER=
set /p COCOS2D_VER="��ġ�Ͻ� Cocos2d-x�� ������ �Է��Ͽ� �ֽñ� �ٶ��ϴ�. ��)2.2.2 > "

if "%COCOS2D_VER%"=="" (
goto input
)

echo %COMMON_DIR% �������� PIDL�� �����մϴ�.
xcopy ..\%COMMON_DIR%\*.cpp	.\%COMMON_DIR%\ /s /c /y
xcopy ..\%COMMON_DIR%\*.h	.\%COMMON_DIR%\ /s /c /y
xcopy ..\%COMMON_DIR%\*.PIDL	.\%COMMON_DIR%\ /s /c /y

echo PIDL ���� �������� �մϴ�.
..\..\..\util\PIDL_UnProtect.exe .\%COMMON_DIR%\* -outdir .\%COMMON_DIR%\

echo %PROJECT_NAME% ������ �����մϴ�.
xcopy *.* ..\..\..\cocos2d-x-%COCOS2D_VER%\projects\%PROJECT_NAME%\ /s /c /y

echo %PROJECT_NAME% ������ ���ϴ�.
explorer ..\..\..\cocos2d-x-%COCOS2D_VER%\projects\%PROJECT_NAME%

pause
