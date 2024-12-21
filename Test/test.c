#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <crtdbg.h>
typedef struct List
{
	int data;
	int a;
}TList;
int* testallocate()
{
	int* p = (int*)malloc(sizeof(int));
	if(!p)exit(1);

	*p = 10;
	return p;

}

int main() {	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int count = 1000;
	TList list = { .a = 10,.data = 20 };
	while (1)
	{
		count--;
		if (count == 0) break;
		int* p = testallocate();
		free(p);
		Sleep(1);
		
	}

	return 0;
}


