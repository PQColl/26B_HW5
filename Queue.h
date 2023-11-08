#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdio.h>

typedef struct node Q_NODE;
struct node
{
	int data;
	struct node *next;
};

Q_NODE *dequeue(Q_NODE **, Q_NODE **);
void enqueue(Q_NODE **, Q_NODE **, int);
void writeQueue(Q_NODE *, FILE *);

#endif