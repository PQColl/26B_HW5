/******************************* Queue ***********************************
* source file for a linked queue that holds an int. Allows enqueuing,
* dequeuing and printing of the queue.
*
**************************************************************************/

#include <stdlib.h>
#include "Queue.h"

/********************** enqueue ***************************
 * adds a new node to the end of the queue
 *		PRE:  queue - the front of the queue
 *			  rear  - the end of the queue
 *			  data  - the data of the new node
 *		POST: none
 **********************************************************/
void enqueue(Q_NODE **queue, Q_NODE **rear, int data)
{
	Q_NODE *pnew;

	if (*rear && (*rear)->data == data)
		return;

	pnew = (Q_NODE *)malloc(sizeof(Q_NODE));
	if (!pnew)
	{
		printf("... error in enqueue!\n");
		exit(1);
	}

	
	pnew->data = data;
	pnew->next = NULL;
	if (*queue == NULL) *queue = pnew;
	else (*rear)->next = pnew;
	*rear = pnew;
	return;
}

/********************** dequeue ***************************
* removes the node from the front of the queue
*		PRE:  queue - the front of the queue
*			  rear  - the end of the queue
*		POST: first - the front node of the queue
**********************************************************/
Q_NODE *dequeue(Q_NODE **queue, Q_NODE **rear)
{
	Q_NODE *first;

	if (*queue == NULL) return NULL;
	first = *queue;
	*queue = (*queue)->next;
	if (*queue == NULL) *rear = NULL;
	first->next = NULL;

	return first;
}

/********************* writeQueue ************************
* writes the contents of the queue to the file
*		PRE:  queue - the front of the queue
*			  fp    - the file to write to
*		POST: none
**********************************************************/
void writeQueue(Q_NODE *queue, FILE *fp)
{
	Q_NODE *walker = queue;
	int count = 0;
	while (walker)
	{

		if (count > 0 && count % 10 == 0)
			fprintf(fp, "\n%28s", " ");
		fprintf(fp, "%5d ", walker->data);\
		walker = walker->next;
		
		count++;
	}
	putc('\n', fp);
}