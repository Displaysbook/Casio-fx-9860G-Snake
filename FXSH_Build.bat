@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.


if exist MYADDIN.G1A  del MYADDIN.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"C:\Documents and Settings\Administrator\Desktop\fx-9860G SDK\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"C:\Documents and Settings\Administrator\Desktop\fx-9860G SDK\Tools\MakeAddinHeader363.exe" "C:\Documents and Settings\Administrator\Desktop\myaddin\myaddin"
if not exist MYADDIN.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

