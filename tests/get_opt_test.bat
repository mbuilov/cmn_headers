@echo off
setlocal
set step=0

rem 4464: relative include path contains '..'
rem 4820: '...' bytes padding added after data member '...'
set "WARN=/Wall /wd4464 /wd4820"

set "TEST=""! $ ^^ ' """" %%%% ^^ 1 2 #"" -gaf --help 3 --file 2 -b3 --level=4 -l 5 -d6 --debug 7 -o ""8^&"" -vr --output=9 -fg -f g --verbose 1 --verbose=4 -g 3 -g1 -9 - --trace --trace q --trace=v -t -tra -trace -h 6 -ogv- -- 4 -b --y 9"

call :StepOk "cl /nologo /TC %WARN% get_opt_test.c /Foget_opt_test" || exit /b 1
call :StepOk "get_opt_test.exe %TEST% > get_opt.out" || exit /b 1
call :StepOk "fc get_opt.out get_opt_test.out" || exit /b 1

call :StepOk "cl /nologo /TC %WARN% /DGET_OPT_ARGV_NZ get_opt_test.c /Foget_opt_test_nz" || exit /b 1
call :StepOk "get_opt_test_nz.exe %TEST% > get_opt_nz.out" || exit /b 1
call :StepOk "fc get_opt_nz.out get_opt_test.out" || exit /b 1

echo =============== all tests OK ===============
exit /b 0

:StepOk
echo step: %step%
set /a step+=1
rem see gawk-windows/test.bat:execq
set "x=%~1"
set "x=%x:>=^>%"
set "x=%x:&=^&%"
set "x=%x:^^^>=>%"
set "x=%x:^^^&=&%"
set "x=%x:^^^^=^%"
echo %x:""="%
set "x=%~1"
set "x=%x:^^&=&%"
set "x=%x:^^^^=^%"
%x:""="% && exit /b 0
echo failed.
exit /b 1
