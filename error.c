#include "9cc.h"

// グローバル空間で使用
extern char *user_input; // 入力プログラム

// エラーを報告するための関数
// printfと同じ引数を取る
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap); // vfprintf: 可変長引数を取るprintf
  fprintf(stderr, "\n");
  exit(1);
}

// エラー箇所を報告する
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;          // locとuser_inputの差分を取る
  fprintf(stderr, "%s\n", user_input); // user_inputを出力
  fprintf(stderr, "%*s", pos, " ");    // pos個の空白を出力
  fprintf(stderr, "^ ");               // ^を出力
  vfprintf(stderr, fmt, ap); // vfprintf: 可変長引数を取るprintf
  fprintf(stderr, "\n");
  exit(1);
}
