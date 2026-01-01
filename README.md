# 📘 UEFI (EDK II) 開發實戰全攻略：從零到 Hello World

## 第一階段：環境準備與工具安裝

### 1. Visual Studio 2026 (Community/Preview)

* **下載點：** 微軟官網。
* **安裝重點：**  在「工作負載 (Workloads)」中，務必勾選 **「使用 C++ 的桌面開發 (Desktop development with C++)」**。

### 2. NASM (The Netwide Assembler)

* **下載點：** NASM 官網 -> Stable (如 2.15.xx) -> win64 -> 下載 `.exe` 安裝檔。
* **安裝路徑：** 建議保持預設 `C:\Program Files\NASM`。
* **設定環境變數 (重要)：**
1. 按 Win 鍵搜尋「環境變數」。
2. 在「系統變數」區塊找到 `Path` -> 編輯 -> 新增。
3. 貼上路徑：`C:\Program Files\NASM\`。



### 3. Python

* **安裝重點：** 務必勾選 **"Add Python to PATH"**。

---

## 第二階段：取得原始碼與初始化

請使用 **Developer Command Prompt for VS 2022 (或 2026)** 執行以下操作。

### 1. 下載程式碼 (Git Clone)

為了解決權限與子模組問題，請依序執行：

```cmd
:: 1. 設定安全目錄 (避免 Git 報錯)
git config --global --add safe.directory E:/anan/C_program/UEFI/edk2

:: 2. 下載程式碼 (包含所有子模組)
git clone --recursive https://github.com/tianocore/edk2.git E:\anan\C_program\UEFI\edk2

```

### 2. 補救措施 (如果 BaseTools 空空的)

如果你發現 `BaseTools` 資料夾不完整，請進入資料夾執行：

```cmd
cd E:\anan\C_program\UEFI\edk2
git submodule update --init --recursive

```

---

## 第三階段：關鍵設定與問題修復 (Troubleshooting)

這是最關鍵的一步，解決了「中文編碼」與「工具版本不合」的問題。

### 1. 建立設定檔

在專案根目錄下執行：

```cmd
set NASM_PREFIX=C:\Program Files\NASM\
edksetup.bat

```

*(執行後會自動產生 `Conf` 資料夾)*

### 2. 修改 `Conf/target.txt` (指定編譯目標)

請用 VS Code 開啟該檔案，修改以下三行：

* `ACTIVE_PLATFORM       = EmulatorPkg/EmulatorPkg.dsc`
* `TARGET_ARCH           = X64`
* `TOOL_CHAIN_TAG        = VS2022`

### 3. 修復 VfrCompile 工具 (解決版本不合)

因為預先編譯好的工具可能不相容，我們手動重新編譯它：

```cmd
cd BaseTools\Source\C
nmake
cd ..\..\..

```

### 4. 修復繁體中文編碼錯誤 (C4819 Error) **(面試亮點)**

解決 Windows 950 編碼導致編譯失敗的問題。

* **開啟檔案：** `Conf/tools_def.txt`
* **搜尋關鍵字：** `DEBUG_VS2022_X64_CC_FLAGS`
* **修改動作：** 在 **DEBUG**、**RELEASE**、**NOOPT** 這三行的**最後面**，加上 ` /wd4819`。

**修改範例：**

```text
DEBUG_VS2022_X64_CC_FLAGS     = ... /volatileMetadata- /wd4819
RELEASE_VS2022_X64_CC_FLAGS   = ... /volatileMetadata- /wd4819
NOOPT_VS2022_X64_CC_FLAGS     = ... /volatileMetadata- /wd4819

```

---

## 第四階段：建立自動啟動腳本 (Daily Routine)

為了避免每次都要手動設定環境變數，我們建立一個批次檔。

1. 在 `edk2` 根目錄建立檔案 **`start.bat`**。
2. 貼上以下內容：

```cmd
@echo off
:: 設定 NASM 路徑 (後面要有斜線)
set NASM_PREFIX=C:\Program Files\NASM\

:: 呼叫官方初始化腳本
call edksetup.bat

echo.
echo ============================================
echo   UEFI Environment is Ready! Go Build!
echo ============================================

```

---

## 第五階段：撰寫 Hello World 程式

現在環境已經完美，我們來寫第一個 UEFI 應用程式。

### 1. 建立檔案結構

在 `edk2` 根目錄下建立：

* `MyPkg`
* `HelloWorld`
* 📄 `HelloWorld.c`
* 📄 `HelloWorld.inf`





### 2. 撰寫程式碼 (`HelloWorld.c`)

**重點：** 必須加入 `UefiBootServicesTableLib.h` 才能使用 `gBS` 和 `gST`。

```c
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h> // 關鍵：為了讓編譯器認識 gBS 和 gST

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  Print(L"\n");
  Print(L"========================================\n");
  Print(L"  Hello World! I am ready for BIOS!\n");
  Print(L"========================================\n");
  Print(L"\n");

  Print(L"Press any key to exit...\n");
  gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, NULL); // 等待按鍵

  return EFI_SUCCESS;
}

```

### 3. 撰寫模組設定 (`HelloWorld.inf`)

**重點：** `[LibraryClasses]` 必須補上 `UefiBootServicesTableLib`。

```ini
[Defines]
  INF_VERSION                    = 0x00010005
  BASE_NAME                      = MyHelloWorld
  FILE_GUID                      = 6987936E-ED34-44db-AE97-1FA5E4ED2A16
  MODULE_TYPE                    = UEFI_APPLICATION
  VERSION_STRING                 = 1.0
  ENTRY_POINT                    = UefiMain

[Sources]
  HelloWorld.c

[Packages]
  MdePkg/MdePkg.dec
  MdeModulePkg/MdeModulePkg.dec

[LibraryClasses]
  UefiLib
  UefiApplicationEntryPoint
  UefiBootServicesTableLib

```

### 4. 註冊到編譯清單 (`EmulatorPkg.dsc`)

**重點：** 絕對不能在 `.dsc` 檔裡面寫中文註解，會導致編碼錯誤 (`File read failure`)。

* **開啟檔案：** `EmulatorPkg/EmulatorPkg.dsc`
* **搜尋區塊：** `[Components]`
* **加入位置：** 在檔案最後面，`[BuildOptions]` 之前。

```text
#--------------------------------------
  MyPkg/HelloWorld/HelloWorld.inf
#--------------------------------------

```

---

## 第六階段：每日啟動與執行流程 (SOP)

以後每次你要開發或展示時，請嚴格遵守此流程：

1. **開啟終端機**
* 開啟 "Developer Command Prompt for VS 2022 (或 2026)"。


2. **進入資料夾**
```cmd
E:
cd E:\anan\C_program\UEFI\edk2

```


3. **發動引擎 (執行腳本)**
```cmd
start.bat

```


*(看到 "Ready" 表示環境載入成功)*
4. **開始編譯**
```cmd
build

```


*(等待綠色的 **- Done -** 出現)*
5. **啟動模擬器**
```cmd
cd Build\EmulatorX64\DEBUG_VS2022\X64
WinHost.exe

```


6. **在 UEFI Shell 執行程式**
* 視窗跳出後，輸入：
```cmd
fs0:
MyHelloWorld.efi

```


* **成功畫面：** 螢幕顯示 `"Hello Gigabyte! I am ready for BIOS!"`



---