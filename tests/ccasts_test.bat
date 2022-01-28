@echo off
setlocal
set step=0

rem 4464: relative include path contains '..'
rem 4820: '...' bytes padding added after data member '...'
rem 4514: '...': unreferenced inline function has been removed
set "WARN=/Wall /wd4464 /wd4820 /wd4514"

rem should be compiled
call :StepOk "cl /nologo /TC /c %WARN% ccasts_test.c" || exit /b 1
call :StepOk "cl /nologo /TP /c %WARN% ccasts_test.c" || exit /b 1

rem should not be compiled
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD1") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD2") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD3") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD4") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD5") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD6") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD7") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD8") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD9") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD10") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD11") || exit /b 1
(call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD12") || exit /b 1
rem not supported
rem (call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD13") || exit /b 1
rem (call :StepFail "cl /nologo /TC /c %WARN% ccasts_test.c /DBAD14") || exit /b 1

rem should not be compiled
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD1") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD2") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD3") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD4") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD5") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD6") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD7") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD8") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD9") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD10") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD11") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD12") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD13") || exit /b 1
(call :StepFail "cl /nologo /TP /c %WARN% ccasts_test.c /DBAD14") || exit /b 1

echo =============== all tests OK ===============
exit /b 0

:StepOk
echo step: %step%
set /a step+=1
echo %~1
%~1 && exit /b 0
goto :ErrExit

:StepFail
echo step: %step%
set /a step+=1
echo %~1
%~1 || exit /b 0
goto :ErrExit

:ErrExit
echo failed.
exit /b 1
