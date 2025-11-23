#!/bin/bash
set -xue

QEMU=qemu-system-riscv32

# clangのパス
CC=/opt/local/bin/clang
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fuse-ld=lld -fno-stack-protector -ffreestanding -nostdlib"

# カーネルをビルド
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
  kernel.c

$QEMU -machine virt -bios default -nographic -serial pty -monitor stdio \
  --no-reboot -kernel kernel.elf
# 例では-serial mon:stdioだが、手元の環境（古mac）はQEMUモニターへの切り替えに失敗するので、直接接続する
