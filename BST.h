#ifndef BST_H_
#define BST_H_
#include <stdio.h>
#include "Queue.h"

typedef struct nodeTag {
	char *data;
	Q_NODE *queue;
	Q_NODE *rear;
	struct nodeTag *left;
	struct nodeTag *right;
} T_NODE;

void writeTreeInorder(T_NODE *, FILE *);
int insert(T_NODE **, char *, int);

#endif