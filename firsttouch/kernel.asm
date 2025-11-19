bits 16

org 0x1000

mov si, msg
call print
jmp $

print:
  lodsb; 1文字読み込んでsiを次へ。
  or al, al ;ゼロに到達したらzfを立てる
  jz .done ;zfが立ってたらdoneへ飛ぶ
  mov ah, 0x0e ;1文字表示したらカーソルを右へ動かすモードにする
  int 0x10 ;画面表示
  jmp print ;printの最初に戻る
.done:
  ret ;終了。スタックのアドレスに戻る

msg: db "Hello from KERNEL :D", 0 ;変数msgの設定
