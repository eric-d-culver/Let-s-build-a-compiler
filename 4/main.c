#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cradle.h"

int Term();
int Expression();
int Factor();
void Assignment();


/* Parse and Translate a Math Factor */
int Factor()
{
    int factor;
    if (Look == '(') {
        Match('(');
        factor = Expression();
        Match(')');
    } else if (IsAlpha(Look)) {
        factor = Table[GetName() - 'A'];
    } else {
        factor = GetNum();
    }

    return factor;
}


/* Parse and Translate a Math Term */
int Term()
{
    int value = Factor();
    while (strchr("*/", Look)) {
        switch(Look)
        {
            case '*':
                Match('*');
                value *= Factor();
                break;
            case '/':
                Match('/');
                value /= Factor();
                break;
            default:
                Expected("Mulop");
        }
    }

    return value;
}


/* Parse and Translate an Expression */
int Expression()
{
    int value;
    if(IsAddop(Look))
        value = 0;
    else
        value = Term();

    while (IsAddop(Look)) {
        switch(Look)
        {
            case '+':
                Match('+');
                value += Term();
                break;
            case '-':
                Match('-');
                value -= Term();
                break;
            default:
                Expected("Addop");
        }
    }

    return value;
}


/* Parse and Translate an Assignment Statement */
void Assignment()
{
    char name = GetName();
    Match('=');
    Table[name - 'A'] = Expression();
}


/* Input Routine
 * We do a little different to the original article.  The syntax of
 * input is "?<variable name><expression>" */
void Input()
{
    Match('?');
    char name = GetName();
    Table[name - 'A'] = Expression();
}


/* Output Routine */
void Output()
{
    Match('!');
    sprintf(tmp, "%d", Table[GetName() - 'A']);
    EmitLn(tmp);
}


int main()
{
    Init();
    do
    {
        switch(Look) {
        case '?':
            Input();
            break;
        case '!':
            Output();
            break;
        default:
            Assignment();
        }

        Newline();
    } while (Look != '.');
    return 0;
}
