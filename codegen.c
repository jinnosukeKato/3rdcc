#include "3rdcc.h"

void put_asm_global(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  printf("\n");
}

void put_asm(const char *fmt, ...) {
  printf("  ");
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  printf("\n");
}

void gen(Node *node) {
  if (node->kind == ND_NUM) {
    put_asm("push %d", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  put_asm("pop rdi");
  put_asm("pop rax");

  switch (node->kind) {
  case ND_ADD:
    put_asm("add rax, rdi");
    break;
  case ND_SUB:
    put_asm("sub rax, rdi");
    break;
  case ND_MUL:
    put_asm("imul rax, rdi");
    break;
  case ND_DIV:
    put_asm("cqo");
    put_asm("idiv rdi");
    break;
  case ND_EQ:
    put_asm("cmp rax, rdi");
    put_asm("sete al");
    put_asm("movzb rax, al");
    break;
  case ND_NQ:
    put_asm("cmp rax, rdi");
    put_asm("setne al");
    put_asm("movzb rax, al");
    break;
  case ND_LT:
    put_asm("cmp rax, rdi");
    put_asm("setl al");
    put_asm("movzb rax, al");
    break;
  case ND_LE:
    put_asm("cmp rax, rdi");
    put_asm("setle al");
    put_asm("movzb rax, al");
    break;
  }

  put_asm("push rax");
}
