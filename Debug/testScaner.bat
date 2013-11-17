@echo off
SetLocal EnableDelayedExpansion

Set DataRoot=%~dp0

Set Extensions=in


For %%A In (%Extensions%) Do (
	For /F "delims=" %%B In ('Dir "%DataRoot%\test\scanner\*.%%A" /B /A-D 2^>nul') Do (compiler.exe /S test/scanner/%%B)
	For /F "delims=" %%B In ('Dir "%DataRoot%\test\scanner\*.%%A" /B /A-D 2^>nul') Do (
		fc /a "%DataRoot%\test\scanner\%%~nB.out" "%DataRoot%\test\scanner\%%~nB.sol" > nul
		if %errorlevel%==0 (
			echo %%~nB.out Ok
			) else (
			echo %%~nB.out Bad
			)		
		)
)
Pause
