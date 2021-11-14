#include <Uefi.h>
#include <Library/UefiLib.h>

// To call gBS (Global Boot Services)
#include <Library/UefiBootServicesTableLib.h>

void ReadLine(IN EFI_SYSTEM_TABLE *SystemTable, OUT CHAR16 *ReturnArray, int len)
{
    SystemTable->ConOut->EnableCursor(SystemTable->ConOut, 1);

    EFI_INPUT_KEY key;
    for (int i = 0; i < len; i++)
    {
        UINTN KeyEvent;

        SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &KeyEvent);
        SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key);
        SystemTable->ConIn->Reset(SystemTable->ConIn, 0);
        Print(L"%c", key.UnicodeChar);

        if (key.UnicodeChar == L'\r')
        {
            *(ReturnArray + i) = 0;
            break;
        }

        if (key.UnicodeChar == 8 && i > 0)
        {
            *(ReturnArray + i - 1) = 0x00;
            i -= 2;
            continue;
        }

        *(ReturnArray + i) = key.UnicodeChar;
    }
    SystemTable->ConOut->EnableCursor(SystemTable->ConOut, 0);
    Print(L"\n");
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
    SystemTable->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    Print(L"MinSys\n");

    while (1)
    {
        CHAR16 input[255];
        Print(L"> ");
        ReadLine(SystemTable, input, 255);
        Print(input);
        Print(L"\n");
    }

    return EFI_SUCCESS;
}
