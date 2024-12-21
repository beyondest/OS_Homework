#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define DebugPrintf(fmt, ...) { \
    char buffer[256]; \
    sprintf_s(buffer, sizeof(buffer), fmt, __VA_ARGS__); \
    OutputDebugStringA(buffer); \
}


char buffer[200] = { 0 };

CRITICAL_SECTION cs;
int roundCount = 0;
int clipTime = 1;
int processNum = 5;
int delayTime = 100;


#pragma region Process Control Block
typedef struct {
	char name[20];
	int pri;
	int burstTime;
	struct PCB* next;
}PCB, * pPCB;
int InputInitPCB(pPCB p, int id)
{
	if (id == -1)
	{
		printf("Enter process name: ");
		if (fgets(buffer, sizeof(buffer), stdin))
		{
			sscanf_s(buffer, "%s", &p->name, (unsigned int)sizeof(p->name));
			printf("Enter process priority: ");
			if (fgets(buffer, sizeof(buffer), stdin))
			{
				sscanf_s(buffer, "%d", &p->pri);
				printf("Enter process burst time: ");
				if (fgets(buffer, sizeof(buffer), stdin))
				{
					sscanf_s(buffer, "%d", &p->burstTime);
					p->next = NULL;
					return 1;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < sizeof(p->name); i++)
			p->name[i] = '\0';
		p->name[0] = 'P';
		p->name[1] = '0' + id;
		p->pri = 5;
		p->burstTime = 10;
		return 1;
	}
	return 0;
}
#pragma endregion

#pragma region Customized Process Queue
typedef struct {
	pPCB front;
	pPCB rear;
}LinkQueue;
LinkQueue Q;
/**
Initialize a link queue with a head node
*/
void InitLinkQueue(LinkQueue* Q)
{
	pPCB p = (pPCB)malloc(sizeof(PCB));
	if (!p) { DebugPrintf("Memory allocation failed\n"); exit(1); }
	else
	{
		Q->front = Q->rear = p;
		p->next = NULL;
		DebugPrintf("Link queue initialized\n");
	}
}

int QueueEmpty(LinkQueue* Q)
{
	return (Q->front == Q->rear);
}
/*
Return NULL if queue is empty, otherwise return the first node of the queue
*/
pPCB  DeQueue(LinkQueue* Q)
{
	pPCB p;
	if (QueueEmpty(Q)) return NULL;
	p = Q->front->next;
	Q->front->next = p->next;
	if (Q->front->next == NULL) Q->rear = Q->front;
	return p;
}
/**
Decending order of priority sort insert
*/
void EnQueueDecendPri(LinkQueue* Q, pPCB p)
{
	if (Q->front == Q->rear)
	{
		Q->rear->next = p;
		Q->rear = p;
		p->next = NULL;
	}
	else
	{
		pPCB prev = Q->front;
		pPCB curr = Q->front->next;
		while (curr && curr->pri >= p->pri)
		{
			prev = curr;
			curr = curr->next;
		}
		if (!curr)
		{
			prev->next = p;
			Q->rear = p;
			p->next = NULL;
		}
		else
		{
			prev->next = p;
			p->next = curr;
		}
	}
}

void PrintQueueInfo(LinkQueue* Q)
{
	pPCB p = Q->front->next;
	while (p)
	{
		DebugPrintf("Name: %s Priority: %d Burst Time: %d\n", p->name, p->pri, p->burstTime);
		p = p->next;
	}
}

#pragma endregion

#pragma region Threads
DWORD WINAPI AddProcessThread(LPVOID lpParam)
{
	//while (1) {

	//	pPCB p = (pPCB)malloc(sizeof(PCB));
	//	if (!p) { DebugPrintf("Process Memory allocation failed\n"); exit(1); }
	//	if (!InputInitPCB(p, -1))
	//	{
	//		DebugPrintf("Process creation failed, input invalid\n");
	//		free(p);
	//		continue;
	//	}
	//	EnterCriticalSection(&cs);
	//	EnQueueDecendPri(&Q, p);
	//	LeaveCriticalSection(&cs);
	//	Sleep(100);  // Reduce CPU usage, avoid dead loop occupying too much CPU resource
	//}

	return 0;

}

DWORD WINAPI RunThread(LPVOID lpParam)
{
	while (1)
	{
		roundCount++;
		DebugPrintf("Round %d\n", roundCount);
		pPCB p;
		EnterCriticalSection(&cs);
		if (!(p = DeQueue(&Q)))
		{
			DebugPrintf("All processes completed\n");
			LeaveCriticalSection(&cs);
			break;
		}
		LeaveCriticalSection(&cs);
		if (!RunProcess(p, clipTime))
		{
			EnterCriticalSection(&cs);
			EnQueueDecendPri(&Q, p);
			LeaveCriticalSection(&cs);
		}
		EnterCriticalSection(&cs);
		PrintQueueInfo(&Q);
		LeaveCriticalSection(&cs);
		if (delayTime > 0)
		{
			Sleep(delayTime);
		}
	}
}

#pragma endregion

/*
Return 1 if process completed, 0 if not
*/
int RunProcess(pPCB p, int clipTime)
{
	DebugPrintf("Process %s running\n", p->name);
	p->burstTime -= clipTime;
	p->pri--;
	if (p->burstTime <= 0)
	{
		DebugPrintf("Process %s completed\n", p->name);
		free(p);
		return 1;
	}
	return 0;
}


int main()
{
	InitializeCriticalSection(&cs);
	InitLinkQueue(&Q);
	HANDLE thAddProcess, thRun;


	for (int i = 0; i < processNum; i++)
	{
		pPCB p = (pPCB)malloc(sizeof(PCB));
		if (!p) { DebugPrintf("Process Memory allocation failed\n"); exit(1); }
		if (!InputInitPCB(p, -1)) {
			free(p);
			DebugPrintf("Process creation failed, input invalid\n");
			continue;
		}
		DebugPrintf("Process %d created\n", i + 1);
		EnQueueDecendPri(&Q, p);
	}
	PrintQueueInfo(&Q);
	thAddProcess = CreateThread(NULL, 0, AddProcessThread, NULL, 0, NULL);
	thRun = CreateThread(NULL, 0, RunThread, NULL, 0, NULL);
	if (thAddProcess == NULL || thRun == NULL) {
		DebugPrintf("Thread creation failed\n");
		return 1;
	}
	WaitForSingleObject(thAddProcess, INFINITE);
	WaitForSingleObject(thRun, INFINITE);
	DeleteCriticalSection(&cs);

	return 0;
}