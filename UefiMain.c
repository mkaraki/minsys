#include <Uefi.h>
#include <Library/UefiLib.h>

void ReadLine(IN EFI_SYSTEM_TABLE *SystemTable, OUT CHAR16 *ReturnArray, int len);

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
        for (int i = 0; i < 255; i++)
        {
            input[i] = 0;
        }
        Print(L"> ");
        ReadLine(SystemTable, input, 255);
        for (int i = 0; i < 255; i++)
        {
            if (input[i] == 0)
            {
                break;
            }
            Print(L"%c", input[i]);
        }
        Print(L"\n");
    }

    return EFI_SUCCESS;
}

void ReadLine(IN EFI_SYSTEM_TABLE *SystemTable, OUT CHAR16 *ReturnArray, int len)
{
    int counter = 0;

    SystemTable->ConOut->EnableCursor(SystemTable->ConOut, 1);

    EFI_INPUT_KEY key;
    do
    {
        if (counter >= len)
        {
            break;
        }

        UINTN KeyEvent;

        SystemTable->BootServices->WaitForEvent(1, &SystemTable->ConIn->WaitForKey, &KeyEvent);
        SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &key);
        SystemTable->ConIn->Reset(SystemTable->ConIn, 0);
        Print(L"%c", key.UnicodeChar);

        *(ReturnArray + counter) = key.UnicodeChar;
        counter++;
    } while (key.UnicodeChar != L'\r');
    SystemTable->ConOut->EnableCursor(SystemTable->ConOut, 0);
    Print(L"\n");
}