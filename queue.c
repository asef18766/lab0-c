#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* return NULL on malloc failure */
    if (q == NULL)
        return NULL;

    bzero(q, sizeof(queue_t));
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    list_ele_t *cur = q->head;
    while (cur) {
        list_ele_t *tar = cur;
        cur = cur->next;
        /* freeing the the string */
        free(tar->value);
        /* freeing the list element */
        free(tar);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh = NULL;

    /* check call to malloc returns NULL */
    newh = malloc(sizeof(list_ele_t));
    bzero(newh, sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    if (!(q->tail))
        q->tail = newh;
    newh->next = q->head;
    newh->value = strdup(s);
    q->head = newh;

    q->count++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL)
        return false;
    list_ele_t *newt = NULL;
    newt = malloc(sizeof(list_ele_t));
    bzero(newt, sizeof(list_ele_t));
    if (newt == NULL)
        return false;
    newt->value = strdup(s);

    if (!(q->head))
        q->head = newt;
    else
        q->tail->next = newt;

    q->tail = newt;
    q->count++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *target = q->head;
    if (target == NULL || target->value == NULL)
        return false;
    if (sp) {
        size_t slen = strnlen(target->value, bufsize);
        strncpy(sp, target->value, slen);
    }
    free(target->value);
    target = target->next;
    free(q->head);
    q->head = target;
    q->count--;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q->count;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->count == 0)
        return;
    list_ele_t *cur = q->head, *next = q->head->next;
    while (next != NULL) {
        list_ele_t *nn = next->next;
        // printf("cuurent at %s\n", cur->value);
        next->next = cur;
        cur = next;
        next = nn;
    }
    // do a swap for head & tail
    q->head = (list_ele_t *) ((u_int64_t)(q->head) ^ (u_int64_t)(q->tail));
    q->tail = (list_ele_t *) ((u_int64_t)(q->head) ^ (u_int64_t)(q->tail));
    q->head = (list_ele_t *) ((u_int64_t)(q->head) ^ (u_int64_t)(q->tail));

    q->tail->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}
