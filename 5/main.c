#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cradle.h"

#ifdef DEBUG
#define dprint(fmt, ...) printf(fmt, __VA_ARGS__);
#else
#define dprint(fmt, ...)
#endif

void Other();
void Block(char *L);
void Condition();
void DoProgram();
void DoIf(char *L);
void DoWhile();
void DoLoop();
void DoRepeat();
void DoFor();
void Expression();
void DoDo();
void DoBreak(char *L);


/* Recognize and Translate an "Other" */
void Other()
{
    sprintf(tmp, "%c", GetName());
    EmitLn(tmp);
}


/* Recognize and Translate a Statement Block */
void Block(char *L)
{
    while (! strchr("elu", Look)) {
        dprint("Block: get Look = %c\n", Look);
        switch (Look) {
            case 'i':
                DoIf(L);
                break;
            case 'w':
                DoWhile();
                break;
            case 'p':
                DoLoop();
                break;
            case 'r':
                DoRepeat();
                break;
            case 'f':
                DoFor();
                break;
            case 'd':
                DoDo();
                break;
            case 'b':
                DoBreak(L);
            default:
                Other();
                break;
        }
        /* this is for convenience, otherwise newline character will
        cause an error */
        Newline();
    }
}


/* Parse and Translate a Boolean Condition */
void Condition()
{
    EmitLn("<condition>");
}


/* Parse and Translate a Program */
void DoProgram()
{
    Block(NULL);
    if (Look != 'e') {
        Expected("End");
    }
    EmitLn("END");
}


/* Recognize and Translate and IF Construct */
void DoIf(char *L)
{
    char L1[MAX_BUF];
    char L2[MAX_BUF];
    strcpy(L1, NewLabel());
    strcpy(L2, L1);

    Match('i');
    Condition();

    sprintf(tmp, "jz %s", L1);
    EmitLn(tmp);

    Block(L);
    dprint("DoIf: Got Look = %c\n", Look);

    if (Look == 'l') {
        /* match *else* statement */
        Match('l');
        strcpy(L2, NewLabel());

        sprintf(tmp, "jmp %s", L2);
        EmitLn(tmp);

        PostLabel(L1);

        Block(L);
    }

    Match('e');
    PostLabel(L2);
}


/* Parse and Translate a WHILE Statement */
void DoWhile()
{
    char L1[MAX_BUF];
    char L2[MAX_BUF];

    Match('w');
    strcpy(L1, NewLabel());
    strcpy(L2, NewLabel());
    PostLabel(L1);
    Condition();
    sprintf(tmp, "jz %s", L2);
    EmitLn(tmp);
    Block(L2);
    Match('e');
    sprintf(tmp, "jmp %s", L1);
    EmitLn(tmp);
    PostLabel(L2);
}


/* Parse and Translate a LOOP Statement */
void DoLoop()
{
    char L1[MAX_BUF];
    char L2[MAX_BUF];
    Match('p');
    strcpy(L1, NewLabel());
    strcpy(L2, NewLabel());
    PostLabel(L1);
    Block(L2);
    Match('e');
    sprintf(tmp, "jmp %s", L1);
    EmitLn(tmp);
    PostLabel(L2);
}


/* Parse and Translate a REPEAT Statement */
void DoRepeat()
{
    char L1[MAX_BUF];
    char L2[MAX_BUF];
    Match('r');
    strcpy(L1, NewLabel());
    strcpy(L2, NewLabel());
    PostLabel(L1);
    Block(L2);
    Match('u');
    Condition();

    sprintf(tmp, "jz %s", L1);
    EmitLn(tmp);
    PostLabel(L2);
}


/* I haven't test the actual generated x86 code here, so you're free to
 * inform me if there are bugs. :) */
/* Parse and Translate a FOR Statement */
void DoFor()
{
    char L1[MAX_BUF];
    char L2[MAX_BUF];

    Match('f');
    strcpy(L1, NewLabel());
    strcpy(L2, NewLabel());
    char name = GetName();
    Match('=');
    Expression();
    EmitLn("subl %eax, $1");  /* SUBQ #1, D0*/
    sprintf(tmp, "lea %c, %%edx", name);
    EmitLn(tmp);
    EmitLn("movl %eax, (%edx)");
    Expression();
    EmitLn("push %eax"); /* save the execution of expression */
    PostLabel(L1);
    sprintf(tmp, "lea %c, %%edx", name);
    EmitLn(tmp);
    EmitLn("movl (%edx), %eax");
    EmitLn("addl %eax, 1");
    EmitLn("movl %eax, (%edx)");
    EmitLn("cmp (%esp), %eax");
    sprintf(tmp, "jg %s", L2);
    EmitLn(tmp);
    Block(L2);
    Match('e');
    sprintf(tmp, "jmp %s", L1);
    EmitLn(tmp);
    PostLabel(L2);
    EmitLn("pop %eax");
}


/* Parse and Translate a Math Expression */
void Expression()
{
    EmitLn("<expression>");
}


/* Parse and Translate a DO Statement */
void DoDo()
{
    Match('d');
    char L1[MAX_BUF];
    char L2[MAX_BUF];
    strcpy(L1, NewLabel());
    strcpy(L2, NewLabel());
    Expression();
    EmitLn("subl %eax, $1");
    EmitLn("movl %eax, %ecx");
    PostLabel(L1);
    EmitLn("pushl %ecx");
    Block(L2);
    EmitLn("popl %ecx");
    sprintf(tmp, "loop %s", L1);
    EmitLn(tmp);
    EmitLn("pushl %ecx");
    PostLabel(L2);
    EmitLn("popl %ecx");
}


/* Recognize and Translate a BREAK */
void DoBreak(char *L)
{
    Match('b');
    if (L != NULL) {
        sprintf(tmp, "jmp %s", L);
        EmitLn(tmp);
    } else {
        Abort("No loop to break from");
    }
}


int main()
{
    Init();
    DoProgram();
    return 0;
}
