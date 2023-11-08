/********************************** BST **********************************
 * source file for BST that contains a queue and holds a string. Designed 
 * to be written to a file.
 *
 **************************************************************************/

#include <stdlib.h>
#include "BST.h"


/****************** writeTreeInorder *******************
 *  writes the tree's contents to the passed file 
 *  inorder
 *		PRE:  root - the tree to write
 *			  fp   - the file to write to
 *		POST: none
 *******************************************************/
void writeTreeInorder(T_NODE *root, FILE *fp)
{
	if (root) {
		writeTreeInorder(root->left, fp);
		fprintf(fp, "%-27s ", root->data);
		writeQueue(root->queue, fp);
		writeTreeInorder(root->right, fp);
	}
	return;
}

/********************* insert **************************
 * inserts the new node into the tree
 *		PRE:  root - the tree
 *			  data - string data
 *			  line - line number
 *		POST: 1 - if insertion is successful
 *			  0 - if duplicate
 *******************************************************/
int insert(T_NODE **root, char *data, int line)
{
	T_NODE **ptr_root = root;

	while (*ptr_root) {
		if(strcmp(data, (*ptr_root)->data) > 0)
			ptr_root = &(*ptr_root)->right;
		else if (strcmp(data, (*ptr_root)->data) < 0)
			ptr_root = &(*ptr_root)->left;
		else
		{
			enqueue(&((*ptr_root)->queue), &((*ptr_root)->rear), line);
			return 0; // duplicate
		}
	}
	if (!(*ptr_root = (T_NODE *)malloc(sizeof(T_NODE))))
		printf("Fatal malloc error!\n"), exit(1);
	
	if (!((*ptr_root)->data = (char *)calloc(strlen(data) + 1, sizeof(char))))
		printf("Fatal calloc error!\n"), exit(2);

	strcpy((*ptr_root)->data, data);
	(*ptr_root)->left = (*ptr_root)->right = NULL;
	
	(*ptr_root)->queue = NULL;
	(*ptr_root)->rear = NULL;

	enqueue(&((*ptr_root)->queue), &((*ptr_root)->rear), line);

	return 1; // data inserted
}