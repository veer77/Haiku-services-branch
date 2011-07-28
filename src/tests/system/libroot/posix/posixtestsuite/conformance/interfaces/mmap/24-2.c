/*
 * Copyright (c) 2002, Intel Corporation. All rights reserved.
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this
 * source tree.

 * The mmap( ) function shall fail if:
 * [ENOMEM] MAP_FIXED was specified,
 * and the range [addr,addr+len) exceeds that allowed
 * for the address space of a process; or, if MAP_FIXED was not specified and
 * there is insufficient room in the address space to effect the mapping.
 *
 * Test Step:
 * 1. Map a shared memory object, with size exceeding the value get from
 *    rlim_cur of resource RLIMIT_AS, setting MAP_FIXED;
 * 3. Should get ENOMEM.
 */

#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "posixtest.h"

#define TNAME "mmap/24-2.c"

int main()
{
  char tmpfname[256];
  int shm_fd;

  void *pa = NULL;
  void *addr = NULL;
  size_t len;
  int prot = PROT_READ | PROT_WRITE;
  int flag = MAP_SHARED;
  int fd;
  off_t off = 0;

  /* Size of the shared memory object */
  size_t shm_size;
  struct rlimit rlim;
  unsigned long page_size = sysconf(_SC_PAGE_SIZE);

  shm_size = 2 * page_size;
  snprintf(tmpfname, sizeof(tmpfname), "pts_mmap_24_2_%ld",
           (long)getpid());

  /* Create shared object */
  shm_unlink(tmpfname);
  shm_fd = shm_open(tmpfname, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);
  if(shm_fd == -1)
  {
    printf(TNAME " Error at shm_open(): %s\n", strerror(errno));
    return PTS_UNRESOLVED;
  }
  shm_unlink(tmpfname);
  if(ftruncate(shm_fd, shm_size) == -1) {
    printf(TNAME " Error at ftruncate(): %s\n", strerror(errno));
    return PTS_UNRESOLVED;
  }

  /* Currentlly total available memory of this process, in bytes */
  if (getrlimit (RLIMIT_AS, &rlim) == -1)
  {
		printf(TNAME " Error at getrlimit(): %s\n", strerror(errno));
		return PTS_UNRESOLVED;
  }
  printf("available memory: %lu\n", rlim.rlim_cur);

  /* First mmap, just to get a legal addr for second mmap */
  fd = shm_fd;
  len = shm_size;
  pa = mmap (addr, len, prot, flag, fd, off);
  if (pa == MAP_FAILED)
  {
    printf ("Test Fail: " TNAME " Error at first mmap(): %s\n",
            strerror(errno));
    exit(PTS_FAIL);
  }

  fd = shm_fd;
  len = rlim.rlim_cur;
  flag = MAP_FIXED | MAP_SHARED;
  addr = pa;
  printf("addr: %lx, len: %lx\n", (unsigned long)addr,
		(unsigned long)len);
  /* Make sure addr and len is aligned to page size */
  if ((unsigned long)addr % page_size)
  {
    /* Upper boundary */
    addr += page_size;
    addr = (void *)((unsigned long)addr & ~(page_size - 1));
  }
  if (len % page_size)
  {
    /* Lower boundary */
    len &= ~(page_size - 1);
  }
  printf("addr: %lx, len: %lx\n", (unsigned long)addr,
		(unsigned long)len);
  pa = mmap (addr, len, prot, flag, fd, off);
  if (pa == MAP_FAILED && errno == ENOMEM)
  {
    printf ("Test Pass: " TNAME " Get ENOMEM: %s\n",
            strerror(errno));
    exit(PTS_PASS);
  }

  if (pa == MAP_FAILED)
    perror("Error at mmap()");
  else
    munmap(pa, len);
  close(fd);
  printf ("Test Fail: Did not get ENOMEM as expected\n");
  return PTS_FAIL;
}
