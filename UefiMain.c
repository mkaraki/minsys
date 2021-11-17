#include <Uefi.h>
#include <Library/UefiLib.h>

// To call gBS (Global Boot Services)
#include <Library/UefiBootServicesTableLib.h>

#include "time.h"
#include "screenIO.h"
#include "string.h"

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
    if (printToo)
    {
        Print(L"%d-%d-%d %d:%d:%d\n", time.Year, time.Month, time.Day, time.Hour, time.Minute, time.Second);
    }
    return (unsigned long)getUnixTime(&time);
}

void HandleVerb(IN CHAR16 *verb, IN CHAR16 *args)
{
    if (compaireString(verb, L"exit"))
    {
        gBS->Exit(gImageHandle, 0, 0, NULL);
    }
    else if (compaireString(verb, L"reset"))
    {
        Print(L"Ok.");
        gST->RuntimeServices->ResetSystem(EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
    else if (compaireString(verb, L"reinitialize"))
    {
        Print(L"Ok.");
        gST->RuntimeServices->ResetSystem(EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
    else if (compaireString(verb, L"shutdown"))
    {
        Print(L"Ok.");
        gST->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    }
    else if (compaireString(verb, L"time"))
    {
        unsigned int unixtime = getTime(1);
        CHAR16 unixtimestr[30];
        inttostr((int)unixtime, unixtimestr);
        Print(unixtimestr);
    }
    else if (compaireString(verb, L"timeset"))
    {
        EFI_TIME time;
        CHAR16 strtmp[9];
        gST->RuntimeServices->GetTime(&time, NULL);

        Print(L"YEAR [1900-9999]: ");
        readLine(strtmp, 9);
        time.Year = strtoint(strtmp);

        Print(L"MONTH [1-12]: ");
        readLine(strtmp, 9);
        time.Month = strtoint(strtmp);

        Print(L"DAY [1-31]: ");
        readLine(strtmp, 9);
        time.Day = strtoint(strtmp);

        Print(L"HOUR [0-23]: ");
        readLine(strtmp, 9);
        time.Hour = strtoint(strtmp);

        Print(L"MINUTE [0-59]: ");
        readLine(strtmp, 9);
        time.Minute = strtoint(strtmp);

        Print(L"SECOND [0-59]: ");
        readLine(strtmp, 9);
        time.Second = strtoint(strtmp);

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
        readLine(input, 255);
        GetCommandVerb(input, verb, 16);
        HandleVerb(verb, input);
        Print(L"\n");
    }

    return EFI_SUCCESS;
}
