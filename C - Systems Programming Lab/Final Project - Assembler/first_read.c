/*	 
    * This file reads the lines from the file, and convert them into data structures.
*/

/* Includes */
#include "assembler.h"
#include <ctype.h>
#include <stdlib.h>

/* =========== Directives Parsing Events =========== */
void parseDataDirective(Line* line, int* IC, int* DC);
void parseStringDirective(Line* line, int* IC, int* DC);
void parseEntryDirective(Line* line);
void parseExternDirective(Line* line);

const Directive Directives[] = 
{
    /* Name | Parsing function */
    { "data", parseDataDirective },
    { "string", parseStringDirective },
    { "entry", parseEntryDirective },
    { "extern", parseExternDirective },
    { NULL } /* represent the end of the array */
};

const Command Commands[] = 
{
    /* Name | Funct | Opcode (Decimal) | Params */
    { "mov", 0, 0, 2 },
    { "cmp", 0, CMP_OPCODE, 2 },
    { "add", 1, 2, 2 },
    { "sub", 2, 2, 2 },
    { "lea", 0, LEA_OPCODE, 2 },
    { "clr", 1, 5, 1 },
    { "not", 2, 5, 2 },
    { "inc", 3, 5, 1 },
    { "dec", 4, 5, 1 },
    { "jmp", 1, 9, 1 },
    { "bne", 2, 9, 1 },
    { "jsr", 3, 9, 1 },
    { "red", 0, 12, 1 },
    { "prn", 0, PRN_OPCODE, 1 },
    { "rts", 0, 14, 0 },
    { "stop", 0, 15, 0 },
    { NULL } /* represent the end of the array */
};

/* =========== Externs =========== */

/* Labels */
extern Label labelsArr[MAX_FILE_LINES];
extern int labels_Count;

/* Entry Lines */
extern Line* linesArr[MAX_FILE_LINES];
extern int lines_Count;

/* Data */
extern int dataArr[MAX_DATA_NUM];

/* Reading the file for the first time, line by line, and parsing it. 

    Returns how many errors were found. */
int firstFileRead(FILE *file, Line *lines, int *linesFound, int *IC, int *DC)
{
    char lineStr[MAX_LINE_LENGTH + 2]; /* +2 for the \n and \0 at the end */
    int errorsDetected = 0;

    *linesFound = 0;

    /* Read lines and parse them. */
    while(!feof(file))
    {
        if(readLine(file, lineStr, MAX_LINE_LENGTH + 2))
        {
            if(*linesFound >= MAX_FILE_LINES)
            {
                printError(INVALID_LINE_NUM, "File is too long. Max Lines number in file is: %d", MAX_FILE_LINES);
                return ++errorsDetected;
            }

            /* Parse a line */
            parseLine(&lines[*linesFound], lineStr, *linesFound + 1, IC, DC);

			/* Update errorsDetected */
			if (lines[*linesFound].isError)
                ++errorsDetected;

            /* Check if the number of memory words needed is small enough */
            if(*IC + *DC >= MAX_DATA_NUM)
            {
                /* dataArr is full. Stop reading the file. */
                printError(*linesFound + 1, "Too much data and code. Max memory words is %d.", MAX_DATA_NUM);
				printf("[Info] Memory is full. Stoping to read the file.\n");
				return ++errorsDetected;
            }
            ++*linesFound;
        }
        else if(!feof(file))
        {
            /* Line is too long */
            printError(*linesFound + 1, "Line is too long. Max line length is %d.", MAX_LINE_LENGTH);
            errorsDetected++;
            ++*linesFound;
        }
    }
    return errorsDetected;
}

/* Puts a line from 'file' in 'buf'. 
    Returns if the line is shorter than maxLength. */
Boolean readLine(FILE *file, char *buf, size_t maxLength)
{
    char* endOfLine;
    if(!fgets(buf, maxLength, file))
        return FALSE;
    
    /* Check if the line is too long (no '\n' was present). */
    endOfLine = strchr(buf, '\n');
    if(endOfLine)
        *endOfLine = '\0';
    else
    {
        char c;
        /* Return FALSE, unless it's the end of the file */
        Boolean ret = (feof(file) ? TRUE : FALSE);
        
        /* Keep reading chars until you reach the end of the line ('\n') or EOF */
        do 
        {
            c = fgetc(file);
        } while(c != '\n' && c != EOF);

        return ret;
    }
    return TRUE;
}

/* Finds the label in line->lineStr and add it to the label list. 

    Returns a pointer to the next char after the label, or NULL is there isn't a legal label. */
char *findLabel(Line *line, int IC)
{
    char *labelEnd = strchr(line->str, ':');
    Label label = { 0 };
    label.address = FIRST_ADDRESS + IC;

    /* Find the label (or return NULL if there isn't) */
    if(!labelEnd)
        return NULL;

    *labelEnd = '\0';

    /* Check if the ':' came after the first word */
    if(!isOneWord(line->str))
    {
        *labelEnd = ':'; /* Fix the change in line->str */
        return NULL;
    }

    /* Check of the label is legal and add it to the labels list */
    line->label = appendLabel(label, line);

    return labelEnd + 1; /* +1 to make it point at the next char after the '\0' */
}

/* Adds the number to the dataArr and increases DC. Returns TRUE if it succeeded. */
Boolean addNumberToData(int num, int* IC, int* DC, int lineNum)
{
    if(*DC + *IC < MAX_DATA_NUM)
        dataArr[(*DC)++] = num;
    else
        return FALSE;
    return TRUE;
}

/* Adds the str to the dataArr and increases DC. Returns TRUE if it succeeded. */
Boolean addStringToData(char *str, int* IC, int* DC, int lineNum)
{
    do
    {
        if(!addNumberToData((int)*str, IC, DC, lineNum))
            return FALSE;
    } while (*str++);

    return TRUE;
}

/* Parses a .data directive. */
void parseDataDirective(Line* line, int* IC, int* DC)
{
    char* operandTok = line->str, *endOfOp = line->str;
    int operandValue;
    Boolean foundComma;

    /* Make the label a data label (is there is one) */
    if(line->label)
    {
        line->label->flag = DATA;
        line->label->address = FIRST_ADDRESS + *DC;
    }

    /* Check if there are params */
    if(isWhiteSpaces(line->str))
    {
        /* No parameters */
        printError(line->lineNum, "No parameter");
        line->isError = TRUE;
        return;
    }

    /* Find all the params and add them to dataArr */
    FOREVER
    {
        /* Get next param or break if there isn't */
        if(isWhiteSpaces(line->str))
            break;
        operandTok = getFirstOperand(line->str, &endOfOp, &foundComma);

        /* Add the param to dataArr */
        if(isValidNumberParam(operandTok, MEMORY_WORD_LENGTH, line->lineNum, &operandValue))
        {
            if(!addNumberToData(operandValue, IC, DC, line->lineNum))
            {
                /* Not enough memory */
                line->isError = TRUE;
                return;
            }
        }
        else
        {
            /* Invalid number */
            line->isError = TRUE;
            return;
        }
        /* Change the line to start after the parameter */
        line->str = endOfOp;
    }
    /* Comma after the last param */
    if(foundComma)
    {
		printError(line->lineNum, "Do not write a comma after the last parameter.");
		line->isError = TRUE;
		return;
    }
}

/* Parses a .string directive. */
void parseStringDirective(Line* line, int* IC, int* DC)
{
    /* Make the label a data label (is there is one) */
    if(line->label)
    {
        line->label->flag = DATA;
        line->label->address = FIRST_ADDRESS + *DC;
    }
    
    trimStr(&line->str);

    if(isValidStringParam(&line->str, line->lineNum))
    {
        if(!addStringToData(line->str, IC, DC, line->lineNum))
        {
            /* Not enough memory */
            line->isError = TRUE;
            return;
        }
    }
    else
    {
        /* Invalid string */
        line->isError = TRUE;
        return;
    }
}
/* Omits the last label in labelArr by updating g_labelNum. 

    Used to remove the label from a entry/extern line. */
void removeLastLabel(int lineNum)
{
    labels_Count--;
    printf("[Warning] At line %d: The assembler ignored the label before the directive.\n", lineNum);
}

/* Parses a .entry directive. */
void parseEntryDirective(Line* line)
{
    /* If there is a label in the line, remove it from labelsArr */
    if(line->label)
        removeLastLabel(line->lineNum);
    
    /* Add the label to the entry labels list */
    trimStr(&line->str);

    if(isValidLabel(line->str, line->lineNum, TRUE))
    {
        if(isExistingEntryLabel(line->str))
        {
            printError(line->lineNum, "Label already defined as an entry label.");
            line->isError = TRUE;
        }
        else if(lines_Count < MAX_LABELS_NUM)
            linesArr[lines_Count++] = line;

    }
}

/* Parses a .extern directive. */
void parseExternDirective(Line* line)
{
    Label label = { 0 }, *labelPtr;

    /* If there is a label in the line, remove it from labelsArr */
    if(line->label)
        removeLastLabel(line->lineNum);
    
    trimStr(&line->str);
    labelPtr = appendLabel(label, line);

    /* Make the label an extern label */
    if(!line->isError)
    {
        labelPtr->address = 0;
        labelPtr->flag = EXTERN;
    }
}

/* Adds the label to the labelsArr and increases labels_Count.
    
    Returns a pointer to the label in the array. */
Label *appendLabel(Label label, Line* line)
{
    /* Check if label is valid */
    if(!isValidLabel(line->str, line->lineNum, TRUE))
    {
        /* Invalid Label */
        line->isError = TRUE;
        return NULL;
    }

    /* Check if label is already exists */
    if(isLabelExists(line->str))
    {
        printError(line->lineNum, "Label already exists.");
        line->isError = TRUE;
        return NULL;
    }

    /* Add the name to the label */
    strcpy(label.name, line->str);

    /* Too many labels. */
    if(labels_Count >= MAX_LABELS_NUM)
    {
        printError(line->lineNum, "Too many labels - Max is %d", MAX_LABEL_LENGTH);
        line->isError = TRUE;
        return NULL;
    }

    /* Add the label to labelsArr */
    labelsArr[labels_Count] = label;
    return &labelsArr[labels_Count++];
}

/* Parses the directive and in a directive line. */
void parseDirective(Line* line, int* IC, int* DC)
{
    int i = 0;
    while(Directives[i].name)
    {
        if(!strcmp(line->cmdString, Directives[i].name))
        {
            /* Call the parse function for this type of directive */
            Directives[i].callback(line, IC, DC);
            return;
        }
        i++;
    }
    /* line->cmdString isn't a valid directive */
    printError(line->lineNum, "No such directive as \"%s\".", line->cmdString);
    line->isError = TRUE;
}

/* Parses the oprands in a command line. */
void parseCmdOperands(Line* line, int* IC, int* DC)
{
    char* startOfNext = line->str;
    Boolean foundComma = FALSE;
    int opsFound = 0;

    /* Reset the op types */
    line->op1.type = INVALID;
    line->op2.type = INVALID;

    /* Gets the parameters */
    FOREVER
    {
        /* If both of the oprands are registers, they will only take 1 memory word (instead of 2) */
        if(!(line->op1.type == REGISTER && line->op2.type == REGISTER))
        {
            /* Check if there is enough memory */
            if(*IC + *DC < MAX_DATA_NUM)
                ++*IC; /* Count the last command word or operand */
            else
                line->isError = TRUE;
        }

        /* Check if there are still more operands to read */
        if(isWhiteSpaces(line->str) || opsFound > 2)
            break; /* If there are more than 2 operands it's already illegal */
        
        /* If there are 2 ops, make the destination become the source op */
        if(opsFound == 1)
        {
            line->op1 = line->op2;
            /* Reset op2 */
            line->op2.type = INVALID;
        }

        /* Parse the operand */
        line->op2.str = getFirstOperand(line->str, &startOfNext, &foundComma);
        parseOpInfo(&line->op2, line->lineNum);

        if(line->op2.type == INVALID)
        {
            line->isError = TRUE;
            return;
        }
        opsFound++;
        line->str = startOfNext;
    } /* End of while */

    /* Check if there are enough operands */
    if(opsFound != line->cmd->params)
    {
        /* There are more/less operands than needed */
        printError(line->lineNum, "%s operands.", (opsFound < line->cmd->params) ? "Not enough" : "Too many");
        line->isError = TRUE;
        return;
    }

    /* Check if there is a comma after the last param */
    if(foundComma)
    {
        printError(line->lineNum, "Don't write a comma after the last parameter.");
        line->isError = TRUE;
        return;
    }

    /* Check if the operands types are valid */
    if(!checkValidOpTypes(line->cmd, line->op1, line->op2, line->lineNum))
    {
        line->isError = TRUE;
        return;
    }
}

/* Returns TRUE if the operands types are correct (depending on the command). */
Boolean checkValidOpTypes(const Command* cmd, Operand op1, Operand op2, int lineNum)
{
    /* Check first operand */
    /* lea command (opcode is 4) can only get a label as the 1st op */
    if(cmd->opCode == LEA_OPCODE && op1.type != LABEL)
    {
        printError(lineNum, "Source operand for \"%s\" command must be a label.", cmd->name);
        return FALSE;
    }

    /* 2nd operand can be a number only if the command is "cmp" (opcode is 1) or "prn" (opcode is 13).*/
	if (op2.type == NUMBER && cmd->opCode != CMP_OPCODE && cmd->opCode != PRN_OPCODE)
	{
		printError(lineNum, "Destination operand for \"%s\" command can't be a number.", cmd->name);
		return FALSE;
	}
    return TRUE;
}

/* Updates the type and value of operand. */
void parseOpInfo(Operand* operand, int lineNum)
{
    int value = 0;

    if(isWhiteSpaces(operand->str))
    {
        printError(lineNum, "Empty parameter.");
        operand->type = INVALID;
        return;
    }
    
    /* Check if the type is NUMBER */
    if(*operand->str == '#')
    {
        operand->str++; /* Remove the '#' */

        /* Check if the number is valid */
        if(isspace(*operand->str))
        {
            printError(lineNum, "There is a white space after the '#'.");
            operand->type = INVALID;
        }
        else
            operand->type = isValidNumberParam(operand->str, MEMORY_WORD_LENGTH - 2, lineNum, &value) ? NUMBER : INVALID;
    }
    /* Check if the type is REGISTER */
    else if(isValidRegister(operand->str, &value))
        operand->type = REGISTER;
    /* Check if the type is LABEL */
    else if(isValidLabel(operand->str, lineNum, FALSE))
        operand->type = LABEL;
    /* The type is invalid */
    else
    {
        printError(lineNum, "\"%s\" is an invalid parameter.", operand->str);
		operand->type = INVALID;
		value = -1;
    }
    operand->value = value;
}

/* Parses the command in a command line. */
void parseCommand(Line* line, int* IC, int* DC)
{
    int cmdIndex = indexOfCommand(line->cmdString);
    if(cmdIndex == INVALID_CMD_INDEX)
    {
        line->cmd = NULL;
        /* The command is empty, but the line isn't empty so it's only a label. */
        if(*line->cmdString == '\0') 
            printError(line->lineNum, "Cant write a label to an empty line.");
        /* Invalid command */
        else
            printError(line->lineNum, "No such command as \"%s\".", line->cmdString);
        line->isError = TRUE;
        return;
    }
    line->cmd = &Commands[cmdIndex];
    parseCmdOperands(line, IC, DC);
}

/* Parses a line, and print errors. */
void parseLine(Line* line, char *lineStr, int lineNum, int *IC, int *DC)
{


    char *startOfNext = lineStr;

    line->lineNum = lineNum;
    line->address = FIRST_ADDRESS + *IC;
    line->originalString = allocString(lineStr);
    line->str = line->originalString;
    line->isError = FALSE;
    line->label = NULL;
    line->cmdString = NULL;
    line->cmd = NULL;

    if(!line->originalString)
    {
        printError(lineNum, "malloc failed - Not enough memory.");
        return;
    }

    /* Check if the line is comment */
    if(isCommentOrEmpty(line))
        return;
    
    /* Find label and add it to the label array. */
    startOfNext = findLabel(line, *IC);
    if(line->isError)
        return;

    /* Update the line if startOfNext isn't NULL */
    if(startOfNext)
        line->str = startOfNext;
    
    /* Find the command token */
    line->cmdString = getFirstTok(line->str, &startOfNext);
    line->str = startOfNext;

    /* Parse the command / directive */
    if(isDirective(line->cmdString))
    {
        line->cmdString++; /* Remove the '.' from the command */
        parseDirective(line, IC, DC);
    }
    else
        parseCommand(line, IC, DC);
    
    if(line->isError)
        return;
}
