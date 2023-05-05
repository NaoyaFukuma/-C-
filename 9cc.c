#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
};

// 現在着目しているトークン
Token *token;

// 入力プログラム
char *user_input;

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

  int pos = loc - user_input; // locとuser_inputの差分を取る
  fprintf(stderr, "%s\n", user_input); // user_inputを出力
  fprintf(stderr, "%*s", pos, " "); // pos個の空白を出力
  fprintf(stderr, "^ "); // ^を出力
  vfprintf(stderr, fmt, ap); // vfprintf: 可変長引数を取るprintf
  fprintf(stderr, "\n");
  exit(1);
}

// 次のトークンが期待している記号の時は、トークンを1つ読み進めて
// 真を返す。それ以外の場合は偽を返す。
// 例えば、次のトークンが"+"の場合、トークンを1つ読み進めて真を返す。
// それ以外の場合は偽を返す。
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) { // token->str[0]はtoken->strの先頭文字
    return false;
  }
  token = token->next;
  return true;
}

// 次のトークンが期待している記号の時は、トークンを1つ読み進める。
// それ以外の場合はエラーを報告する。
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) { // token->str[0]はtoken->strの先頭文字
    error_at(token->str, "'%c'ではありません", op);
  }
  token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合はエラーを報告する。
int expect_number() {
  if (token->kind != TK_NUM) {
    error_at(token->str, "数ではありません");
  }
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token)); // calloc: メモリを確保し、0で埋める
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
  Token head; // ダミーのトークン
  head.next = NULL;
  Token *cur = &head;

  while (*p)
  {
    // 空白文字をスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    // 記号
    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++); // p++: pの値を返した後にpをインクリメントする
      continue;
    }

    // 整数
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p); // p++: pの値を返した後にpをインクリメントする
      cur->val = strtol(p, &p, 10); // strtol: 文字列をlong型に変換する
      continue;
    }

    error_at(token->str, "トークナイズできません");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}


int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  // ユーザー入力を保持する
  user_input = argv[1];

  // トークナイズする
  token = tokenize(argv[1]);

  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n"); // アセンブリの構文をintelに設定
  printf(".global main\n"); // main関数をglobalに設定
  printf("main:\n"); // main関数の開始

  // 式の最初は数でなければならないので、それをチェックして
  // 最初のmov命令を出力
  printf("  mov rax, %d\n", expect_number()); // expect_number: 数値を読み込む

  // '+ <数>'あるいは'- <数>'というトークンの並びを消費しつつ
  // アセンブリを出力
  while (!at_eof()) {
    if (consume('+')) { // consume: 次のトークンが期待している記号の時は、トークンを1つ読み進めて真を返す
      printf("  add rax, %d\n", expect_number()); // expect_number: 数値を読み込む
      continue;
    }

    expect('-'); // expect: 次のトークンが期待している記号の時は、トークンを1つ読み進める
    printf("  sub rax, %d\n", expect_number()); // expect_number: 数値を読み込む
  }
  return 0;
}
