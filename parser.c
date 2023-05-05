#include "9cc.h"

// グローバル空間で使用
extern Token *token; // 現在着目しているトークン

bool consume(char op);
void expect(char op);
int expect_number();

Node *mul();
Node *primary();

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node)); // calloc: メモリを確保し、0で埋める
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node)); // calloc: メモリを確保し、0で埋める
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr() {
  Node *node = mul();

  for (;;) {
    if (consume('+')) {
      node = new_node(ND_ADD, node, mul()); // +の場合
    } else if (consume('-')) {
      node = new_node(ND_SUB, node, mul()); // -の場合
    } else {
      return node;
    }
  }
}

Node *mul() {
  Node *node = primary();

  for (;;) {
    if (consume('*')) {
      node = new_node(ND_MUL, node, primary()); // *の場合
    } else if (consume('/')) {
      node = new_node(ND_DIV, node, primary()); // /の場合
    } else {
      return node;
    }
  }
}

Node *primary() {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume('(')) {
    Node *node = expr();
    expect(')'); // ")"のはず
    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number());
}

// 次のトークンが期待している記号の時は、トークンを1つ読み進める。
// それ以外の場合はエラーを報告する。
void expect(char op) {
  if (token->kind != TK_RESERVED ||
      token->str[0] != op) { // token->str[0]はtoken->strの先頭文字
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

// 次のトークンが期待している記号の時は、トークンを1つ読み進めて
// 真を返す。それ以外の場合は偽を返す。
// 例えば、次のトークンが"+"の場合、トークンを1つ読み進めて真を返す。
// それ以外の場合は偽を返す。
bool consume(char op) {
  if (token->kind != TK_RESERVED ||
      token->str[0] != op) { // token->str[0]はtoken->strの先頭文字
    return false;
  }
  token = token->next;
  return true;
}
