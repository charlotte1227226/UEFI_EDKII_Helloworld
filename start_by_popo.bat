@echo off
:: 1. 設定 NASM 路徑 (記得後面要有斜線)
set NASM_PREFIX=C:\Program Files\NASM\

:: 2. 呼叫官方的初始化腳本
call edksetup.bat

:: 3. 顯示成功訊息
echo.
echo ============================================
echo   AnAn UEFI Environment is Ready! Go Build!
echo ============================================