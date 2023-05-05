// #include <ctype.h>
// #include <stdarg.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

#include "9cc.h"

// グローバル空間で使用
char *user_input; // 入力プログラム
Token *token;     // 現在着目しているトークン

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  // ユーザー入力を保持する
  user_input = argv[1];

  // トークナイズ
  token = tokenize(argv[1]);

  // パース
  Node *node = expr();

  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n"); // アセンブリの構文をintelに設定
  printf(".global main\n");           // main関数をglobalに設定
  printf("main:\n");                  // main関数の開始

  // 抽象構文木を下りながらコード生成
  gen(node);

  // スタックトップに式全体の値が残っているはずなので、
  // それをRAXにロードして関数からの返り値とする
  printf("  pop rax\n"); // スタックトップから値を取り出す
  printf("  ret\n");     // 関数からの返り値
  return 0;
}
