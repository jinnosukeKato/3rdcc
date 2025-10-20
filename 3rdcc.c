#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

char *user_input;

void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, " ");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

typedef enum {
  TK_RESERVED,  // 記号
  TK_NUM,       // 整数
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;  // トークン自身の文字列
};

// 現在着目しているトークン
Token *token;

typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_NUM,
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) return false;

  token = token->next;
  return true;
}

void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error_at(token->str, "'%c'ではありません", op);

  token = token->next;
}

int expect_number() {
  if (token->kind != TK_NUM) error_at(token->str, "数値ではありません");

  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() { return token->kind == TK_EOF; }

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr();
Node *mul();
Node *primary();

Node *expr() {
  Node *node = mul();

  while (1) {
    if (consume('+')) {
      node = new_node(ND_ADD, node, mul());
    } else if (consume('-')) {
      node = new_node(ND_SUB, node, mul());
    } else {
      return node;
    }
  }
}

Node *mul() {
  Node *node = primary();

  while (1) {
    if (consume('*')) {
      node = new_node(ND_MUL, node, primary());
    } else if (consume('/')) {
      node = new_node(ND_DIV, node, primary());
    } else {
      return node;
    }
  }
}

Node *primary() {
  if (consume('(')) {
    Node *node = expr();
    expect(')');
    return node;
  }

  return new_node_num(expect_number());
}

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
  }

  put_asm("push rax");
}

// 新しいトークンを作成し，curにつなげる
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (strchr("+-*/()", *p)) {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "トークナイズできない入力: %s", p);
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("引数の個数が正しくありません");
    return 1;
  }

  user_input = argv[1];
  token = tokenize(argv[1]);
  Node *node = expr();

  put_asm_global(".intel_syntax noprefix");
  put_asm_global(".globl main");
  put_asm_global("main:");

  gen(node);

  put_asm("pop rax");
  put_asm("ret");
  return 0;
}
