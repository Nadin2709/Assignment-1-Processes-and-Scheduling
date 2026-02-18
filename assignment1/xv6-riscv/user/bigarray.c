#include "kernel/types.h"
#include "user.h"

#define ARRAY_SIZE (1 << 16)

int main(int argc, char *argv[]){
  static int arr[ARRAY_SIZE];
  for (int i = 0; i < ARRAY_SIZE; i++) {
    arr[i] = i;
  }

  int pids[4];
  int res = forkn(4,pids);
  if (res == -1) {
    printf("forkn failed\n");
    exit(1,"");
  }

  // child
  if (res > 0) { 
    long sum = 0;
    int index = res - 1;
    int st = (ARRAY_SIZE / 4) * index;
    int end = (ARRAY_SIZE / 4) + st;
    for (int i = st; i < end; i++) {
      sum = sum + arr[i];
    }
    printf("child %d sum = %d\n", index + 1, (int)sum);
    exit((int)sum,"");
  }
  // parent
  for (int i = 0; i < 4; i++) {
    printf("child #%d with pid = %d\n", i + 1, pids[i]);
  }
  // wait
  int statuses[64];
  int child_count = 0;
  if (waitall(&child_count, statuses) < 0) {
    printf("waitall failed\n");
    exit(1,"");
  }

  if (child_count != 4) {
    printf("error, expected %d children but instead have %d\n", 4, child_count);
    exit(1,"");
  }

  long total = 0;
  for (int i = 0; i < 4; i++) {
    printf("child %d status is = %d\n", i + 1, statuses[i]);
    total += statuses[i];
  }
  printf("final sum = %d\n", (int)total);
  exit(0,"");
}