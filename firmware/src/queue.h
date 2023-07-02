#pragma once

/**
 * Pure C queue implemented by ChatGPT
 */

#include <stdbool.h>
#include <stdint.h>

typedef struct NODE
{
    uint32_t data;
    struct NODE *next;
} node_t;

typedef struct
{
    node_t *front;
    node_t *rear;
} queue_t;

queue_t *queue_init();
void enqueue(queue_t *q, uint32_t data);
void dequeue(queue_t *q, uint32_t *ret);
void queue_clear(queue_t *q);
bool queue_empty(queue_t* q);