#include  <Uefi.h>
#include  <Library/UefiLib.h>

EFI_STATUS
EFIAPI
UefiMain (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
    )
{
  ClearScreen();
  Print(L"Hello EDK II.\n");
  while (1);
  return 0;
}
