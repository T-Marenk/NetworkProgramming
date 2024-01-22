#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void signalhandler(int a) { 
  printf("\nCaught signal to terminate, %d\n", a);
  exit(1);
};

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
    struct sigaction handler;

    handler.sa_handler = &signalhandler;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;

    sigaction(SIGINT, &handler, NULL);
    copy_file(argv[1]);
  }
  else {
    printf("No file given!\n");
  }

  return 0;
}
