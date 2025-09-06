#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void put_asm_global(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
}

void put_asm(const char *fmt, ...) {
  printf("  ");
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  put_asm_global(".intel_syntax noprefix");
  put_asm_global(".globl main");
  put_asm_global("main:");

  put_asm("mov rax, %d", atoi(argv[1]));
  put_asm("ret");
}
