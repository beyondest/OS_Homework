#pragma once
#define O1 sizeof(struct job)
#define O2 sizeof(struct rest)
#define O3 sizeof(struct use)
#define SUM 100
#define FirstAddress 1

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