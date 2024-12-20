#include<stdio.h>
#include<stdlib.h>
//#define NULL 0
#define O1 sizeof(struct job)
#define O2 sizeof(struct rest)
#define O3 sizeof(struct use)
int SUM = 100;
int FirstAddress = 1;
char buffer[200];

struct job
{
	int name;
	int size;
	int ad;
};
struct rest
{
	int size;
	int ad;
	struct rest* next;
};
struct use
{
	int name;
	int size;
	int ad;
	struct use* next;
};

struct job* CreateJob(void)
{
	struct job* p;
	p = (struct job*)malloc(O1);
	if(!p) exit(1);
	printf("Enter name(int): ");
	if (fgets(buffer, sizeof(buffer), stdin))
	{
		sscanf_s(buffer, "%d", &p->name);
		printf("Enter size: ");
		if (fgets(buffer, sizeof(buffer), stdin))
		{
			sscanf_s(buffer, "%d", &p->size);
			return (p);
		}
	}
	return NULL;
}// 创建作业

/*
Init a LinkList of rest space which has a head node
*/
struct rest* InitRestLinkList(void)
{
	struct rest* head;
	struct rest* p1;
	p1 = (struct rest*)malloc(O2);
	if(!p1) exit(1);
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

/*
First Fit Allocation Algorithm  
@param resthead: the head node of rest linklist
@param pjob: the job to allocate
@return: the updated rest linklist
*/
struct rest* FFAllocate(struct rest* resthead, struct job* pjob, int* workAd)//首次适应算法
{
	struct rest* cur, * pre;
	cur = resthead;
	pre = NULL;
	if(!resthead) return NULL;
	while (cur->size < pjob->size && cur->next != NULL) 
	{
		pre = cur;
		cur = cur->next;
	}
	// found a rest block which is large enough to allocate
	if (cur->size >= pjob->size) 
	{
		int marginSize = cur->size - pjob->size;
		*workAd = cur->ad;
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
				free(cur);
				return NULL;
			}
			pre->next = cur->next;
			free(cur);
			return resthead;
		}
	}
	else
	{
		*workAd = 0;
		return NULL;
	}
}

struct rest* BFAllocate(struct rest* head, struct job* p)//最佳适应算法 
{
	struct rest* p0;
	struct job* a;
	a = p;
	p0 = head;
	//在此补充分配算法逻辑

	return(head);
}

struct rest* WFAllocate(struct rest* head, struct job* p)//最坏适应算法 
{
	struct rest* p0;
	struct job* a;
	a = p;
	p0 = head;
	//在此补充分配算法逻辑

	return(head);
}

/*
Link pjob to the last one in use linklist
*/
struct use* UpdateUse(struct use* usehead, struct job* pjob)
{
	struct use* p0, * p2;
	p0 = (struct use*)malloc(O3);
	if (!p0) exit(1);
	if (usehead == NULL)
	{
		p0->name = pjob->name;
		p0->size = pjob->size;
		p0->ad = SUM;
		p0->next = NULL;
		usehead = p0;
	}
	else
	{
		p0->name = pjob->name;
		p0->size = pjob->size;
		p0->ad = pjob->ad;
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
struct use* DeleteJob(struct use* usehead, struct job* p)
{
	struct job* pjob;
	struct use* cur, * pre;
	cur = usehead;
	pjob = p;
	pre = NULL;
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

struct job* GetJob(struct use* usehead)
{
	struct job* p1;
	struct use* p2;
	int num = 0;
	p1 = (struct job*)malloc(O1);
	if(!p1) exit(1);
	printf("Enter job name you want to remove: ");
	if (fgets(buffer, sizeof(buffer), stdin))
	{
		sscanf_s(buffer, "%d", &num);
	}
	p2 = usehead;
	while ((num != p2->name) && (p2->next != NULL))
	{
		p2 = p2->next;
	}
	if (num == p2->name)
	{
		p1->name = p2->name;
		p1->size = p2->size;
		p1->ad = p2->ad;
	}
	else
	{
		free(p1);
		return NULL;
	}
	return(p1);
}

/*
Update rest linklist according to the location of job removed
*/
struct rest* UpdateRest(struct job* p, struct rest* resthead)
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

void structPrint(struct rest* h1, struct use* h2)
{
	struct rest* m1;
	struct use* n1;
	m1 = h1;
	n1 = h2;
	if (m1 == NULL)
	{
		printf("Rest LinkList is empty!\n");
	}
	else
	{
		while (m1 != NULL)
		{
			printf("Rest Block Address is %d, its size is %d\n", m1->ad, m1->size);
			m1 = m1->next;
		}
	}
	if (n1 == NULL)
	{
		printf("Use LinkList is empty!\n");
	}
	else
	{
		while (n1 != NULL)
		{
			printf("Use Block Address is %d, its size is %d, its name is %d\n", n1->ad, n1->size, n1->name);
			n1 = n1->next;
		}
	}
}


void FF(void)
{
	struct rest* prest;
	struct use* puse;
	struct job* pjob, * pjob2;
	int* workAd = (int*)malloc(sizeof(int));
	if(!workAd)exit(1);
	int y = 1;
	int n = 0;
	int a = 1;
	int c = 3;
	prest = InitRestLinkList();
	puse = InitUseLinkList();
	printf("Initial situation:\n");
	structPrint(prest, puse);
	while (a == y)
	{
		printf("Enter operation id 1-init job; 2-remove job; 3-end : ");
		if (fgets(buffer, sizeof(buffer), stdin))
		{
			sscanf_s(buffer, "%d", &c);
		}
		switch (c)
		{
		case 1:
			pjob = CreateJob();
			if (!pjob)
			{
				printf("Create job failed!\n");
				break;
			}
			prest = FFAllocate(prest, pjob,workAd);
			if (!(*workAd))
			{
				printf("Allocate failed, no rest space available!\n");
				break;
			}
			puse = UpdateUse(puse, pjob);
			structPrint(prest, puse); break;
		case 2:
			pjob2 = GetJob(puse);
			if (!pjob2)
			{
				printf("Get job failed!\n");
				break;
			}
			puse = DeleteJob(puse, pjob2);
			prest = UpdateRest(pjob2, prest);
			structPrint(prest, puse); break;
		case 3:
			y = 0; break;
			free(workAd);
		}
	}
}

void BF(void)
{
	struct rest* p1;
	struct use* p2;
	struct job* p, * q;
	int y = 1;
	int n = 0;
	int a = 1;
	int c = 3;
	p1 = InitRestLinkList();
	p2 = InitUseLinkList();
	printf("Initial situation:\n");
	structPrint(p1, p2);
	while (a == y)
	{
		printf("Enter operation id 1-init job; 2-remove job; 3-end : ");
		if (fgets(buffer, sizeof(buffer), stdin))
		{
			sscanf_s(buffer, "%d", &c);
		}
		switch (c)
		{
		case 1:
			p = CreateJob();
			p1 = BFAllocate(p1, p);
			p2 = UpdateUse(p2, p);
			structPrint(p1, p2); break;
		case 2:
			q = GetJob(p2);
			p2 = DeleteJob(p2, q);
			p1 = UpdateRest(q, p1);
			structPrint(p1, p2); break;
		case 3:
			y = 0; break;
		}
	}
}

//void WF(void)
//{
//	struct rest* p1;
//	struct use* p2;
//	struct job* p, * q;
//	int y = 1;
//	int n = 0;
//	int a = 1;
//	int c = 3;
//	p1 = InitRestLinkList();
//	p2 = InitUseLinkList();
//	printf("Initial situation:\n");
//	structPrint(p1, p2);
//	while (a == y)
//	{
//		printf("Enter operation id 1-init job; 2-remove job; 3-end : ");
//		if (fgets(buffer, sizeof(buffer), stdin))
//		{
//			sscanf_s(buffer, "%d", &c);
//		}
//		switch (c)
//		{
//		case 1:
//			p = CreateJob();
//			p1 = WFAllocate(p1, p);
//			p2 = UpdateUse(p2, p);
//			structPrint(p1, p2); break;
//		case 2:
//			q = GetJob(p2);
//			p2 = DeleteJob(p2, q);
//			p1 = UpdateRest(q, p1);
//			structPrint(p1, p2); break;
//		case 3:
//			y = 0; break;
//		}
//	}
//}

int main()
{
	printf("First Fit Allocation Algorithm \n");
	FF();
	//printf("最佳适应算法 \n");
	//BF();
	//printf("最坏适应算法 \n");
	//WF();
	return 0;
}
