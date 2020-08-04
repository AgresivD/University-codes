/*
    * File: match.c
    * 
    *   The program receives two strings from the user (pattern and text)
    *   And uses the string matching function that returns the position with the smallest number of discrepancies.
        
        * @Task: Maman11 
        * @Author: Dan Elimelech
*/
#include <stdio.h>
#include <string.h>

/* Definitions */
#define INVALID_MISMATCHES -1
#define MAX_USER_ENTERY 100
#define RESULT(res) (res == INVALID_MISMATCHES ? 0 : res)

/* Functions signatures */
int lowest_mismatches_recursive(char pattern[], int plen, char text[], int txtlen, int i, int k, int tmp, int res);
int lowest_mismatches(char pattern[], char text[]);

/* Recursive function that performs the algorithm */
int lowest_mismatches_recursive(char pattern[], int plen, char text[], int txtlen, int i, int k, int tmp, int res)
{
    if(i == plen) /* If we through all the positions return the best result */
        return RESULT(res);
    if(k >= 0 && txtlen >= plen)
    {
        if(pattern[k] != text[i + k]) /* Condition in case there is no match */
            return lowest_mismatches_recursive(pattern, plen, text, txtlen, ((k == plen) ? i + 1 : i), 
                ((k == plen) ? 0 : k + 1), ((k == plen) ? 0 : tmp + 1), 
                    ((k == plen) ? ((tmp + 1 < res || res == INVALID_MISMATCHES ? tmp + 1 : res)) : res));
        return lowest_mismatches_recursive(pattern, plen, text, txtlen, ((k == plen) ? i + 1 : i), 
            ((k == plen) ? 0 : k + 1), ((k == plen) ? 0 : tmp), 
                ((k == plen) ? ((tmp < res || res == INVALID_MISMATCHES ? tmp : res)) : res));
    }
    return 0;
}
/* Function as requested */
int lowest_mismatches(char pattern[], char text[])
{
    return lowest_mismatches_recursive(pattern, strlen(pattern) - 1, text, 
        strlen(text) - 1, 0, 0, 0, INVALID_MISMATCHES);
}
int main()
{
    char pattern[MAX_USER_ENTERY], text[MAX_USER_ENTERY];
    /* Initialize to NULL values */
    memset(pattern, 0, MAX_USER_ENTERY);
    memset(text, 0, MAX_USER_ENTERY);
    /* Read function parameters from user */
    printf("\n\033[0;32mUSAGE: \033[0mlowest_mismatches(char pattern[], char text[])\n\n");
    printf("Enter the pattern: ");
    scanf("%s", pattern);
    printf("Enter the text: ");
    scanf("%s", text);
    /* Print the result.*/
    printf("\nlowest_mismatches result is %d\n", lowest_mismatches(pattern, text));
    return 0;
}
