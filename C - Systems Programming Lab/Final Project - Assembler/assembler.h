/*	 
    * General header file for the assembly.
    * Contains Definitions, Macros, Data structures and method declaration.
*/
#ifndef ASSEMBLER_H
    #define ASSEMBLER_H

/* Includes */
#include <stdio.h>
#include <string.h>

/* Macros */
#define FOREVER             while(TRUE)

/* =========== Constatns =========== */

#define FIRST_ADDRESS               100 
#define BYTE_SIZE			        8
#define MEMORY_WORD_LENGTH          24

/* Extensions */
#define ASM_EXTENSION               ".as"
#define EXTERN_EXTENSION            ".ext";
#define OBJECT_EXTENSION            ".obj";

/* Invalid */
#define INVALID_LINE_NUM            -1
#define INVALID_CMD_INDEX           -1

/* Limits */
#define MAX_DIRECTIVES              4 
#define MAX_REGISTER_DIGIT          7
#define MAX_LABEL_LENGTH            30
#define MAX_LINE_LENGTH             80
#define MAX_FILE_LINES              700
#define MAX_LABELS_NUM              MAX_FILE_LINES
#define MAX_DATA_NUM                1000 /* 2^21? */

/* Important op codes */
#define LEA_OPCODE                  4
#define CMP_OPCODE                  1
#define PRN_OPCODE                  13

/* =========== Types =========== */

/* Boolean */
typedef enum 
{
    FALSE = 0,
    TRUE = 1
} Boolean;

/* Label Flag */
typedef enum 
{
    NONE = 0,
    EXTERN = 1,
    DATA = 2
} LabelFlag;

/* Operand */
typedef enum {

    INVALID = -1,
    LABEL = 0,
    NUMBER = 1,
    REGISTER = 2
} OperandType;

/* ARE */
typedef enum 
{
    ABSOLUTE = 0,
    EXTERNAL = 1,
    RELOCATABLE = 2 
} AreType;

/* =========== Data Structures =========== */

/* Label */
typedef struct 
{
    int address;                     /* The address its contains. */   
    char name[MAX_LABEL_LENGTH];     /* The name of the label. */
    LabelFlag flag;                  /* The flag (.data or .string) */
} Label;

/* Command */
typedef struct 
{
    char* name;
    int funct;
    unsigned int opCode : 6;
    int params;
} Command;

/* Directive */
typedef struct 
{
    char* name;
    void (*callback)();
} Directive;

/* Operand */
typedef struct
{
    int value;
    char* str;
    OperandType type;
    int address;
} Operand;

/* Line */
typedef struct 
{
    int lineNum;
    int address;

    char *originalString;
    char *str;

    Boolean isError;
    Label *label;

    char* cmdString;
    const Command *cmd;
    Operand op1, op2;
} Line;

/* Memory Word */
typedef struct 
{
    unsigned are : 3;

    union 
    {
        struct 
        {
            unsigned int opcode : 6;
            unsigned int src : 2;
            unsigned int dest : 2;
            unsigned int funct : 5;

        } cmdBits;

        struct 
        {
            unsigned int srcBits : 3;
            unsigned int destBits : 3;
        } regBits;

        /* For address only */
		int value : 22;

    } valueBits;
} MemoryWord;

/* =========== Methods Declaration =========== */

/* main.c Methods */
void parseFile(char* fileName);

/* first_read.c Methods */
int firstFileRead(FILE* file, Line* lines, int* linesFound, int* IC, int* DC);
Boolean readLine(FILE* file, char* buf, size_t maxLength);
void parseLine(Line* line, char* lineStr, int lineNum, int* IC, int* DC);
char *findLabel(Line* line, int IC);
Label *appendLabel(Label label, Line* line);
void parseDirective(Line* line, int* IC, int* DC);
void parseCommand(Line* line, int* IC, int* DC);
void parseCmdOperands(Line* line, int* IC, int* DC);
void parseOpInfo(Operand* operand, int lineNum);
Boolean checkValidOpTypes(const Command* cmd, Operand op1, Operand op2, int lineNum);
Boolean addNumberToData(int num, int* IC, int* DC, int lineNum);
Boolean addStringToData(char *str, int* IC, int* DC, int lineNum);
void removeLastLabel(int lineNum);

/* second_read.c Methods */
void updateDataLabelsAddress(int IC);
int countInvalidEntries();
int secondFileRead(int* memoryArr, Line* lines, int lineNum, int IC, int DC);
Boolean updateLableOpAddress(Operand* op, int lineNum);
Boolean addLineToMemory(int* memoryArr, int* memoryCounter, Line* line);
int getNumFromMemoryWord(MemoryWord memory);
int getOpTypeId(Operand op);
MemoryWord getCmdMemoryWord(Line line);
MemoryWord getOpMemoryWord(Operand op, Boolean isDest);
void addWordToMemory(int* memoryArr, int* memoryCounter, MemoryWord memory);
void addDataToMemory(int* memoryArr, int* memoryCounter, int DC);

/* utils.c Methods */
void printError(int lineNum, const char* format, ...);
int randomInRange(int min, int max);
FILE* openFile(char* name, char* ext, const char* mode);
char *allocString(const char *str);
Boolean isCommentOrEmpty(Line* line);
void trimStr(char** ptStr);
void trimLeftStr(char** ptStr);
Boolean isOneWord(char* str);
Boolean isWhiteSpaces(char* str);
Boolean isValidLabel(char* str, int lineNum, Boolean printErrors);
Boolean isValidRegister(char* str, int* value);
int indexOfCommand(char* cmd);
Label* getLabel(char* label);
Boolean isExistingEntryLabel(char* label);
Boolean isLabelExists(char *label);
char* getFirstTok(char* str, char** endOfTok);
Boolean isDirective(char* cmd);
char* getFirstOperand(char* line, char** endOfOp, Boolean* foundComma);
Boolean isValidNumberParam(char* numStr, int numOfBits, int lineNum, int* value);
Boolean isValidStringParam(char** strParam, int lineNum);

/* exports.c Methods */
void createObjectFile(char* name, int IC, int DC, int *memoryArr);
void createEntriesFile(char* name);
void createExternFile(char* name, Line* lines, int linesFound);

#endif
