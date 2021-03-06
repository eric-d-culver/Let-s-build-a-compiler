#ifndef _CRADLE_H
#define _CRADLE_H
#define UPCASE(C) ((1<<6)| (C))

#define MAX_BUF 100
char tmp[MAX_BUF];

char Look; /* Lookahead Character */

void GetChar();

void Error(char *s);
void Abort(char *s);
void Expected(char *s);
void Match(char x);

int IsAlpha(char c);
int IsDigit(char c);
int IsAddop(char c);

char GetName();
char GetNum();

void Emit(char *s);
void EmitLn(char *s);

void Init();

#endif
