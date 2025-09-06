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

  char *p = argv[1];

  put_asm_global(".intel_syntax noprefix");
  put_asm_global(".globl main");
  put_asm_global("main:");

  put_asm("mov rax, %ld", strtol(p, &p, 10));

  while (*p) {
    if (*p == '+') {
      p++;
      put_asm("add rax, %ld", strtol(p, &p, 10));
      continue;
    }

    if (*p == '-') {
      p++;
      put_asm("sub rax, %ld", strtol(p, &p, 10));
      continue;
    }

    fprintf(stderr, "予期しない文字です: '%c'\n", *p);
    return 1;
  }

  put_asm("ret");
  return 0;
}
