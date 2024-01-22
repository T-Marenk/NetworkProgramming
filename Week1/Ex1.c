#include <bits/types/struct_timeval.h>
#include <endian.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

extern char **environ;

void process_id(void) {
  struct timeval t0;
  struct timeval t1;

  gettimeofday(&t0, NULL);
  
  pid_t pid = getpid();
  pid_t ppid = getppid();
  pid_t pgid = getpgid(pid);
  pid_t uid = getuid();

  gettimeofday(&t1, NULL);

  long elapsed = (t1.tv_sec - t0.tv_sec)*1000000 + t1.tv_usec - t0.tv_usec;

  printf("%d\n%d\n%d\n%d\n", pid,ppid,pgid,uid);
  printf("%ld us\n", elapsed);
}

void print_environ() {
  struct timeval t0;
  struct timeval t1;

  gettimeofday(&t0, NULL);

  int i = 0;
  while(environ[i]) {
    printf("%s\n", environ[i++]);
  }

  gettimeofday(&t1, NULL);

  long elapsed = (t1.tv_sec - t0.tv_sec)*1000000 + t1.tv_usec - t0.tv_usec;

  printf("%ld us\n", elapsed);
}

int main(int argc, char *argv[], char *envp[]) {

  process_id();

  print_environ();

  return 0;
}