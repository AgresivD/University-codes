/*	 
    * This file gets the data structures from the first file read, and convert them into bits.
*/

/* Includes */
#include "assembler.h"

/* =========== Externs =========== */

/* Labels */
extern Label labelsArr[MAX_FILE_LINES];
extern int labels_Count;

/* Entry Lines */
extern Line* linesArr[MAX_FILE_LINES];
extern int lines_Count;

/* Data */
extern int dataArr[MAX_DATA_NUM];

/* Updates the addresses of all the data labels in labelsArr. */
void updateDataLabelsAddress(int IC)
{
    int i;

    /* Search in the array for label with DATA flag */
    for(i = 0; i < labels_Count; i++)
    {
        if(labelsArr[i].flag == DATA)
            labelsArr[i].address += IC; /* Increase the adress */
    }
}

/* Returns if there is an invalid entry line in linesArr. */
int countInvalidEntries()
{
    int i, ret = 0;
    Label* label;

    for(i = 0; i < lines_Count; i++)
    {
        label = getLabel(linesArr[i]->str);
        if(label)
        {
            if(label->flag == EXTERN)
            {
                printError(linesArr[i]->lineNum, "The parameter for .entry can't be an external label.");
                ret++;
            }
        }
        else
        {
            printError(linesArr[i]->lineNum, "No such label as \"%s\".",linesArr[i]->str);
            ret++;
        }
    }
    return ret;
}

/* Adds the data from dataArr to the end of memoryArr. */
void addDataToMemory(int* memoryArr, int* memoryCounter, int DC)
{
    int i;
    /* Create an int of "MEMORY_WORD_LENGTH" times '1', and all the rest are '0' */
    unsigned int mask = ~0;
    mask >>= (sizeof(int) * BYTE_SIZE - MEMORY_WORD_LENGTH);

    /* Add each int from dataArr to the end of memoryArr */
    for (i = 0; i < DC; i++)
	{
		if (*memoryCounter < MAX_DATA_NUM)
		{
			/* The mask makes sure we only use the first "MEMORY_WORD_LENGTH" bits */
			memoryArr[(*memoryCounter)++] = mask & dataArr[i];
		}
		else
		{
			/* No more space in memoryArr */
			return;
		}
	}
}

/* Reads the data from the first read for the second time. 

    It converts all the lines into the memory. */
int secondFileRead(int* memoryArr, Line* lines, int lineNum, int IC, int DC)
{
    int errorsDetected, memoryCounter, i;
    errorsDetected = memoryCounter = 0;

    /* Update the data labels */
    updateDataLabelsAddress(IC);

    /* Check if there are invalid entries */
    errorsDetected += countInvalidEntries();

    /* Add each line in lines to the memoryArr */
    for(i = 0; i < lineNum; i++)
    {
        /* An error was found while adding the line to the memory */
        if(!addLineToMemory(memoryArr, &memoryCounter, &lines[i]))
        {
            errorsDetected++;
        }
    }

    /* Add the data from dataArr to the end of memoryArr */
    addDataToMemory(memoryArr, &memoryCounter, DC);

    return errorsDetected;
}

Boolean updateLableOpAddress(Operand* op, int lineNum)
{
    if(op->type == LABEL)
    {
        Label* label = getLabel(op->str);
        
        /* Check is a real label name */
        if(label == NULL)
        {
            if(isValidLabel(op->str, lineNum, TRUE))
                printError(lineNum, "No such label as \"%s\".", op->str);

            return FALSE;
        }
        op->value = label->address;
    }
    return TRUE;
}
/* Returns the int value of a memory word. */
int getNumFromMemoryWord(MemoryWord memory)
{
    /* Create an int of "MEMORY_WORD_LENGTH" times '1', and all the rest are '0' */
    unsigned int mask = ~0;
    mask >>= (sizeof(int) * BYTE_SIZE - MEMORY_WORD_LENGTH);

    /* The mask makes sure we only use the first "MEMORY_WORD_LENGTH" bits */
    return mask & ((memory.valueBits.value << 2) + memory.are); 
}

/* Adds the value of memory word to the memoryArr, and increase the memory counter. */
void addWordToMemory(int* memoryArr, int* memoryCounter, MemoryWord memory)
{
    /* Check if memoryArr isn't full yet */
    if(*memoryCounter < MAX_DATA_NUM)
    {
        /* Add the memory word and increase memoryCounter */
        memoryArr[(*memoryCounter)++] = getNumFromMemoryWord(memory);
    }
}

/* Returns the id of the addressing method of the operand */
int getOpTypeId(Operand op)
{
    /* Check if the operand have valid type */
    if(op.type != INVALID)
        return (int)op.type;
    return 0;
}

/* Returns a memory word which represents the command in a line. */
MemoryWord getCmdMemoryWord(Line line)
{
    MemoryWord memory = { 0 };

    /* Updates all the bits in the command word */
    memory.are = (AreType)ABSOLUTE;
    memory.valueBits.cmdBits.dest = getOpTypeId(line.op1);
    memory.valueBits.cmdBits.src = getOpTypeId(line.op2);
    memory.valueBits.cmdBits.opcode = line.cmd->opCode;
    memory.valueBits.cmdBits.funct = line.cmd->funct;

    return memory;
}

/* Returns a memory word which represents the operand (assuming it's a valid operand). */
MemoryWord getOpMemoryWord(Operand op, Boolean isDest)
{
    MemoryWord memory = { 0 };

    /* Check if it's a register or not *//*	 
    * Copyright © Dan Elimelech - All rights reserved
    *
    * This file gets the data structures from the first file read, and convert them into bits.
*/

/* Includes */
#include "assembler.h"

/* =========== Externs =========== */

/* Labels */
extern Label labelsArr[MAX_FILE_LINES];
extern int labels_Count;

/* Entry Lines */
extern Line* linesArr[MAX_FILE_LINES];
extern int lines_Count;

/* Data */
extern int dataArr[MAX_DATA_NUM];

/* Updates the addresses of all the data labels in labelsArr. */
void updateDataLabelsAddress(int IC)
{
    int i;

    /* Search in the array for label with DATA flag */
    for(i = 0; i < labels_Count; i++)
    {
        if(labelsArr[i].flag == DATA)
            labelsArr[i].address += IC; /* Increase the adress */
    }
}

/* Returns if there is an invalid entry line in linesArr. */
int countInvalidEntries()
{
    int i, ret = 0;
    Label* label;

    for(i = 0; i < lines_Count; i++)
    {
        label = getLabel(linesArr[i]->str);
        if(label)
        {
            if(label->flag == EXTERN)
            {
                printError(linesArr[i]->lineNum, "The parameter for .entry can't be an external label.");
                ret++;
            }
        }
        else
        {
            printError(linesArr[i]->lineNum, "No such label as \"%s\".",linesArr[i]->str);
            ret++;
        }
    }
    return ret;
}

/* Adds the data from dataArr to the end of memoryArr. */
void addDataToMemory(int* memoryArr, int* memoryCounter, int DC)
{
    int i;
    /* Create an int of "MEMORY_WORD_LENGTH" times '1', and all the rest are '0' */
    unsigned int mask = ~0;
    mask >>= (sizeof(int) * BYTE_SIZE - MEMORY_WORD_LENGTH);

    /* Add each int from dataArr to the end of memoryArr */
    for (i = 0; i < DC; i++)
	{
		if (*memoryCounter < MAX_DATA_NUM)
		{
			/* The mask makes sure we only use the first "MEMORY_WORD_LENGTH" bits */
			memoryArr[(*memoryCounter)++] = mask & dataArr[i];
		}
		else
		{
			/* No more space in memoryArr */
			return;
		}
	}
}

/* Reads the data from the first read for the second time. 

    It converts all the lines into the memory. */
int secondFileRead(int* memoryArr, Line* lines, int lineNum, int IC, int DC)
{
    int errorsDetected, memoryCounter, i;
    errorsDetected = memoryCounter = 0;

    /* Update the data labels */
    updateDataLabelsAddress(IC);

    /* Check if there are invalid entries */
    errorsDetected += countInvalidEntries();

    /* Add each line in lines to the memoryArr */
    for(i = 0; i < lineNum; i++)
    {
        /* An error was found while adding the line to the memory */
        if(!addLineToMemory(memoryArr, &memoryCounter, &lines[i]))
        {
            errorsDetected++;
        }
    }

    /* Add the data from dataArr to the end of memoryArr */
    addDataToMemory(memoryArr, &memoryCounter, DC);

    return errorsDetected;
}

Boolean updateLableOpAddress(Operand* op, int lineNum)
{
    if(op->type == LABEL)
    {
        Label* label = getLabel(op->str);
        
        /* Check is a real label name */
        if(label == NULL)
        {
            if(isValidLabel(op->str, lineNum, TRUE))
                printError(lineNum, "No such label as \"%s\".", op->str);

            return FALSE;
        }
        op->value = label->address;
    }
    return TRUE;
}
/* Returns the int value of a memory word. */
int getNumFromMemoryWord(MemoryWord memory)
{
    /* Create an int of "MEMORY_WORD_LENGTH" times '1', and all the rest are '0' */
    unsigned int mask = ~0;
    mask >>= (sizeof(int) * BYTE_SIZE - MEMORY_WORD_LENGTH);

    /* The mask makes sure we only use the first "MEMORY_WORD_LENGTH" bits */
    return mask & ((memory.valueBits.value << 2) + memory.are); 
}

/* Adds the value of memory word to the memoryArr, and increase the memory counter. */
void addWordToMemory(int* memoryArr, int* memoryCounter, MemoryWord memory)
{
    /* Check if memoryArr isn't full yet */
    if(*memoryCounter < MAX_DATA_NUM)
    {
        /* Add the memory word and increase memoryCounter */
        memoryArr[(*memoryCounter)++] = getNumFromMemoryWord(memory);
    }
}

/* Returns the id of the addressing method of the operand */
int getOpTypeId(Operand op)
{
    /* Check if the operand have valid type */
    if(op.type != INVALID)
        return (int)op.type;
    return 0;
}

/* Returns a memory word which represents the command in a line. */
MemoryWord getCmdMemoryWord(Line line)
{
    MemoryWord memory = { 0 };

    /* Updates all the bits in the command word */
    memory.are = (AreType)ABSOLUTE;
    memory.valueBits.cmdBits.dest = getOpTypeId(line.op1);
    memory.valueBits.cmdBits.src = getOpTypeId(line.op2);
    memory.valueBits.cmdBits.opcode = line.cmd->opCode;
    memory.valueBits.cmdBits.funct = line.cmd->funct;

    return memory;
}

/* Returns a memory word which represents the operand (assuming it's a valid operand). */
MemoryWord getOpMemoryWord(Operand op, Boolean isDest)
{
    MemoryWord memory = { 0 };

    /* Check if it's a register or not */
    if(op.type == REGISTER)
    {
        /* Registers are absolute */
        memory.are = (AreType)ABSOLUTE;
        if(isDest)
            memory.valueBits.regBits.destBits = op.value;
        else 
            memory.valueBits.regBits.srcBits = op.value; 
    }
    else
    {
        Label* label = getLabel(op.str);
        
        /* Set ARE */
        if(op.type == LABEL && label && label->flag == EXTERN)
            memory.are = EXTERNAL;
        else
            memory.are = (op.type == NUMBER) ? (AreType)ABSOLUTE : (AreType)RELOCATABLE;
        
        memory.valueBits.value = op.value;
    }
    return memory;
}

/* Adds a while line into the memoryArr, and increase the memory counter */
Boolean addLineToMemory(int* memoryArr, int* memoryCounter, Line* line)
{
    Boolean foundError = FALSE;

    /* Don't do anything if the line is error or if it's not a command line */
    if(!line->isError && line->cmd != NULL)
    {
        /* Update the label operands value */
        if(!updateLableOpAddress(&line->op1, line->lineNum) || !updateLableOpAddress(&line->op2, line->lineNum))
        {
            line->isError = TRUE;
            foundError = TRUE;
        }

        /* Add the command word to the memory */
        addWordToMemory(memoryArr, memoryCounter, getCmdMemoryWord(*line));

        if(line->op1.type == REGISTER && line->op2.type == REGISTER)
        {
            /* Create the memory word */
            MemoryWord memory = { 0 };
            memory.are = (AreType)ABSOLUTE;
            memory.valueBits.regBits.srcBits = line->op1.value;
            memory.valueBits.regBits.destBits = line->op2.value;

            /* Add the memory word to the memoryArr array */
            addWordToMemory(memoryArr, memoryCounter, memory);
        }
        else
        {
            /* Check if there is a source operand in this line */
            if(line->op1.type == INVALID)
            {
                /* Add the op1 word to the memory */
                line->op1.address = FIRST_ADDRESS + *memoryCounter;
                addWordToMemory(memoryArr, memoryCounter, getOpMemoryWord(line->op1, FALSE));
                /* ^^ The FALSE param means it's not the 2nd op */
            }

            /* Check if there is a destination operand in this line */
            if(line->op2.type != INVALID)
            {
                /* Add the op2 word to the memory */
                line->op2.address = FIRST_ADDRESS + *memoryCounter;
                addWordToMemory(memoryArr, memoryCounter, getOpMemoryWord(line->op2, TRUE));
            }
        }
    }
    return !foundError;
}
    if(op.type == REGISTER)
    {
        /* Registers are absolute */
        memory.are = (AreType)ABSOLUTE;
        if(isDest)
            memory.valueBits.regBits.destBits = op.value;
        else 
            memory.valueBits.regBits.srcBits = op.value; 
    }
    else
    {
        Label* label = getLabel(op.str);
        
        /* Set ARE */
        if(op.type == LABEL && label && label->flag == EXTERN)
            memory.are = EXTERNAL;
        else
            memory.are = (op.type == NUMBER) ? (AreType)ABSOLUTE : (AreType)RELOCATABLE;
        
        memory.valueBits.value = op.value;
    }
    return memory;
}

/* Adds a while line into the memoryArr, and increase the memory counter */
Boolean addLineToMemory(int* memoryArr, int* memoryCounter, Line* line)
{
    Boolean foundError = FALSE;

    /* Don't do anything if the line is error or if it's not a command line */
    if(!line->isError && line->cmd != NULL)
    {
        /* Update the label operands value */
        if(!updateLableOpAddress(&line->op1, line->lineNum) || !updateLableOpAddress(&line->op2, line->lineNum))
        {
            line->isError = TRUE;
            foundError = TRUE;
        }

        /* Add the command word to the memory */
        addWordToMemory(memoryArr, memoryCounter, getCmdMemoryWord(*line));

        if(line->op1.type == REGISTER && line->op2.type == REGISTER)
        {
            /* Create the memory word */
            MemoryWord memory = { 0 };
            memory.are = (AreType)ABSOLUTE;
            memory.valueBits.regBits.srcBits = line->op1.value;
            memory.valueBits.regBits.destBits = line->op2.value;

            /* Add the memory word to the memoryArr array */
            addWordToMemory(memoryArr, memoryCounter, memory);
        }
        else
        {
            /* Check if there is a source operand in this line */
            if(line->op1.type == INVALID)
            {
                /* Add the op1 word to the memory */
                line->op1.address = FIRST_ADDRESS + *memoryCounter;
                addWordToMemory(memoryArr, memoryCounter, getOpMemoryWord(line->op1, FALSE));
                /* ^^ The FALSE param means it's not the 2nd op */
            }

            /* Check if there is a destination operand in this line */
            if(line->op2.type != INVALID)
            {
                /* Add the op2 word to the memory */
                line->op2.address = FIRST_ADDRESS + *memoryCounter;
                addWordToMemory(memoryArr, memoryCounter, getOpMemoryWord(line->op2, TRUE));
            }
        }
    }
    return !foundError;
}
