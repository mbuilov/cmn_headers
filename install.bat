@echo off

rem install/uninstall headers in specified directory

setlocal

set argc=0
for %%x in (%*) do set /A argc+=1

if not "%~1"=="" (
	if %argc%==1 (
		if not "%~1"=="-u" (
			set uninstall=0
			goto cont
		)
	) else if %argc%==2 (
		if "%~1"=="-u" (
			if not "%~2"=="" (
				set uninstall=1
				shift
				goto cont
			)
		)
	)
)

echo install/uninstall headers in specified directory
echo usage: %0 [-u] ^<incdir^>
echo where:
echo     -u       - if specified, then uninstall headers
echo     ^<incdir^> - directory where to copy/delete headers

exit /b 1

:cont

if not exist "%~1\*" (
	echo include directory "%~1" does not exist
	exit /b 2
)

set "incdir=%~1"
set headers=asserts.h bswaps.h ccasts.h dprint.h get_opts.inl get_opt.inl sal_defs.h

if %uninstall%==1 (

	for %%x in (%headers%) do (
		if exist "%incdir%\%%x" (
			echo del "%incdir%\%%x"
			for /f %%i in ('del /F /Q "%incdir%\%%x" 2^>^&1') do (
				echo command failed: del /F /Q "%incdir%\%%x"
				exit /b 3
			)
		)
	)

) else (

	for %%x in (%headers%) do (
		echo copy "%%x"  	-^> "%incdir%"
		copy /B /Y "%%x" "%incdir%" >NUL || (
			echo command failed: copy /B /Y "%%x" "%incdir%"
			exit /b 4
		)
	)

)

exit /b 0
