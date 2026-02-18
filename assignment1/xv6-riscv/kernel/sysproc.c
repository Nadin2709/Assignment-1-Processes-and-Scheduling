#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  char exit_msg[32];
  argint(0, &n);
  argstr(1, exit_msg, sizeof(exit_msg));
  uint64 addr;
  argaddr(1, &addr);
  if(addr == 0)
    exit(n, "No exit message");
  else{
     exit_msg[sizeof(exit_msg) - 1] = '\0';
     exit(n, exit_msg);
  }
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  uint64 exit_msg;
  argaddr(1, &exit_msg);

  return wait(p, exit_msg);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_memsize(void)
{
  return myproc()->sz;
}

uint64
sys_forkn(void)
{
  int amount = 0;
  uint64 pid_array = 0;
  // fetching args
  argint(0, &amount);
  argaddr(1, &pid_array);
  return forkn(amount, (int *)pid_array);
}

uint64
sys_waitall(void)
{
  uint64 ptr_n = 0;
  uint64 ptr_status = 0;
  // fetching args
  argaddr(0, &ptr_n);
  argaddr(1, &ptr_status);
  return waitall((int *)ptr_n, (int *)ptr_status);
}