/*	 
	 * Copyright © Dan Elimelech - All rights reserved
	 *
	 * An interactive program that reads commands from the standrad input,
	 * decodes and execute them. The commands deal with arithmetic oprations on complex numbers.
	 *
	 * This code was prepared for a laboratory course in systems programming.
*/
#include <stdio.h>
#include <string.h>
#include "complex.h"

/* Program Definitions. */
#define MAX_LINE_SIZE 256
#define MAX_CMD_SIZE 14
#define MAX_PARAMS 2
#define MAX_PARAM_SIZE 32
#define INVALID_COMP 0

/* Params positions definitions. */
#define PARAM_A 0
#define PARAM_B 1

/* Commands params definitions.*/ 
#define PRINT_COMP_PARAMS 0
#define READ_COMP_PARAMS 2
#define ADD_SUB_COMP_PARAMS 1
#define REAL_IMG_COMP_MULT_PARAMS 1
#define MULT_COMP_COMP_PARAMS 1

int main()
{   
    /* Program variables. */
    char 
        comp,
        input[MAX_LINE_SIZE],
        cmd[MAX_CMD_SIZE],
        **params = (char**)malloc(MAX_PARAMS * sizeof(char*)),
        params_str[MAX_PARAMS * MAX_PARAM_SIZE] 
    ;
    struct Complex* complex[MAX_PROG_COMPS];
    /* Initilaize complex array. */
    init_complex_arr(complex, MAX_PROG_COMPS);
 
    printf("\n>> Complex Running...\n\n");
    printf("Enter your complex command:");
    
    /* Command entery section. */
    cmd_entery:
    printf("\n\n"); 

    /* Reset for the next command. */
    comp = '\0';
    strcpy(input, "");
    strcpy(params_str, "");
    
    /* Reads from user standrad input. */
    fgets(input, MAX_LINE_SIZE, stdin);

    /* Parsing input. */
    if(sscanf(input, "%s %c, %[^\n]", cmd, &comp, params_str))
    {
        int i, c_params;
        /* Memory allocation for params. */
        for(i = 0, c_params = 0; i < MAX_PARAMS; i++)
            params[i] = (char*)malloc(MAX_PARAM_SIZE + 1);

        /* Parsing parameters & count if there any. */
        if(params_str[0] != '\0')
        {
            char *param = strtok(params_str, ", \t");
            while(param) 
            {
                if(c_params < MAX_PARAMS)
                    strcpy(params[c_params], param);
                c_params++;
                param = strtok(NULL, ", \t");
            }
        }
        if(strcmp(cmd, "print_comp") == 0)
        {
            if(IsValidComplexVar(comp) == FALSE)
            {
                print_error("Undefined complex variable.");
                goto cmd_entery;
            }
            if(c_params > PRINT_COMP_PARAMS || params_str[0] != '\0')
            {
                print_error("Extraneous text after end of command.");
                goto cmd_entery;
            }
            printf("\n%c = ", comp);
            print_comp(complex[comp - 'A'], FALSE);
            goto cmd_entery;
        }
        else if(strcmp(cmd, "read_comp") == 0)
        {
            if(IsValidComplexVar(comp) == FALSE)
            {
                print_error("Undefined complex variable.");
                goto cmd_entery;
            }
            if(c_params < READ_COMP_PARAMS)
            {
                print_error("Missing parameter.");
                goto cmd_entery;  
            }
            if(IsFloat(params[PARAM_A]) == FALSE || IsFloat(params[PARAM_B]) == FALSE)
            {
                print_error("Invalid parameter - not a number.");
                goto cmd_entery;  
            }
            if(c_params > READ_COMP_PARAMS)
            {
                print_error("Extraneous text after end of command.");
                goto cmd_entery;
            }
            read_comp(complex[comp - 'A'], atof(params[PARAM_A]), atof(params[PARAM_B]));
            printf("\n%c = ", comp);
            print_comp(complex[comp - 'A'], FALSE);
            goto cmd_entery;
        }
        else if(strcmp(cmd, "add_comp") == 0 || strcmp(cmd, "sub_comp") == 0)
        {
            Boolean isSub = (strcmp(cmd, "sub_comp") == 0);
            if(c_params < ADD_SUB_COMP_PARAMS)
            {
                print_error("Missing parameter.");
                goto cmd_entery;  
            }
            if(c_params > ADD_SUB_COMP_PARAMS)
            {
                print_error("Extraneous text after end of command.");
                goto cmd_entery;  
            }
            if(strlen(params[PARAM_A]) != 1 || IsValidComplexVar(params[PARAM_A][0]) == FALSE || IsValidComplexVar(comp) == FALSE)
            {
                print_error("Undefined complex variable.");
                goto cmd_entery;
            }
            char _comp = params[PARAM_A][0];
            if(!isSub)
                add_comp(complex[comp - 'A'], complex[_comp - 'A']);
            else 
                sub_comp(complex[comp - 'A'], complex[_comp - 'A']);
            free(params);
            goto cmd_entery;
        }
        else if(strcmp(cmd, "mult_comp_real") == 0 || strcmp(cmd, "mult_comp_img") == 0)
        {
            Boolean isImg = (strcmp(cmd, "mult_comp_img") == 0);
            if(IsValidComplexVar(comp) == FALSE)
            {
                print_error("Undefined complex variable.");
                goto cmd_entery;
            }
            if(c_params < REAL_IMG_COMP_MULT_PARAMS)
            {
                print_error("Missing parameter.");
                goto cmd_entery;  
            }
            if(c_params > REAL_IMG_COMP_MULT_PARAMS)
            {
                print_error("Extraneous text after end of command.");
                goto cmd_entery;
            }
            if(!IsFloat(params[PARAM_A]))
            {
                print_error("Invalid parameter - not a number.");
                goto cmd_entery;  
            }
            if(!isImg)
                mult_comp_real(complex[comp - 'A'], atof(params[PARAM_A]));
            else
                mult_comp_img(complex[comp - 'A'], atof(params[PARAM_A]));
            free(params);
            goto cmd_entery;
        }
        else if(strcmp(cmd, "mult_comp_comp") == 0)
        {
            if(IsValidComplexVar(comp) == FALSE)
            {
                print_error("Undefined complex variable.");
                goto cmd_entery;
            }
            if(c_params < MULT_COMP_COMP_PARAMS)
            {
                print_error("Missing parameter.");
                goto cmd_entery;  
            }
            if(c_params > MULT_COMP_COMP_PARAMS)
            {
                print_error("Extraneous text after end of command.");
                goto cmd_entery;
            }
            if(strlen(params[PARAM_A]) != 1 || IsValidComplexVar(params[PARAM_A][0]) == FALSE)
            {
                print_error("Undefined complex variable.");
                goto cmd_entery;  
            }
            mult_comp_comp(complex[comp - 'A'], complex[params[PARAM_A][0] - 'A']);
            free(params);
            goto cmd_entery;
        }
        else if(strcmp(cmd, "abs_comp") == 0)
        {
            if((int)comp == INVALID_COMP)
            {
                print_error("Missing parameter.");
                goto cmd_entery;  
            }
            if(IsValidComplexVar(comp) == FALSE)
            {
                print_error("Undefined complex variable.");
                goto cmd_entery;
            }
            abs_comp(complex[comp - 'A']);
            goto cmd_entery;
        }
        else if(strcmp(cmd, "stop") == 0)
        {
            if((int)comp != INVALID_COMP)
            {
                print_error("Extraneous text after end of command.");
                goto cmd_entery;
            }
            printf("\nThe program was successfully stopped.\n\n");
        }
        else
        {
            print_error("Undefined command name.");
            goto cmd_entery;
        }
    }
    return 0;
}
