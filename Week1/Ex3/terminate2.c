#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Function, that is called, when a signal is detected
void signalhandler(int a) { 
  printf("\nCaught signal to terminate, %d\n", a);
  exit(1);
};

// Same as in part a
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
    struct sigaction handler; // Struct for sigaction

    handler.sa_handler = &signalhandler; // Change the handler to our own function
    sigemptyset(&handler.sa_mask); // Set signalset to empty
    handler.sa_flags = 0; // No additional behavior wanted for sigaction

    sigaction(SIGINT, &handler, NULL); // Start sigaction to listen to keyboard interrupts
    copy_file(argv[1]);
  }
  else {
    printf("No file given!\n");
  }

  return 0;
}
