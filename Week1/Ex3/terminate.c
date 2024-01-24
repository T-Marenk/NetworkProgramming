#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// The name of the function, that will be called when a signal is detected
void signalhandler(int);

// Function, that will copy file to standard output. Basically the same as exercise 2
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
    // Initialize signal. Using SIGINT, which is a interrupt from keyboard
    signal(SIGINT, signalhandler);
    copy_file(argv[1]);
  }
  else {
    printf("No file given!\n");
  }

  return 0;
}

// This is the function that gets called, when a signal is detected
void signalhandler(int signum) {
  printf("Caught signal to terminate, %d\n", signum);
  exit(1);
}