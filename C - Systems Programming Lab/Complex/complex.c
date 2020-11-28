/*	 
	 * An interactive program that reads commands from the standrad input,
	 * decodes and execute them. The commands deal with arithmetic oprations on complex numbers.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "complex.h"

/* 
    Allocates memory for a Complex array by size,
    And initializes each complex element to be 0.0 + 0.0i
*/
void init_complex_arr(struct Complex* complex[], int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        complex[i] = (Complex*)malloc(sizeof(Complex));
        read_comp(complex[i], 0.0, 0.0);
    }
}
/* 
    Placing a complex number in a variable. 
*/
void read_comp(Complex* comp, float real, float img)
{
    comp->realNum = real;
    comp->imgNum = img;
}
/* 
    Print value of a complex element. 
*/
void print_comp(Complex* comp, Boolean newline)
{
    char sign = (comp->imgNum >= 0) ? '+' : '-';
    if(newline == TRUE)
        printf("\n");
    printf("%.2f %c (%.2f)i", comp->realNum, sign, sign == '-' ? (-1) * comp->imgNum : comp->imgNum);
}
/* 
    Adds two complex elements and print the result.
*/
void add_comp(Complex* comp_a, Complex* comp_b)
{
    Complex* result = (Complex*)malloc(sizeof(Complex));
    result->realNum = comp_a->realNum + comp_b->realNum;
    result->imgNum = comp_a->imgNum + comp_b->imgNum;
    print_comp(result, TRUE);
    free(result);
}
/* 
    Subracts two complex elements and print the result.
*/
void sub_comp(Complex* comp_a, Complex* comp_b)
{
    Complex* result = (Complex*)malloc(sizeof(Complex));
    result->realNum = comp_a->realNum - comp_b->realNum;
    result->imgNum = comp_a->imgNum - comp_b->imgNum;
    print_comp(result, TRUE);
    free(result);
}
/* 
    Multiply complex in real number.
*/
void mult_comp_real(Complex* comp, float real)
{
    Complex* result = (Complex*)malloc(sizeof(Complex));
    result->realNum = comp->realNum * real;
    result->imgNum = comp->imgNum * real;
    print_comp(result, TRUE);
    free(result);
}
/* 
    Multiply complex in imagination number.
*/
void mult_comp_img(Complex* comp, float img)
{
    Complex* result = (Complex*)malloc(sizeof(Complex));
    result->realNum = ((-1) * comp->imgNum * img);
    result->imgNum = comp->realNum * img;
    print_comp(result, TRUE);
    free(result);
}
/* 
    Multiplier two complex elements and print the result.
*/
void mult_comp_comp(Complex* comp_a, Complex* comp_b)
{
    Complex* result = (Complex*)malloc(sizeof(Complex));
    result->realNum = (comp_a->realNum * comp_b->realNum) - (comp_a->imgNum * comp_b->imgNum);
    result->imgNum = (comp_a->realNum * comp_b->imgNum) + (comp_a->imgNum * comp_b->realNum);
    print_comp(result, TRUE);
    free(result);
}
/* 
    Prints the absolute value of complex element.
*/
void abs_comp(Complex* comp)
{
    float result = fabs((sqrt(pow(comp->realNum, 2) + pow(comp->imgNum, 2))));
    printf("%.2f\n\n", result);
}
/* 
    Returns TRUE if string is a decimal number.
*/
Boolean IsFloat(char* str)
{
    int len, ret;
    float num;
    ret = sscanf(str, "%f %n", &num, &len);
    return ret ? TRUE : FALSE;
}
/* 
    Returns TRUE if var is valid complex variable in our program.
*/
Boolean IsValidComplexVar(char var)
{
    if((var - 'A') >= 0 && (var - 'A') < MAX_PROG_COMPS)
        return TRUE;
    return FALSE;
}
/* 
    Prints error using color.
*/
void print_error(char message[])
{
    printf("\n\033[1;31m[Error]:\033[0m %s", message);
}
