#include <stdio.h>
#include <stdlib.h>
int* testallocate()
{
	int* p = (int*)malloc(sizeof(int));
	*p = 10;
	return p;

}

int main() {	
	int* p = testallocate();
	printf("%d\n", *p);
	free(p);
	return 0;
}


