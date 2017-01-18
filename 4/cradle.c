#include "cradle.h"
#include <stdio.h>
#include <stdlib.h>

/* Helper Function */
char uppercase(char c)
{
    return (c & 0xDD);
}


/* Read New Character From Input Stream */
void GetChar() 
{
    Look = getchar();
    /* printf("Getchar: %c\n", Look); */
}


/* Report an Error */
void Error(char *s)
{
    printf("\nError: %s.", s);
}


/* Report Error and Halt */
void Abort(char *s)
{
    Error(s);
    exit(1);
}


/* Report What Was Expected */
void Expected(char *s)
{
    sprintf(tmp, "%s Expected", s);
    Abort(tmp);
}


/* Match a Specific Input Character */
void Match(char x)
{
    if(Look == x) {
        GetChar();
    } else {
        sprintf(tmp, "' %c ' ",  x);
        Expected(tmp);
    }
}


/* Recognize and Skip Over a Newline */
void Newline()
{
    if (Look == '\r') {
        GetChar();
        if (Look == '\n') {
            GetChar();
        }
    } else if (Look == '\n') {
        GetChar();
    }
}


/* Recognize an Alpha Character */
int IsAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
} 


/* Recognize a Decimal Digit */
int IsDigit(char c)
{
    return (c >= '0') && (c <= '9');
}


/* Recognize an Addop */
int IsAddop(char c)
{
    return (c == '+') || (c == '-');
}


/* Get an Identifier */
char GetName()
{
    char c = Look;

    if( !IsAlpha(Look)) {
        sprintf(tmp, "Name");
        Expected(tmp);
    }

    GetChar();

    return uppercase(c);
}


/* Get a Number */
int GetNum()
{
    int value = 0;
    if( !IsDigit(Look)) {
        sprintf(tmp, "Integer");
        Expected(tmp);
    }

    while (IsDigit(Look)) {
        value = value * 10 + Look - '0';
        GetChar();
    }

    return value;
}


/* Output a String with Tab */
void Emit(char *s)
{
    printf("\t%s", s);
}


/* Output a String with Tab and Newline */
void EmitLn(char *s)
{
    Emit(s);
    printf("\n");
}


/* Initialize */
void Init()
{
    InitTable();
    GetChar();
}


/* Initialize the Variable Area */
void InitTable()
{
    int i;
    for (i = 0; i < TABLE_SIZE; i++) {
        Table[i] = 0;
    }

}
