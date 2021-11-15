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

int GetCommandVerb(IN CHAR16 *source, OUT CHAR16 *verb, int searchLen)
{
    for (int i = 0; i < searchLen; i++)
    {
        if (source[i] == L' ')
        {
            *(verb + i) = 0;
            return 1;
        }
        *(verb + i) = source[i];
    }
    return 0;
}

int CompaireString(IN CHAR16 *a, IN CHAR16 *b)
{
    int i = 0;
    while (a[i] != 0)
    {
        if (a[i] != b[i])
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int pow(int x, int y)
{
    int result = 1;
    for (int i = 0; i < y; i++)
    {
        result *= x;
    }
    return result;
}

int parseInt(IN CHAR16 *source)
{
    int result = 0;

    int firstpos = 0;
    for (int i = 0; source[i] != 0; i++)
        firstpos = i;

    int isminus = 0;

    int j = 0;
    for (int i = firstpos; i >= 0; i--)
    {
        if (i == firstpos && source[i] == '-')
        {
            isminus = 1;
            continue;
        }

        if (source[i] < L'0' || source[i] > L'9')
            break;

        result += (source[i] - L'0') * pow(10, j);
        j++;
    }

    if (isminus)
        result *= -1;

    return result;
}

// This return unix time too
unsigned long getTime(int printToo)
{
    EFI_TIME time;
    EFI_STATUS status = gST->RuntimeServices->GetTime(&time, NULL);
    if (status != EFI_SUCCESS)
    {
        Print(L"Error getting time\n");
        return 0;
    }
    unsigned long unixTime = (time.Year - 1970) * 365 * 24 * 60 * 60 + (time.Month - 1) * 30 * 24 * 60 * 60 + time.Day * 24 * 60 * 60 + time.Hour * 60 * 60 + time.Minute * 60 + time.Second;
    if (printToo)
    {
        Print(L"%d-%d-%d %d:%d:%d\n", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);
    }
    return unixTime;
}

void HandleVerb(IN CHAR16 *verb, IN CHAR16 *args)
{
    if (CompaireString(verb, L"exit"))
    {
        gBS->Exit(gImageHandle, 0, 0, NULL);
    }
    else if (CompaireString(verb, L"reset"))
    {
        Print(L"Ok.");
        gST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
    else if (CompaireString(verb, L"reinitialize"))
    {
        Print(L"Ok.");
        gST->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
    else if (CompaireString(verb, L"shutdown"))
    {
        Print(L"Ok.");
        gST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    }
    else if (CompaireString(verb, L"time"))
    {
        unsigned int unixtime = getTime(1);
        Print(L"%u\n", unixtime);
    }
    else if (CompaireString(verb, L"timeset"))
    {
        EFI_TIME time;
        CHAR16 strtmp[9];
        gST->RuntimeServices->GetTime(&time, NULL);

        Print(L"YEAR [1900-9999]: ");
        ReadLine(gST, strtmp, 9);
        time.Year = parseInt(strtmp);

        Print(L"MONTH [1-12]: ");
        ReadLine(gST, strtmp, 9);
        time.Month = parseInt(strtmp);

        Print(L"DAY [1-31]: ");
        ReadLine(gST, strtmp, 9);
        time.Day = parseInt(strtmp);

        Print(L"HOUR [0-23]: ");
        ReadLine(gST, strtmp, 9);
        time.Hour = parseInt(strtmp);

        Print(L"MINUTE [0-59]: ");
        ReadLine(gST, strtmp, 9);
        time.Minute = parseInt(strtmp);

        Print(L"SECOND [0-59]: ");
        ReadLine(gST, strtmp, 9);
        time.Second = parseInt(strtmp);

        gST->RuntimeServices->SetTime(&time);

        getTime(1);
    }
    else
    {
        Print(L"Unknown command");
    }
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
        CHAR16 verb[16];
        Print(L"> ");
        ReadLine(SystemTable, input, 255);
        GetCommandVerb(input, verb, 16);
        HandleVerb(verb, input);
        Print(L"\n");
    }

    return EFI_SUCCESS;
}
