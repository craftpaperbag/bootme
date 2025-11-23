// libcがない世界なので、自分で別名をつける。
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

// 各領域の先頭アドレス（リンカ内で決めてある）を取得
extern char __bss[], __bss_end[], __stack_top[];

// 領域の先頭アドレスと、埋める文字と、サイズを渡すと埋めてくれる
// bss領域のゼロクリアに使うために定義する
void *memset(void *buf,char c, size_t n) {
  uint8_t *p = (uint8_t *) buf;
  // サイズ=n回くり返し
  while (n--)
    // ポインタを1個進めて値にcを入れる
    *p++ = c;

  // 処理終わった配列などのポインタ返す
  return buf;
}

// bootから呼ぶ。bootはOpenSBIが呼ぶ
void kernel_main(void) {
  // bss領域のゼロクリア（一応やっとく
  memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

  // 無限ループ
  for(;;);
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
