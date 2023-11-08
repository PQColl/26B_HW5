/************************************** 26B_HW5 *******************************************
 * This program uses a Binary Search Tree and Queues to process the identifiers in a
 * .c file. Each keyword is stored in its own node, and a queue of line numbers it appears
 * on is associated with each node.
 *		By: Patrick Collins
 *		IDE: Visual Studio
 *
 *******************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "Queue.h"
#include "BST.h"
#ifdef _MSC_VER
#include <crtdbg.h>  // needed to check for memory leaks (Windows only!)
#endif

#define DELIM "\040\t\n^.,!?[]{};:()->+=#&%"
#define WRITE "w"
#define READ "r"

// bit field for flags to track line parsing
struct LINE_FLAGS
{
	unsigned int dq_flag : 1;	// double quote flag
	unsigned int sq_flag : 1;	// single quote flag
	unsigned int lc_flag : 1;	// single line comment flag
	unsigned int bc_flag : 1;	// block comment flag
	unsigned int in_flag : 1;	// include <> flag
	unsigned int chg_flag : 1;	// change flag
};

FILE *openFile(char *filename, char *mode);
T_NODE *freeTree(T_NODE *root);
T_NODE *buildTreeFromFile(T_NODE *root, FILE *fpIn);
void parseLine(char *line, T_NODE **root, int lineNum);
void writeHeader(FILE *fp);
void copyFile(FILE *fpIn, FILE *fpOut);

int main(int argc, char *argv[])
{
	T_NODE *root = NULL;

	if (argc == 3)
	{
		printf("Opening %s to read...\n", argv[1]);
		FILE *fpIn = openFile(argv[1], READ);
		printf("Opening %s to write...\n", argv[2]);
		FILE *fpOut = openFile(argv[2], WRITE);
		printf("Building tree of identifiers from %s...\n", argv[1]);
		root = buildTreeFromFile(root, fpIn);
		printf("Writing identifier tree and %s to %s...\n", argv[1], argv[2]);
		writeHeader(fpOut);
		writeTreeInorder(root, fpOut);
		copyFile(fpIn, fpOut);
		printf("Writing to %s successful! See %s for output.\n\n", argv[2], argv[2]);
		fclose(fpIn);
		fclose(fpOut);

		freeTree(root);

		#ifdef _MSC_VER
			printf(_CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
		#endif
	}
	else
		printf("Improper format.\n"
			   "26B_HW5.c <In File> <Out File>\n");
	system("pause");
	return 0;
}

/******************** writeHeader ***********************
* writes the header to the file
*		PRE:  fp - pointer to file
*		POST: none
*
********************************************************/
void writeHeader(FILE *fp)
{
	struct tm *timeptr;
	time_t timeval;
	
	char buffer[80];

	time(&timeval);
	timeptr = localtime(&timeval);

	fprintf(fp, "/************************  Cross Reference Listing  ************************/\n\n");

	fprintf(fp, "Cross reference listing made at: ");
	strftime(buffer, sizeof(buffer), "%A, %b %d, %I:%M:", timeptr);
	fprintf(fp, "%s%02d, ", buffer, timeptr->tm_sec);
	strftime(buffer, sizeof(buffer), "%Y\n", timeptr);
	fprintf(fp, "%s\n", buffer);
}

/********************** openFile ***********************
* opens the file in the proper mode
*		PRE:  filename  - name of the file to open
*			  mode		- mode to open in
*		POST: fp - the opened file
********************************************************/
FILE *openFile(char *filename, char *mode)
{
	FILE *fp = fopen(filename, mode);
	if (!fp)
	{
		(!strcmp(mode, WRITE)) ? printf("Error opening file for writing!\n"), exit(101) :
								 printf("Error opening file for reading!\n"), exit(102);
	}
	
	return fp;
}

/***************** buildTreeFromFile *******************
* copies the contents of one file to another
*		PRE:  root  - pointer to root of tree
*			  fp    - pointer to file to read
*		POST: root  - built tree
********************************************************/
T_NODE *buildTreeFromFile(T_NODE *root, FILE *fp)
{
	char line[256];
	int lineNum = 1;
	
	while (fgets(line, 255, fp))
	{
		parseLine(line, &root, lineNum);
		lineNum++;
	}
	rewind(fp);
	return root;
}

/******************** parseLine ************************
* parses the contents of the line and extracts only
* identifiers
*		PRE:  line    - the line from the file
			  root    - the tree that is being built
*			  lineNum - the current line number
*		POST: none
********************************************************/
void parseLine(char *line, T_NODE **root, int lineNum)
{
	static struct LINE_FLAGS flags;
	
	char *token = strtok(line, DELIM);

	while (token && !flags.lc_flag)
	{
		if (strstr(token, "//")) flags.lc_flag = 1;
		if (strstr(token, "<"))  flags.in_flag = 1;
		if (strstr(token, "/*")) flags.bc_flag = 1;
		if (strstr(token, "*/")) flags.bc_flag = 0, flags.chg_flag = 1;
		if (strchr(token, '\"')) flags.dq_flag = ~flags.dq_flag, flags.chg_flag = 1;
		if (strchr(token, '\'')) flags.sq_flag = ~flags.sq_flag, flags.chg_flag = 1;

		if (!flags.bc_flag && !flags.lc_flag && !flags.chg_flag &&
			!flags.dq_flag && !flags.sq_flag &&
			!flags.in_flag && !strtol(token, NULL, 10))
		{
			if (*token == '*' && strlen(token) > 1)
				insert(root, token + 1, lineNum);
			else if(isalpha(*token))
				insert(root, token, lineNum);
		}
			
		token = strtok(NULL, DELIM);
		flags.chg_flag = 0;
	}

	flags.lc_flag = 0;
	flags.in_flag = 0;
	flags.dq_flag = 0;
	flags.sq_flag = 0;	
}

/********************** copyFile ************************
 * copies the contents of one file to another
 *		PRE:  fpIn  - pointer to original file
 *			  fpOut - pointer to output file
 *		POST: none
 *
 ********************************************************/
void copyFile(FILE *fpIn, FILE *fpOut)
{
	char line[256];
	int lineNum = 1;
	while (fgets(line, 255, fpIn))
		fprintf(fpOut, "%d\t: %s",lineNum++, line);
}

/********************** freeTree ***********************
* frees the memory allocated to the tree
*		PRE:  root - pointer to the root of the tree
*		POST: NULL
********************************************************/
T_NODE *freeTree(T_NODE *root)
{
	if (root) {
		freeTree(root->left);
		freeTree(root->right);
		free(root->data);
		while (root->queue)
		{	
			free(dequeue(&(root->queue), &(root->rear)));
		}
		free(root);
	}
	return NULL;
}

/*********************************************************/
/*					Console Output:
C:\Program Files (x86)\Microsoft Visual Studio 14.0>E:

E:\>cd code\cis26b\HW5\hw5\Debug

E:\code\cis26b\HW5\hw5\Debug>hw5.exe input.c output.txt
Opening input.c to read...
Opening output.txt to write...
Building tree of identifiers from input.c...
Writing identifier tree and input.c to output.txt...
Writing to output.txt successful! See output.txt for output.

No Memory Leak
Press any key to continue . . .
*/