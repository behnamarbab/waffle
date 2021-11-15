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
  int MaxInstCount;
};

static int MaxInstCount = 0;

void __attribute__((constructor)) traceBegin(void) {
  ;
}

void __attribute__((destructor)) traceEnd(void) {

  unsigned char *mem_str = getenv(ERU_ENV_VAR);

  if (mem_str) {

    unsigned int shm_mem_id = atoi(mem_str);

    struct sys_data *da;

    da = shmat(shm_mem_id, NULL, 0);

    /* Whooooops. */

    if (da == (void *)-1) _exit(1);

    /* Write something into the bitmap so that even with low AFL_INST_RATIO,
       our parent doesn't give up on us. */

    da->MaxInstCount = MaxInstCount;
  }
}
