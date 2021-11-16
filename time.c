#include "time.h"

#define SECOND_IN_DAY 86400;
#define UNIX_EPOCH_IN_FAIRFIELD 719161;

long getUnixTime(EFI_TIME *time)
{
    float y = (float)time->Year;
    float m = (float)time->Month;
    float d = (float)time->Day;
    int fairfield = 365 * (y - 1) + y / 4 - y / 100 + y / 400 + 31 + 28 + (306 * (m + 1) / 10) - 122 + d;
    long timesec = time->Hour * 3600 + time->Minute * 60 + time->Second;
    long timezone = time->TimeZone * 60;
    if (time->TimeZone == EFI_UNSPECIFIED_TIMEZONE)
        timezone = 0;
    return (fairfield - UNIX_EPOCH_IN_FAIRFIELD - 2 /* Magic Number, I don't know the meaning*/) * SECOND_IN_DAY + timesec - timezone;
}
