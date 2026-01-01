#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
// 這是 UEFI 程式的進入點 (就像一般 C 的 main)
//
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  // 呼叫 Print 函式印出字串
  // 記得要用 L"" 包起來，因為 UEFI 使用寬字元 (Unicode)
  Print(L"\n");
  Print(L"========================================\n");
  Print(L"  Hello World! I am ready for BIOS!\n");
  Print(L"========================================\n");
  Print(L"\n");

  Print(L"Press any key to exit...\n");
  gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, NULL);

  return EFI_SUCCESS;
}