@echo off

set PROJECT_NAME=SynchWorldCocos2dx
set COMMON_DIR=Common

echo 자세한 사용방법은 ProudNet 도움말을 참고하여 주시기 바랍니다.

:input
set COCOS2D_VER=
set /p COCOS2D_VER="설치하신 Cocos2d-x의 버전을 입력하여 주시기 바랍니다. 예)2.2.2 > "

if "%COCOS2D_VER%"=="" (
goto input
)

echo %COMMON_DIR% 폴더에서 PIDL을 복사합니다.
xcopy ..\%COMMON_DIR%\*.cpp	.\%COMMON_DIR%\ /s /c /y
xcopy ..\%COMMON_DIR%\*.h	.\%COMMON_DIR%\ /s /c /y
xcopy ..\%COMMON_DIR%\*.PIDL	.\%COMMON_DIR%\ /s /c /y

echo PIDL 파일 컴파일을 합니다.
..\..\..\util\PIDL_UnProtect.exe .\%COMMON_DIR%\* -outdir .\%COMMON_DIR%\

echo %PROJECT_NAME% 폴더를 복사합니다.
xcopy *.* ..\..\..\cocos2d-x-%COCOS2D_VER%\projects\%PROJECT_NAME%\ /s /c /y

echo %PROJECT_NAME% 폴더를 엽니다.
explorer ..\..\..\cocos2d-x-%COCOS2D_VER%\projects\%PROJECT_NAME%

pause
