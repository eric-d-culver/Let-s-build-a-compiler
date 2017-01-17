#include "cradle.h"
#include <stdio.h>
#include <stdlib.h>


/* Read New Character From Input Stream */
void GetChar() 
{
    Look = getchar();
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
        SkipWhite();
    } else {
        sprintf(tmp, "' %c ' ",  x);
        Expected(tmp);
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


/* Recognize an Alphanumeric */
int IsAlNum(char c)
{
    return IsAlpha(c) || IsDigit(c);
}


/* Recognize an Addop */
int IsAddop(char c)
{
    return (c == '+') || (c == '-');
}


/* Recognize White Space */
int IsWhite(char c)
{
    return (c == ' ') || (c == '\t');
}


/* Get an Identifier */
char* GetName()
{
    char *token = token_buf;

    if( !IsAlNum(Look)) {
        Expected("Name");
    }
    while (IsAlNum(Look)) {
        *token = Look;
        token++;

        GetChar();
    }

    SkipWhite();

    *token = '\0';
    return token_buf;
}


/* Get a Number */
char* GetNum()
{
    char *value = token_buf;

    if( !IsAlNum(Look)) {
        Expected("Integer");
    }
    while (IsDigit(Look)) {
        *value = Look;
        value++;

        GetChar();
    }

    SkipWhite();

    *value = '\0';
    return token_buf;
}


/* Skip Over Leading White Space */
void SkipWhite()
{
    while (IsWhite(Look)) {
        GetChar();
    }
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
    GetChar();
    SkipWhite();
}

