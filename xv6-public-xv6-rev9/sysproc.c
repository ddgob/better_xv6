#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_date(void)
{
  char *ptr;
  argptr(0, &ptr, sizeof(struct rtcdate*));

  struct rtcdate *timestamp = (struct rtcdate *)ptr;
  if (timestamp == 0) {
    return -1;
  }
  
  cmostime(timestamp);

  return 0;
}

int
sys_forkcow(void)
{
  return forkcow();
}

pte_t* walkpgdir(pde_t *pgdir, const void *va, int alloc);

int
sys_virt2real(void)
{
  char *va;
  pte_t *pte;

  if (argptr(0, (void*)&va, sizeof(char*)) < 0) {
    return -1;
  }

  pte = walkpgdir(proc->pgdir, va, 0);
  if (!pte || !(*pte & PTE_P)) {
    return -1;
  }

  int pa_num = PTE_ADDR(*pte) | ((uint)va & 0xFFF);

  return pa_num;
}

int
sys_num_pages(void)
{
  uint va;
  int num_pages = 0;
  pte_t *pte;

  for (va = 0; va < proc->sz; va += PGSIZE) {
    
    pte = walkpgdir(proc->pgdir, (void*)va, 0);
    if (pte && (*pte & PTE_P)) {
      num_pages++;
    }
    
  }

  return num_pages;
}

