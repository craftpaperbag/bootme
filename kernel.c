#include "kernel.h"

// libcがない世界なので、自分で別名をつける。
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// 各領域の先頭アドレス（リンカ内で決めてある）を取得
extern char __bss[], __bss_end[], __stack_top[];

// SBIの呼び出しをする関数
struct sbiret sbi_call(
  long arg0,
  long arg1,
  long arg2,
  long arg3,
  long arg4,
  long arg5,
  long fid,
  long eid) {
  
  //変数a0をレジスタa0に割り当て、arg0を代入
  //以下同様
  register long a0 __asm__("a0") = arg0;
  register long a1 __asm__("a1") = arg1;
  register long a2 __asm__("a2") = arg2;
  register long a3 __asm__("a3") = arg3;
  register long a4 __asm__("a4") = arg4;
  register long a5 __asm__("a5") = arg5;
  register long a6 __asm__("a6") = fid;
  register long a7 __asm__("a7") = eid;

  __asm__ __volatile__("ecall"
    : "=r"(a0), "=r"(a1)
    : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6), "r"(a7)
    : "memory");

  return (struct sbiret){.error = a0, .value = a1};

}

void putchar(char ch) {
  // コンソール出力
  sbi_call(ch, 0, 0, 0, 0, 0, 0, 1);
}

// 領域の先頭アドレスと、埋める文字と、サイズを渡すと埋めてくれる
// bss領域のゼロクリアに使うために定義する
void *memset(void *buf,char c, size_t n) {
  uint8_t *p = (uint8_t *) buf;
  // サイズ=n回くり返し
  while(n--)
    // ポインタを1個進めて値にcを入れる
    *p++ = c;

  // 処理終わった配列などのポインタ返す
  return buf;
}

void kernel_main(void) {
  // bss領域のゼロクリア（一応やっとく
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);
  
  // 出力したい文字列
  const char *s = "\n\nHello World :D\n";

  // 文字を表示
  for (int i=0; s[i] != '\0'; i++) {
    putchar(s[i]);
  }

  // 無限ループ
  for (;;) {
    __asm__ __volatile__("wfi"); // Wait for Interrupt
  }
}

// CPUが最初に実行するコード
// .text.bootがROMの先頭に置かれる
__attribute__((section(".text.boot")))
__attribute__((naked))
// 裸関数。
void boot(void) {
  __asm__ __volatile__( // アセンブリで中身を書く
    "mv sp, %[stack_top]\n" // スタックポインタをスタックの先頭にセット
    "j kernel_main\n" // ジャンプしろ：kernel_mainに。
    :
    : [stack_top] "r" (__stack_top) // 汎用レジスタの空いてるところをつかってstack_top（リンカで定義してる、スタック領域の先頭）を渡せ
  );
}
