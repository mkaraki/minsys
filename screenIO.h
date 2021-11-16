#include <Uefi.h>
#include <Library/UefiLib.h>

#include <Library/UefiBootServicesTableLib.h>

extern void switchCursorEnabled(int enabled);
extern void readLine(OUT CHAR16 *ReturnArray, int len);