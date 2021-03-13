#include "../config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>//ipc
#include <sys/shm.h>
#include <unistd.h>

#ifdef __GNUC__
#define _msize malloc_usable_size
#endif

struct sys_data
{
	unsigned long long int MaxContinueCMNum;
	unsigned long long int MaxCallNum;
  unsigned long long int MaxInstCount;
};

static unsigned long long int MaxCallNum = 0;
static unsigned long long int ContinueCallNum = 1000000; //防溢出
static unsigned long long int MaxContinueCMNum = 0;
static unsigned long long int MaxInstCount = 0;

void __attribute__((constructor)) traceBegin(void) {
  ;
}

void instr_AddInsts(int cnt) {
  MaxInstCount += (unsigned long long)cnt;
}