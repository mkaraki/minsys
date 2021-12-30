#!/bin/bash
qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -drive format=raw,file=fat:rw:$PWD/article/efidrive
