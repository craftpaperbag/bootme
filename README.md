# bootme
OS勉強用

# コマンドメモ

## nasm -f bin aaaaa.asm -o bbbbb.bin
実行可能なバイナリファイルに変換

## xxd
みやすく16進数表示

## 起動
```
qemu-system-x86_64 -drive format=raw,file=os.img
```
