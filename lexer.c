#include "9cc.h"

Token *new_token(TokenKind kind, Token *cur, char *str);

extern char *user_input; // 入力プログラム
extern Token *token;     // 現在着目しているトークン

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

    // 記号
    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur,
                      p++); // p++: pの値を返した後にpをインクリメントする
      continue;
    }

    // 整数
    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur,
                      p); // p++: pの値を返した後にpをインクリメントする
      cur->val = strtol(p, &p, 10); // strtol: 文字列をlong型に変換する
      continue;
    }

    error_at(token->str, "トークナイズできません");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token)); // calloc: メモリを確保し、0で埋める
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}
