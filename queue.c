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

/*
    Free all storage used by queue
    if q is NULL, then return
*/
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    /* Free queue structure */
    list_ele_t *cur = q->head;
    while (cur != NULL) {
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
    if (q == NULL)
        return false;
    list_ele_t *newh = NULL;

    /* check call to malloc returns NULL */
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    bzero(newh, sizeof(list_ele_t));
    newh->value = strdup(s);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }

    if (!(q->tail))
        q->tail = newh;
    newh->next = q->head;
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
    if (newt == NULL)
        return false;
    bzero(newt, sizeof(list_ele_t));
    newt->value = strdup(s);
    if (newt->value == NULL) {
        free(newt);
        return false;
    }

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
    if (q == NULL || q->count == 0)
        return false;

    list_ele_t *target = q->head;

    if (sp) {
        size_t slen = strnlen(target->value, bufsize);
        if (slen == bufsize)
            slen--;
        bzero(sp, bufsize);
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
    if (q == NULL)
        return 0;

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
list_ele_t *merge_combine(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t tmp_ele, *cur = &tmp_ele;

    while (cur != NULL) {
        if (l1 == NULL) {
            cur->next = l2;
            break;
        } else if (l2 == NULL) {
            cur->next = l1;
            break;
        } else {
            list_ele_t **tar = ((strcmp(l1->value, l2->value) < 1) ? &l1 : &l2);
            cur->next = (*tar);
            *tar = (*tar)->next;
            cur = cur->next;
            cur->next = NULL;
        }
    }
    return tmp_ele.next;
}
/*
    the main sorting algorithm
*/
list_ele_t *merge_sort(list_ele_t *start, size_t len)
{
    if (len == 0 || len == 1)
        return start;

    size_t l1_len = len / 2, l2_len = len - l1_len;
    list_ele_t *l1 = start, *l2 = start;

    for (int i = 0; i < l1_len; ++i) {
        if (i != l1_len - 1) {
            l2 = l2->next;
        } else {
            list_ele_t *tmp = l2;
            l2 = l2->next;
            tmp->next = NULL;
        }
    }

    l1 = merge_sort(l1, l1_len);
    l2 = merge_sort(l2, l2_len);
    return merge_combine(l1, l2);
}

void q_sort(queue_t *q)
{
    if (q == NULL || q->count == 0)
        return;

    q->head = merge_sort(q->head, q->count);
    q->tail = q->head;
    while (q->tail->next != NULL)
        q->tail = q->tail->next;
}
