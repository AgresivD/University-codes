/*	 
    * This file manages the assembling process.
    * It uses the "two transitions" algorithm, and thenc creates the output files.
*/

/* Includes */
#include "assembler.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* =========== Global Data Structures =========== */

/* Labels */
Label labelsArr[MAX_FILE_LINES];
int labels_Count;

/* Entry Lines */
Line* linesArr[MAX_FILE_LINES];
int lines_Count;

/* Data */
int dataArr[MAX_DATA_NUM];


/* Parsing a file, and creating the output files. */
void parseFile(char *fileName)
{
    FILE* file;
    Line fileLines[MAX_FILE_LINES];
	int memoryArr[MAX_DATA_NUM] = { 0 }, IC = 0, DC = 0, numOfErrors = 0, linesFound = 0;

    /* Open File */
    file = openFile(fileName, ASM_EXTENSION, "r");
    if(file == NULL)
    {
        printError(INVALID_LINE_NUM, "Can't open the file \"%s\"", fileName);
        return;
    }

    /* First Read */
    numOfErrors += firstFileRead(file, fileLines, &linesFound, &IC, &DC);

    /* Second Read */
    numOfErrors += secondFileRead(memoryArr, fileLines, linesFound, IC, DC);

    /* Create output files */
    if(numOfErrors == 0)
    {
        /* Create all output files */

        createObjectFile(fileName, IC, DC, memoryArr);
        createExternFile(fileName, fileLines, linesFound);
        createEntriesFile(fileName);
        printf("\n[Info] Created output files for the file \"%s.as\".\n", fileName);
    }

}

int main(int argc, char* agrv[])
{
    int i;

    if(argc < 2)
    {
        printf("[Info] no file names were observed.\n");
        return 1;
    }
    
    /* initialize random seed for later use */
	srand((unsigned)time(NULL));

    for(i = 1; i < argc; i++)
    {
        parseFile(agrv[i]);
    }
    return 0;
}
