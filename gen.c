#include "9cc.h"

void gen(Node *node) {
  if (node->kind == ND_NUM) {
    printf("  push %d\n", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n"); // rdi: 64bit整数
  printf("  pop rax\n"); // rax: 64bit整数

  switch (node->kind) {
  case ND_ADD:
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("  imul rax, rdi\n"); // imul: 2つの64bit整数の積をraxに格納
    break;
  case ND_DIV:
    printf(
        "  cqo\n"); // cqo: raxの64bit整数を128bitに拡張し、それをrdx:raxに格納
    printf(
        "  idiv rdi\n"); // idiv: rdx:raxをrdiで割り、商をrax、余りをrdxに格納
    break;
  case ND_EQ:
    printf("  cmp rax, rdi\n");  // cmp: raxとrdiを比較
    printf("  sete al\n");       // sete: raxの下位8bitを1にする
    printf("  movzb rax, al\n"); // movzb: raxの下位8bitを64bitに拡張
    break;
  case ND_NE:
    printf("  cmp rax, rdi\n");  // cmp: raxとrdiを比較
    printf("  setne al\n");      // setne: raxの下位8bitを1にする
    printf("  movzb rax, al\n"); // movzb: raxの下位8bitを64bitに拡張
    break;
  case ND_LT:
    printf("  cmp rax, rdi\n");  // cmp: raxとrdiを比較
    printf("  setl al\n");       // setl: raxの下位8bitを1にする
    printf("  movzb rax, al\n"); // movzb: raxの下位8bitを64bitに拡張
    break;
  case ND_LE:
    printf("  cmp rax, rdi\n");  // cmp: raxとrdiを比較
    printf("  setle al\n");      // setle: raxの下位8bitを1にする
    printf("  movzb rax, al\n"); // movzb: raxの下位8bitを64bitに拡張
    break;
  case ND_NUM:
    break;
  }

  printf("  push rax\n"); // raxの値をスタックに積む
}
