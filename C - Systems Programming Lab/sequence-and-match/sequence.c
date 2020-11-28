/*
    * File: sequence.c
    * 
    *   The Program receives text input from the user through the terminal
    *   And prints its series type according to ASCII values except "white" characters.
*/
#include <stdio.h>
#include <string.h>

/* Series types. */
#define NOT_ORDER_SERIES 0
#define RISING_SERIES 1
#define REALLY_RISING_SERIES 2
#define DOWN_SERIES 3
#define REALLY_DOWN_SERIES 4

#define INVALID_INDEX -1 /* Invalid index of array. */
#define MAX_ENTERY_TEXT 101 /* Maximum user text entery. */
#define IS_VALID_CHAR(c) (c != '\t' && c != ' ') /* Return true if c is not new line, tab, or white space. */

/* Functions signatures. */
int get_next_valid_index(char str[], int i, int len);
int f_sequence_recursive(char str[], int state, int i, int len);
void f_sequence(char str[]);

/* Returns a character index in a string,
    If the character is "white" it moves next to valid character. */
int get_next_valid_index(char str[], int i, int len)
{
    if(i == len)
        return INVALID_INDEX;
    if(i >= 0 && i <= len && IS_VALID_CHAR(str[i]))
        return i;
    return get_next_valid_index(str, i + 1, len);
}
int f_sequence_recursive(char str[], int state, int i, int len)
{
    int next = get_next_valid_index(str, i + 1, len);
    if(i == len || next == INVALID_INDEX) /* Our exit stragety. */
        return state;
    if(str[i] == str[next] && state == REALLY_RISING_SERIES)
        return f_sequence_recursive(str, RISING_SERIES, next, len);
    else if(str[i] == str[next] && state == REALLY_DOWN_SERIES)
        return f_sequence_recursive(str, DOWN_SERIES, next, len);
    if(str[i] < str[next])
    {
        if(state == DOWN_SERIES || state == REALLY_DOWN_SERIES) /* Detected not order series. */
            return NOT_ORDER_SERIES; 
        return f_sequence_recursive(str, (state == RISING_SERIES ? RISING_SERIES : REALLY_RISING_SERIES), next, len);
    }
    else if(str[i] > str[next]) 
    {
        if(state == RISING_SERIES || state == REALLY_RISING_SERIES) /* Detected not order series. */
            return NOT_ORDER_SERIES; 
        return f_sequence_recursive(str, (state == DOWN_SERIES ? DOWN_SERIES : REALLY_DOWN_SERIES), next, len);
    }
    return f_sequence_recursive(str, RISING_SERIES, next, len);
}
/* Receives array of characters and prints 
    the type of series order by ASCII code values (except: NEW_LINE, TAB, WHITE_SPACE). */
void f_sequence(char str[])
{
    int len = strlen(str) - 1; 
    if(len <= 0)
        printf("\nstring parameter is empty.\n");
    else
    {
        switch(f_sequence_recursive(str, NOT_ORDER_SERIES, 0, len))
        {
            case NOT_ORDER_SERIES:
                printf("\nThe series of characters %s\nis without fixed order.\n", str);
                break;
            case RISING_SERIES:
                printf("\nThe series of characters %s\nis rising series.\n", str);
                break;
            case REALLY_RISING_SERIES:
                printf("\nThe series of characters %s\nis really rising series.\n", str);
                break;
            case DOWN_SERIES:
                printf("\nThe series of characters %s\nis descending series.\n", str);
                break;
            case REALLY_DOWN_SERIES:
                printf("\nThe series of characters %s\nis really descending series.\n", str);
                break;
        }
    }
}
int main()
{
    char text[MAX_ENTERY_TEXT];
    /* Initialize to NULL value */
    memset(text, 0, MAX_ENTERY_TEXT);
    printf("\n\033[0;32mUSAGE: \033[0mf_sequence(char str[])\n\n");
    printf("Enter string: ");
    /* Read function text parameter from user */
    fgets(text, MAX_ENTERY_TEXT, stdin);
    f_sequence(text);
    return 0;
}
