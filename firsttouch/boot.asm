bits 16
org 0x7c00 ;ブートセクタが読み込まれる場所。origin detective:実行開始アドレス

; 画面にメッセージ
mov si, msg
call print


;かーねる読み込み
mov bx, 0x1000 ; 読み込む先を4096に設定

; int 0x13(BIOS割り込み)の準備
mov ah, 0x02 ; 読み込みなさい
mov al, 1    ; 512バイト読み込みなさい(kernelがそれ以下なので）
mov ch, 0    ; CHS方式:シリンダーゼロ
mov cl, 2    ; セクタ番号：２（kernel.bin）
mov dh, 0    ; ヘッド番号: 0　（フロッピーや古いHDD想定
mov dl, 0x80 ; ドライブ番号。0x00: フロッピー、0x80: HDD

; BIOS割り込み
int 0x13 ; 設定した内容でディスクを読み込め

jc disk_error ;キャリー足ったらエラー

;
; カーネルへジャンプ
;
jmp 0x0000:0x1000 ;読み込んだカーネルを実行している。

print:
  lodsb
  or al, al
  jz .done
  mov ah, 0x0e
  int 0x10
  jmp print
.done:
  ret

msg: db "Bootloader: loading kernel...", 0


;
;disk_error関数(関数？)
;

disk_error:
  mov si, errmsg
  call print
  jmp $

errmsg: db "Disk read error :(", 0

;
; ブートセクタのパディング＋シグネチャ
;

times 510-($-$$) db 0
dw 0xaa55
