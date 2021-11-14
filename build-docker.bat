@echo off
SET CURRENTDIR="%cd%"

docker run --rm -it -v %CURRENTDIR%:/edk2/MinSys:ro -v %CURRENTDIR%/article:/edk2/Build/MyPkgX64/DEBUG_GCC5/X64/ ghcr.io/mkaraki/edk2-app-builder:latest build -a X64 -t GCC5 -p MinSys/MinSys.dsc
mkdir article\efidrive\EFI\Boot\
copy article\MinSys.efi article\efidrive\EFI\Boot\Bootx64.efi