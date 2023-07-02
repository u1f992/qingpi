#include "queue.h"

#include <stdlib.h>

static node_t *create_node(uint32_t data)
{
    node_t *temp = (node_t *)malloc(sizeof(node_t));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

queue_t *queue_init()
{
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(queue_t *q, uint32_t data)
{
    node_t *temp = create_node(data);

    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

void dequeue(queue_t *q, uint32_t *ret)
{
    if (q->front == NULL)
    {
        ret = NULL;
        return;
    }

    node_t *temp = q->front;
    *ret = temp->data;
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
}

void queue_clear(queue_t *q)
{
    uint32_t ret;
    while (q->front != NULL)
        dequeue(q, &ret);
}

bool queue_empty(queue_t* q) {
    return (q->front == NULL);
}