#pragma once
#include "main.h"


// Function declarations
__declspec(dllexport) struct rest* InitRestLinkList(void);
__declspec(dllexport) struct use* InitUseLinkList(void);
__declspec(dllexport) struct use* AddJob2UseLlist(struct use* usehead, struct job* pjob);
__declspec(dllexport) struct use* DelJobFromUseLlist(struct use* usehead, struct job* p);
__declspec(dllexport) struct rest* RecycleRestLlist(struct job* p, struct rest* resthead);
__declspec(dllexport) struct rest* FreeRestLinkList(struct rest* resthead);
__declspec(dllexport) struct use* FreeUseLinkList(struct use* usehead);


