/*	 
    * Copyright © Dan Elimelech - All rights reserved
    *
    * This file contains the assembling export methods.
*/

/* Includes */
#include "assembler.h"
#include <ctype.h>
#include <stdlib.h>

/* =========== Externs =========== */

/* Entry Lines */
extern Line* linesArr[MAX_FILE_LINES];
extern int lines_Count;

/* Creates the .obj file, which contains the assembled lines. */
void createObjectFile(char *name, int IC, int DC, int *memoryArr)
{
    int i;
    FILE* file;
    file = openFile(name, ".ob", "w");

    fprintf(file, "%d\t\t%d", IC, DC);

    /* Print all of memoryArr */
    for(i = 0; i < IC + DC; i++)
        fprintf(file, "\n%07d\t\t%06x", FIRST_ADDRESS + i, memoryArr[i]);

    fclose(file);
}

/* Creates the .ent file, which contains the addresses for the .entry labels. */
void createEntriesFile(char* name)
{
    int i;
    FILE* file;

    /* Don't create the entries file if there aren't entry lines */
    if(!lines_Count)
        return;

    file = openFile(name, ".ent", "w");
    for(i = 0; i < lines_Count; i++)
    {
        fprintf(file, "%s\t\t%07d", linesArr[i]->str, getLabel(linesArr[i]->str)->address);
        if(i != lines_Count - 1)
            fprintf(file, "\n");
    }
    
    fclose(file);
}

/* Creates the .ext file, which contains the addresses for the extern labels operands. */
void createExternFile(char* name, Line* lines, int linesFound)
{
    int i;
    Label* label;
    Boolean firstPrint = TRUE; /* This bool ment to prevent the creation of the file if there aren't any externs */
    FILE* file = NULL;

    for(i = 0; i < linesFound; i++)
    {
        /* Check if the 1st operand is extern label, and print it. */
        if(lines[i].cmd && lines[i].cmd->params >= 2 && lines[i].op1.type == LABEL)
        {
            label = getLabel(lines[i].op1.str);
            if(label && label->flag == EXTERN)
            {
                if(firstPrint)
                    file = openFile(name, ".ext", "w");
                else
                    fprintf(file, "\n");

                fprintf(file, "%s\t\t%07d", label->name, lines[i].op1.address);
                firstPrint = FALSE;
            }
        }

        /* Check if the 2nd operand is extern label, and print it */
        if(lines[i].cmd && lines[i].cmd->params >= 1 && lines[i].op2.type == LABEL)
        {
            label = getLabel(lines[i].op2.str);
            if(label && label->flag == EXTERN)
            {
                if(firstPrint)
                    file = openFile(name, ".ext", "w");
                else
                    fprintf(file, "\n");
                
                fprintf(file, "%s\t\t%07d", label->name, lines[i].op2.address);
                firstPrint = FALSE;
            }
        }
    }
    if(file)
        fclose(file);
}
