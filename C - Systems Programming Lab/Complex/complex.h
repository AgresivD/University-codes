#ifndef COMPLEX_H
#define COMPLEX_H
/*	 
	 * An interactive program that reads commands from the standrad input,
	 * decodes and execute them. The commands deal with arithmetic oprations on complex numbers.
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*  Amount of variables that will be allocated to the program. */
#define MAX_PROG_COMPS 6

/* Complex model. */
typedef struct Complex
{
    float realNum;
    float imgNum;
} Complex;

/* Boolean type. */
typedef enum 
{
    FALSE = 0,
    TRUE = 1
} Boolean;

/* Arithmetic and I/O signatures. */
void read_comp(Complex* comp, float real, float img);
void print_comp(Complex* comp, Boolean newline);
void add_comp(Complex* comp_a, Complex* comp_b);
void sub_comp(Complex* comp_a, Complex* comp_b);
void mult_comp_real(Complex* comp, float real);
void mult_comp_img(Complex* comp, float img);
void mult_comp_comp(Complex* comp_a, Complex* comp_b);
void abs_comp(Complex* comp);

/* Utilities */
void init_complex_arr(struct Complex* complex[], int size);
Boolean IsFloat(char* str);
Boolean IsValidCommand(char cmd[]);
Boolean IsValidComplexVar(char var);
void print_error(char message[]);

#endif
