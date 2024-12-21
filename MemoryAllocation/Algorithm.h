#pragma once
#include "main.h"
__declspec(dllexport) struct rest* FFAllocate(struct rest* resthead, struct job* pjob);
__declspec(dllexport) struct rest* BFAllocate(struct rest* resthead, struct job* pjob);
__declspec(dllexport) struct rest* WFAllocate(struct rest* resthead, struct job* pjob);
