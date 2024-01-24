#include <bits/types/struct_timeval.h>
#include <endian.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

// Get the external variable environ, which holds all the environment variables
extern char **environ;

void process_id(void) {
  // Structs to be used by gettimeofday
  struct timeval t0;
  struct timeval t1;

  gettimeofday(&t0, NULL); // Get time before the commands are executed
  
  pid_t pid = getpid(); // Get process id
  pid_t ppid = getppid(); // Get process parent id
  pid_t pgid = getpgid(pid); // Get process group id
  pid_t uid = getuid(); // Get user id

  gettimeofday(&t1, NULL); // Get time after the commands have been executed

  long elapsed = (t1.tv_sec - t0.tv_sec)*1000000 + t1.tv_usec - t0.tv_usec; // Calculate elapsed time

  printf("%d\n%d\n%d\n%d\n", pid,ppid,pgid,uid); // Print all the ids
  printf("%ld us\n", elapsed);
}

void print_environ() {
  struct timeval t0;
  struct timeval t1;

  gettimeofday(&t0, NULL);

  int i = 0;
  // Loop over all the environment variables in the environ array and print them
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