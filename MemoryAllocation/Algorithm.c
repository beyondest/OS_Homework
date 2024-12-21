#include "Algorithm.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

/*
First Fit Allocation Algorithm
@param resthead: the head node of rest linklist
@param pjob: the job to allocate
@return: the updated rest linklist
*/
struct rest* FFAllocate(struct rest* resthead, struct job* pjob)
{
    struct rest* cur, * pre;
    cur = resthead;
    pre = NULL;
    // rest linklist is empty
    if (!resthead)
    {
        pjob->ad = 0;
        return NULL;
    }
    while (cur && cur->size < pjob->size)
    {
        pre = cur;
        cur = cur->next;
    }
    // found a rest block which is large enough to allocate
    if (cur && cur->size >= pjob->size)
    {
        int marginSize = cur->size - pjob->size;
        pjob->ad = cur->ad;
        // split the rest block into two parts
        if (marginSize > 0)
        {
            cur->ad += pjob->size;
            cur->size = marginSize;
            return resthead;
        }
        else
        {
            if (!pre)
            {
                resthead = cur->next;
            }
            else
            {
                pre->next = cur->next;
            }
            free(cur);
            return resthead;
        }
    }
    // not found a rest block which is large enough to allocate
    else
    {
        pjob->ad = 0;
        return resthead;
    }
}

/*
Best Fit Allocation Algorithm
@param resthead: the head node of rest linklist
@param pjob: the job to allocate
@return: the updated rest linklist
*/
struct rest* BFAllocate(struct rest* resthead, struct job* pjob)
{
    struct rest* cur, * pre;
    cur = resthead;
    pre = NULL;
    int bestFitSize = SUM + 1;
    struct rest* bestFit = NULL;
    struct rest* bestFitpre = NULL;
    if (!resthead)
    {
        pjob->ad = 0;
        return NULL;
    }
    while (cur)
    {
        // find the best fit rest block
        if (cur->size >= pjob->size && cur->size < bestFitSize)
        {
            bestFitSize = cur->size;
            bestFit = cur;
            bestFitpre = pre;
        }
        pre = cur;
        cur = cur->next;
    }
    // found a rest block which is large enough to allocate
    if (bestFit)
    {
        int marginSize = bestFit->size - pjob->size;
        pjob->ad = bestFit->ad;
        // split the rest block into two parts
        if (marginSize > 0)
        {
            bestFit->ad += pjob->size;
            bestFit->size = marginSize;
            return resthead;
        }
        else
        {
            if (!bestFitpre)
            {
                resthead = bestFit->next;
            }
            else
            {
                bestFitpre->next = bestFit->next;
            }
            free(bestFit);
            return resthead;
        }
    }
    // not found a rest block which is large enough to allocate
    else
    {
        pjob->ad = 0;
        return resthead;
    }
}

/*
Worst Fit Allocation Algorithm
@param resthead: the head node of rest linklist
@param pjob: the job to allocate
@return: the updated rest linklist
*/
struct rest* WFAllocate(struct rest* resthead, struct job* pjob)
{
    struct rest* cur, * pre;
    cur = resthead;
    pre = NULL;
    int worstFitSize = 0;
    struct rest* worstFit = NULL;
    struct rest* worstFitpre = NULL;
    if (!resthead)
    {
        pjob->ad = 0;
        return NULL;
    }
    while (cur)
    {
        // find the worst fit rest block
        if (cur->size >= pjob->size && cur->size > worstFitSize)
        {
            worstFitSize = cur->size;
            worstFit = cur;
            worstFitpre = pre;
        }
        pre = cur;
        cur = cur->next;
    }
    // found a rest block which is large enough to allocate
    if (worstFit)
    {
        int marginSize = worstFit->size - pjob->size;
        pjob->ad = worstFit->ad;
        // split the rest block into two parts
        if (marginSize > 0)
        {
            worstFit->ad += pjob->size;
            worstFit->size = marginSize;
            return resthead;
        }
        else
        {
            if (!worstFitpre)
            {
                resthead = worstFit->next;
            }
            else
            {
                worstFitpre->next = worstFit->next;
            }
            free(worstFit);
            return resthead;
        }
    }
    // not found a rest block which is large enough to allocate
    else
    {
        pjob->ad = 0;
        return resthead;
    }
}
