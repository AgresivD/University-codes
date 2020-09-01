/* Includes */
#include "assembler.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

/* =========== Externs =========== */
extern const Command Commands[];

/* Labels */
extern Label labelsArr[MAX_FILE_LINES];
extern int labels_Count;

/* Entry Lines */
extern Line* linesArr[MAX_FILE_LINES];
extern int lines_Count;

/* Print an error with the line number. */
void printError(int lineNum, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\n\033[1;31m[Error]:\033[0m ");

    if(lineNum != INVALID_LINE_NUM)
        printf("At line %d: ", lineNum);

    vprintf(format, args);
    printf("\n");

    va_end(args);
}

/* Generates a random int between 'min' and 'max'. */
int randomInRange(int min, int max)
{
    if(max <= min)
        return min;
    return (rand() % (max - min + 1)) + min;
}

/* Opens the file whose name is specified in the parameter fileName.

    Returned stream that can be identified in future operations by the FILE pointer. */
FILE* openFile(char* name, char* ext, const char* mode)
{
	FILE *file;
	char *mallocStr = (char *)malloc(strlen(name) + strlen(ext) + 1), *fileName = mallocStr;
	sprintf(fileName, "%s%s", name, ext);

	file = fopen(fileName, mode);
	free(mallocStr);

	return file;
}

/* Returns the same string in a different part of the memory. */
char *allocString(const char *str)
{
    char* newString = (char*)malloc(strlen(str) + 1);
    if(newString)
        strcpy(newString, str);
    return newString;
}

/* Return a bool, represent whether 'line' is a comment or not. 

    If the first char is ';' but it's not at the start of the line, it returns true and update line->isError to be TRUE. */
Boolean isCommentOrEmpty(Line* line)
{
    char* firstWord = line->str;

    if(*line->str == ';') /* Comment */
        return TRUE;
    
    trimLeftStr(&firstWord);

    if(*firstWord == '\0') /* Empty line */
        return TRUE;
    
    if(*firstWord == ';')
    {
        /* Illegal comment - ';' isn't at the start of the line */
        printError(line->lineNum, "Comments must start with ';' at the start of the line.");
        line->isError = TRUE;
        return TRUE;
    }
    return FALSE;
}

/* Removes all the spaces from the edges of the string ptStr is pointing to. */
void trimStr(char** ptStr)
{
    char* eos;

    /* Return if it's NULL or empty string */
    if(!ptStr || **ptStr == '\0')
        return;
    
    trimLeftStr(ptStr);

    /* eos is pointing to the last char in str, before '\0' */
    eos = *ptStr + strlen(*ptStr) - 1;

    /* Remove spaces from the end */
    while(isspace(*eos) && eos != *ptStr)
        *eos-- = '\0';
}

/* Removes spaces from start. */
void trimLeftStr(char **ptStr)
{
    /* Return if it's NULL */
    if(!ptStr)
        return;
    
    /* Get ptStr to the start of the actual text */
    while(isspace(**ptStr))
        ++*ptStr;
}

/* Returns TRUE if str contains only one word. */
Boolean isOneWord(char *str)
{
    trimLeftStr(&str);
    while(!isspace(*str) && *str) 
        str++;
    return isWhiteSpaces(str);
}

/* Returns TRUE if str contains only white space. */
Boolean isWhiteSpaces(char *str)
{
    while(*str)
    {
        if(!isspace(*str++))
            return FALSE; 
    }
    return TRUE;
}

/* Returns TRUE if str is a legal label name. */
Boolean isValidLabel(char *str, int lineNum, Boolean printErrors)
{
    int labelLength, i;
    labelLength = strlen(str);

    /* Check if the label is short enough */
    if(labelLength > MAX_LABEL_LENGTH)
    {
        if(printErrors)
            printError(lineNum, "Label is too long. Max label name length is %d", MAX_LABEL_LENGTH);       
        return FALSE;
    }

    /* Check if the label isn't an empty string */
    if(*str == '\0')
    {
        if(printErrors)
            printError(lineNum, "Label name is empty.");      
        return FALSE;
    }

    /* Check if the label start of the line */
    if(isspace(*str))
    {
        if(printErrors)
            printError(lineNum, "Label must start at the start of the line.");
        return FALSE;
    }

    /* Check if it's chars only */
    for(i = 1; i < labelLength; i++)
    {
        if(!isalnum(str[i]))
        {
            if(printErrors)
                printError(lineNum, "\"%s\" is not valid label - use letters and numbers only.", str);    
            return FALSE;
        }
    }

    /* Check if the first char is latter. */
    if(!isalpha(*str) && *str != '&')
    {
        if(printErrors)
            printError(lineNum, "\"%s\" is not valid label - first char must be a letter.", str);
    }

    /* Check if it's not a name of register */
    if(isValidRegister(str, NULL)) /* NULL since we don't have to save the register number */
    {
        if(printErrors)
            printError(lineNum, "\"%s\" is not valid label - don't use a name of a register.", str);
        return FALSE; 
    }

    /* Check if it's not a name of a command */
    if(indexOfCommand(str) != INVALID_CMD_INDEX)
    {
        if(printErrors)
            printError(lineNum, "\"%s\" is not valid label - don't use a name of a command.", str);
        return FALSE;
    }
    
    return TRUE;
}
/* Returns TRUE if str is a register name and update value to be the register value. */
Boolean isValidRegister(char* str, int* value)
{
    if(str[0] == 'r' && str[1] >= '0' && str[1] - '0' <= MAX_REGISTER_DIGIT && str[2] == '\0')
    {
        /* Update value if it's not NULL */
        if(value)
            *value = str[1] - '0'; /* -'0' To get the actual number the char represents */    
        return TRUE;
    }
    return FALSE;
}

/* Returns the ID of the command with 'name' in Commands or -1 if there isn't such command. */
int indexOfCommand(char* cmd)
{
    int i = 0;
    while(Commands[i].name)
    {
        if(strcmp(cmd, Commands[i].name) == 0)
            return i;
        i++;
    }
    return INVALID_CMD_INDEX;
}

/* Returns a pointer to the label contains 'label' name in labelsArr or NULL if there isn't such label. */ 
Label* getLabel(char* label)
{
    int i;
    if(label)
    {
        if(label[0] == '&')
            label++;
        for(i = 0; i < labels_Count; i++)
            if(strcmp(label, labelsArr[i].name) == 0)
                return &labelsArr[i];
    }
    return NULL;
}

Boolean isLabelExists(char *label)
{
    return getLabel(label) != NULL;
}

/* Returns TRUE if the label is already in the entry lines array. */
Boolean isExistingEntryLabel(char* label)
{
    int i;
    if(label)
    {
        for(i = 0; i < lines_Count; i++)
            if(strcmp(label, linesArr[i]->str) == 0)
                return TRUE;
    }
    return FALSE;
}

/* Returns a pointer to the start of first token. 
    
    Also makes *endOfTok (if it's not NULL) to point at the last char after the token. */
char* getFirstTok(char* str, char** endOfTok)
{
    char* tokStart = str;
    char* tokEnd = NULL;

    /* Trim the start */
    trimLeftStr(&tokStart);

    /* Find the end of the first word */
    tokEnd = tokStart;
    while(*tokEnd != '\0' && !isspace(*tokEnd))
        tokEnd++;
    
    /* Add \0 at the end if needed */
    if(*tokEnd != '\0')
    {
        *tokEnd = '\0';
        tokEnd++;
    }

    /* Make *endOfTok (if it's not NULL) to point at the last char after the token */
    if(endOfTok)
        *endOfTok = tokEnd;
    
    return tokStart;
}

/* Returns TRUE if cmd is a directive. */
Boolean isDirective(char *cmd)
{
    return (*cmd == '.' ? TRUE : FALSE);
}

/* Returns a pointer to the start of the first operand in 'line' and change the end of it to '\0'. 

    Also makes *endOfOp (if it's not NULL) point at the next char after the operand. */
char* getFirstOperand(char* line, char** endOfOp, Boolean* foundComma)
{
    if(!isWhiteSpaces(line))
    {
        /* Find the first comma */
        char* end = strchr(line, ',');
        if(end)
        {
            *foundComma = TRUE;
            *end = '\0';
            end++;
        }
        else
            *foundComma = FALSE;

        /* Set endOfOp (if it's not NULL) to point at the next char after the operand
            
            (Or at the end of it if it's the end of the line). */
        if(endOfOp)
            *endOfOp = (end) ? end : strchr(line, '\0');
    }
    
    trimStr(&line);
    return line;
}

/* Returns TRUE if the num is a valid number param, and save it's value in *value. */
Boolean isValidNumberParam(char* numStr, int numOfBits, int lineNum, int* value)
{
    char* endOfNum;
    /* maxNum is the max number you can represent with (MAX_LABEL_LENGTH - 1) bits 
	
        (-1 for the negative/positive bit) */
    int maxNum = (1 << numOfBits) - 1;

    if(isWhiteSpaces(numStr))
    {
        printError(lineNum, "Empty parameter.");
        return FALSE;
    }

    *value = strtol(numStr, &endOfNum, 0);

    /* Check if endOfNum is at the end of the string */
    if(*endOfNum)
    {
        printError(lineNum, "\"%s\" isn't a valid number.", numStr);
        return FALSE;
    }

    /* Check if the number is small enough to fit into 1 memory word 
	    
        (if the absolute value of number is smaller than 'maxNum' */
    if(*value > maxNum || *value < -maxNum)
    {
        printError(lineNum, "\"%s\" is too %s, must be between %d and %d.", numStr, (*value > 0) ? "big" : "small", -maxNum, maxNum);
        return FALSE;
    }
    
    return TRUE;
}

/* Returns TRUE if the strParam is a valid string param (enclosed in quotes), and remove the quotes. */
Boolean isValidStringParam(char** strParam, int lineNum)
{
    /* Check if the string param is enclosed in quotes */
    if((*strParam)[0] == '"' && (*strParam)[strlen(*strParam) - 1] == '"')
    {
        /* Remove the quotes */
        (*strParam)[strlen(*strParam) - 1] = '\0';
        ++*strParam;
        return TRUE;
    }
    printError(lineNum, (**strParam == '\0') ? "No Parameter" : "The parameter for .string must be enclosed in quotes.");
    return FALSE;
}
