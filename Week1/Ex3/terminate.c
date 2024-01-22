#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void signalhandler(int);

void copy_file(char* filapath) {
  char *line = malloc(124);
  int fp = open(filapath, O_RDONLY);
  ssize_t nread;

  while (read(fp, &line[0], 1)) {
    write(1, line, 1);
  }
}

int main(int argc, char *argv[], char *envp[]) {
  if (argc == 2) {
    signal(SIGINT, signalhandler);
    copy_file(argv[1]);
  }
  else {
    printf("No file given!\n");
  }

  return 0;
}

void signalhandler(int signum) {
  printf("Caught signal to terminate, %d\n", signum);
  exit(1);
}