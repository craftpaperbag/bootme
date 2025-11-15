bits 16
org 0x7c00 ;ブートセクタが読み込まれる場所。origin detective:実行開始アドレス

; 画面にメッセージ
mov si, msg
call print


;かーねる読み込み
mov bx, 0x1000 ; 読み込む先を4096に設定

; int 0x13(BIOS割り込み)の準備
mov ah, 0x02
mov al, 1
mov ch, 0
mov cl, 2
mov dh, 0
mov dl, 0x80

; BIOS割り込み
int 0x13

jc disk_error ;キャリー足ったらエラー

;
; カーネルへジャンプ
;
jmp 0x0000:0x1000



jmp $

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
