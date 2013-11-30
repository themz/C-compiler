@echo off
SetLocal EnableDelayedExpansion

Set DataRoot=%~dp0



Set Extensions=in


For %%A In (%Extensions%) Do (
	For /F "delims=" %%B In ('Dir "%DataRoot%\test\parser\*.%%A" /B /A-D 2^>nul') Do ( compiler.exe /PS test/parser/%%B )
	For /F "delims=" %%B In ('Dir "%DataRoot%\test\parser\*.%%A" /B /A-D 2^>nul') Do (
		fc /b "%DataRoot%\test\parser\%%~nB.out" "%DataRoot%\test\parser\%%~nB.sol" > nul
		if ERRORLEVEL 0 echo %%~nB.out Ok
		if ERRORLEVEL 1 echo %%~nB.out Bab
		)
)
Pause
