#include "screenIO.h"

void switchCursorEnabled(int enabled)
{
    gST->ConOut->EnableCursor(gST->ConOut, enabled);
}

void readLine(OUT CHAR16 *ReturnArray, int len)
{
    switchCursorEnabled(1);

    EFI_INPUT_KEY key;
    for (int i = 0; i < len; i++)
    {
        UINTN KeyEvent;

        gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &KeyEvent);
        gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
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
    switchCursorEnabled(0);
    Print(L"\n");
}
