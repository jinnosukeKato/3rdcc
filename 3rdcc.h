#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char *user_input;

void error(char *fmt, ...);

void error_at(char *loc, char *fmt, ...);

// Token関連
typedef enum {
  TK_RESERVED, // 記号
  TK_NUM,      // 整数
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int len;
  int val;
  char *str; // トークン自身の文字列
};

// 現在着目しているトークン
extern Token *token;

bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();

bool startswith(char *p, char *q);

Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

// 構文木関連
typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_EQ, // ==
  ND_NQ, // !=
  ND_LT, // <
  ND_LE, // <=
  ND_NUM,
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);

// コードジェネレータ関連
void put_asm_global(const char *fmt, ...);
void put_asm(const char *fmt, ...);
void gen(Node *node);
