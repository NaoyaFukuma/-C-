#include "9cc.h"

extern char *user_input; // 入力プログラム
extern Token *token;     // 現在着目しているトークン

Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool startswith(char *p, char *q);

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
  Token head; // ダミーのトークン
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // 空白文字をスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    // Multi-letter punctuators
    if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") ||
        startswith(p, ">=")) {
      cur = new_token(TK_RESERVED, cur, p,
                      2); // p++: pの値を返した後にpをインクリメントする
      p += 2;
      continue;
    }

    // Single-letter punctuators
    if (strchr("+-*/()<>", *p)) {
      cur = new_token(TK_RESERVED, cur, p++,
                      1); // p++: pの値を返した後にpをインクリメントする
      continue;
    }

    // 整数
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p,
                      0); // p++: pの値を返した後にpをインクリメントする
      char *tmp =
          p; // pをtmpにコピーして、strtolでpを進め、その差分でlenを計算する
      cur->val = strtol(p, &p, 10); // strtol: 文字列をlong型に変換する
      cur->len = p - tmp;
      continue;
    }

    error_at(token->str, "トークナイズできません");
  }

  new_token(TK_EOF, cur, p, 0);
  return head.next;
}

bool startswith(char *p, char *q) { return memcmp(p, q, strlen(q)) == 0; }

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token)); // calloc: メモリを確保し、0で埋める
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}
