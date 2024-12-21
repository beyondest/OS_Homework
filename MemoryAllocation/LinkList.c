#include "LinkList.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
/*
Init a LinkList of rest space which has a head node
*/
struct rest* InitRestLinkList(void)
{
	struct rest* head;
	struct rest* p1;
	p1 = (struct rest*)malloc(O2);
	if (!p1) exit(1);
	p1->size = SUM;
	p1->ad = FirstAddress;
	p1->next = NULL;
	head = p1;
	return (head);
}
/*
Init a LinkList of rest space which NOT has a head node
*/
struct use* InitUseLinkList(void)
{
	struct use* head;
	head = NULL;
	return(head);
}
struct use* FreeUseLinkList(struct use* usehead)
{
	struct use* p1, * p2;
	p1 = usehead;
	while (p1 != NULL)
	{
		p2 = p1->next;
		free(p1);
		p1 = p2;
	}
	return (NULL);
}
struct rest* FreeRestLinkList(struct rest* resthead)
{
	struct rest* p1, * p2;
	p1 = resthead;
	while (p1 != NULL)
	{
		p2 = p1->next;
		free(p1);
		p1 = p2;
	}
	return (NULL);
}		
/*
Link pjob to the last one in use linklist
*/
struct use* AddJob2UseLlist(struct use* usehead, struct job* pjob)
{
	struct use* p0, * p2;
	p0 = (struct use*)malloc(O3);
	if (!p0) exit(1);
	p0->name = pjob->name;
	p0->size = pjob->size;
	p0->ad = pjob->ad;
	if (usehead == NULL)
	{
		p0->next = NULL;
		usehead = p0;
	}
	else
	{
		p2 = usehead;
		while (p2->next != NULL)
		{
			p2 = p2->next;
		}
		p2->next = p0;
		p0->next = NULL;
	}
	return(usehead);
}
/*
Delete job from use linklist
*/
struct use* DelJobFromUseLlist(struct use* usehead, struct job* p)
{
	struct job* pjob;
	struct use* cur, * pre;
	cur = usehead;
	pjob = p;
	pre = NULL;
	if (usehead == NULL)return (NULL);
	while ((pjob->name != cur->name) && (cur->next != NULL))
	{
		pre = cur;
		cur = cur->next;
	}
	if (pjob->name == cur->name)
	{
		if (cur == usehead)
			usehead = cur->next;
		else
			pre->next = cur->next;
		free(cur);
	}
	return (usehead);
}
/*
Update rest linklist according to the location of job removed
*/
struct rest* RecycleRestLlist(struct job* p, struct rest* resthead)
{
	struct rest* cur, * pre, * pjobNew;
	struct job* pjob;
	pjob = p;
	cur = resthead;
	pre = NULL;
	pjobNew = (struct rest*)malloc(O2);
	if (!pjobNew) exit(1);
	pjobNew->size = pjob->size;
	pjobNew->ad = pjob->ad;
	if (!resthead)
	{
		pjobNew->next = NULL;
		resthead = pjobNew;
		return(resthead);
	}

	while ((cur->ad < pjob->ad) && (cur->next != NULL))
	{
		pre = cur;
		cur = cur->next;
	}
	// exits rest blocks after pjob
	if (cur->ad > pjob->ad)
	{
		//no rest block before pjob
		if (resthead == cur)
		{
			if ((cur->ad - pjobNew->ad) == (pjobNew->size)) // job is directly before cur rest block 
			{
				cur->ad = pjobNew->ad;
				cur->size = pjobNew->size + cur->size;
				free(pjobNew);
			}
			else // job is in front of cur but not directly
			{
				resthead = pjobNew;
				pjobNew->next = cur;
			}
		}
		//exists rest block before pjob
		else
		{
			if ((cur->ad - pjobNew->ad) == (pjobNew->size)) // job is directly before cur rest block
			{
				cur->ad = pjobNew->ad;
				cur->size = pjobNew->size + cur->size;
				if ((pjobNew->ad - pre->ad) == (pre->size))// job is directly behind pre rest block
				{
					pre->size = cur->size + pre->size;
					pre->next = cur->next;
					free(cur);
				}
				else // job is behind pre but not directly
				{
					pre->next = cur;
				}
				free(pjobNew);
			}
			else // job is in front of cur but not directly
			{
				if ((pjobNew->ad - pre->ad) == (pre->size)) // job is directly behind pre rest block
				{
					pre->size = pre->size + pjobNew->size;
					free(pjobNew);
				}
				else
				{
					pjobNew->next = cur;
					pre->next = pjobNew;
				}
			}
		}
	}
	// pjob is the last one in memory
	else
	{
		//pjob is directly behind cur rest block
		if ((pjobNew->ad - cur->ad) == (cur->size))
		{
			cur->size = cur->size + pjobNew->size;
			free(pjobNew);
		}
		//pjob is behind cur but not directly
		else
		{
			cur->next = pjobNew;
			pjobNew->next = NULL;
		}
	}
	return(resthead);
}

