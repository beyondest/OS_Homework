#include "main.h"
#include "LinkList.h"
#include "Algorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "crtdbg.h"

char buffer[200];
int algorithm = 1;
int iterations = 1000;
int sleeptime = 100;


#pragma region RandomInput
/*
Generate a random job
*/
struct job* GenerateRandomJob(void)
{
    struct job* p = (struct job*)malloc(O1);
    if (!p) exit(1);

    p->name = rand() % 1000; // Random name between 0 and 999
    p->size = (rand() % 100) + 1; // Random size between 1 and 100
    p->ad = 0;
    return p;
}

/*
Generate a random operation id
*/
int GenerateRandomOperation(void)
{
    return (rand() % 2) + 1; // Random operation id between 1 and 2
}

/*
Find a random job to delete in usehead list
*/
struct job* FindJobRandomly(struct use* usehead)
{
	struct use* p = usehead;
	int count = 0;
	while (p)
	{
		count++;
		p = p->next;
	}
	if (count == 0) return NULL;
	int num = rand() % count + 1;
	p = usehead;
	count = 0;
	while (p && count < num - 1)
	{
		count++;
		p = p->next;
	}
	struct job* p1 = (struct job*)malloc(O1);
	if (!p1) exit(1);
	p1->name = p->name;
	p1->size = p->size;
	p1->ad = p->ad;
	return p1;
}
#pragma endregion

#pragma region InputFunctions
/*
Create a job by input and return the job
*/
struct job* InitJobByInput(void)
{
    struct job* p = (struct job*)malloc(O1);
    if (!p) exit(1);

    printf("Enter name(int): ");
    if (fgets(buffer, sizeof(buffer), stdin))
    {
        sscanf_s(buffer, "%d", &p->name);
        printf("Enter size: ");
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            sscanf_s(buffer, "%d", &p->size);
            p->ad = 0;
            return p;
        }
    }
    free(p);
    return NULL;
}

/*
Get a job by input and check if it exists in use linklist
@return: the job if it exists, otherwise NULL
*/
struct job* GetJobByInput(struct use* usehead)
{
    struct job* p1 = (struct job*)malloc(O1);
    if (!p1) exit(1);

    int num = 0;
    printf("Enter job name you want to remove: ");
    if (fgets(buffer, sizeof(buffer), stdin))
    {
        sscanf_s(buffer, "%d", &num);
    }

    struct use* p2 = usehead;
    while (p2 && num != p2->name)
    {
        p2 = p2->next;
    }

    if (p2 && num == p2->name)
    {
        p1->name = p2->name;
        p1->size = p2->size;
        p1->ad = p2->ad;
        return p1;
    }

    free(p1);
    return NULL;
}

#pragma endregion

void structPrint(struct rest* h1, struct use* h2)
{
    struct rest* m1 = h1;
    struct use* n1 = h2;
    int count = 0;

    if (!m1)
    {
        printf("Rest LinkList is empty\n");
    }
    else
    {
        while (m1)
        {
            count++;
            printf("########## RestBlock: %d, Address: %d, Size: %d ########\n", count, m1->ad, m1->size);
            m1 = m1->next;
        }
    }

    if (!n1)
    {
        printf("Use LinkList is empty\n");
    }
    else
    {
        count = 0;
        while (n1)
        {
            count++;
            printf("********** Use Block: %d, Address: %d, Size: %d, Name: %d ***********\n", count, n1->ad, n1->size, n1->name);
            n1 = n1->next;
        }
    }
}

void Run(int algorithm)
{
    struct rest* prest = InitRestLinkList();
    struct use* puse = InitUseLinkList();
    struct job* pjob = NULL;
    struct job* pjob2 = NULL;

    printf("Initial situation:\n");
    structPrint(prest, puse);

    int y = 1;
    while (y)
    {
        int c = 3;
        printf("Enter operation id 1-init job; 2-remove job; 3-end: ");
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            sscanf_s(buffer, "%d", &c);
        }

        switch (c)
        {
        case 1:
            pjob = InitJobByInput();
            if (!pjob)
            {
                printf("Create job failed!\n");
                break;
            }

            if (algorithm == 1) prest = FFAllocate(prest, pjob);
            else if (algorithm == 2) prest = BFAllocate(prest, pjob);
            else if (algorithm == 3) prest = WFAllocate(prest, pjob);

            if (!pjob->ad)
            {
                printf("Allocate failed, no rest space available!\n");
                break;
            }
            puse = AddJob2UseLlist(puse, pjob);
            structPrint(prest, puse);
            free(pjob);

            break;

        case 2:
            pjob2 = GetJobByInput(puse);
            if (!pjob2)
            {
                printf("Get job failed!\n");
                break;
            }
            puse = DelJobFromUseLlist(puse, pjob2);
            prest = RecycleRestLlist(pjob2, prest);
            structPrint(prest, puse);
            free(pjob2);
            break;

        case 3:
            y = 0;
            break;

        default:
            printf("Invalid operation id\n");
            break;
        }
    }

    FreeRestLinkList(prest);
    FreeUseLinkList(puse);
}

void RunRandom(int algorithm, int iterations)
{
    struct rest* prest = InitRestLinkList();
    struct use* puse = InitUseLinkList();
    struct job* pjob = NULL;
    struct job* pjob2 = NULL;

    printf("Initial situation:\n");
    structPrint(prest, puse);

    for (int i = 0; i < iterations; i++)
    {
		Sleep(sleeptime); // Sleep for 100ms
        int c = GenerateRandomOperation();

        switch (c)
        {
        case 1:
            pjob = GenerateRandomJob();
            if (!pjob)
            {
                printf("Create job failed!\n");
                break;
            }

            if (algorithm == 1) prest = FFAllocate(prest, pjob);
            else if (algorithm == 2) prest = BFAllocate(prest, pjob);
            else if (algorithm == 3) prest = WFAllocate(prest, pjob);

            if (!pjob->ad)
            {
                printf("Allocate failed, no rest space available!\n");
                break;
            }

            puse = AddJob2UseLlist(puse, pjob);
            structPrint(prest, puse);
            free(pjob);
            break;

        case 2:
            pjob2 = FindJobRandomly(puse);
            if (!pjob2) break;
            puse = DelJobFromUseLlist(puse, pjob2);
            prest = RecycleRestLlist(pjob2, prest);
            structPrint(prest, puse);
            free(pjob2);
            break;

        case 3:
            printf("Ending simulation\n");
            i = iterations; // End the loop
            break;

        default:
            printf("Invalid operation id\n");
            break;
        }
    }

    FreeRestLinkList(prest);
    FreeUseLinkList(puse);
}

void RunRandomWithInput(int algorithm, int iterations)
{
    struct rest* prest = InitRestLinkList();
    struct use* puse = InitUseLinkList();
    struct job* pjob = NULL;
    struct job* pjob2 = NULL;
    printf("Initial situation:\n");
    structPrint(prest, puse);
    for (int i = 0; i < iterations; i++)
    {
        int c = 3;
        printf("Enter operation id 1-init job; 2-remove job; 3-end: ");
        if (fgets(buffer, sizeof(buffer), stdin))
        {
            sscanf_s(buffer, "%d", &c);
        }
        switch (c)
        {
        case 1:
			pjob = GenerateRandomJob();
            if (!pjob)
            {
                printf("Create job failed!\n");
                break;
            }
            if (algorithm == 1) prest = FFAllocate(prest, pjob);
            else if (algorithm == 2) prest = BFAllocate(prest, pjob);
            else if (algorithm == 3) prest = WFAllocate(prest, pjob);
            if (!pjob->ad)
            {
                printf("Allocate failed, no rest space available!\n");
                break;
            }
            puse = AddJob2UseLlist(puse, pjob);
            structPrint(prest, puse);
            free(pjob);
            break;
        case 2:
			pjob2 = FindJobRandomly(puse);
            if (!pjob2)
            {
                printf("Get job failed!\n");
                break;
            }
            puse = DelJobFromUseLlist(puse, pjob2);
            prest = RecycleRestLlist(pjob2, prest);
            structPrint(prest, puse);
            free(pjob);
            break;
        case 3:
            printf("Ending simulation\n");
            i = iterations; // End the loop
            break;
        default:
            printf("Invalid operation id\n");
            break;
        }
    }
    FreeRestLinkList(prest);
    FreeUseLinkList(puse);
}


int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    srand((unsigned int)time(NULL)); // Seed the random number generator
    //printf("First Fit Allocation Algorithm\n");
    //Run(algorithm);
    printf("\nRandom Simulation\n");
    RunRandomWithInput(algorithm, iterations); // Run 100 random operations
    _CrtDumpMemoryLeaks();
    return 0;
}
