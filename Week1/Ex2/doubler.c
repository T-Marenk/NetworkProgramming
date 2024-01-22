#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void stdin_with_stdio() {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while ((nread = getline(&line, &len, stdin)) != -1) {
     printf(line, nread, 1, stdout);
     printf(line, nread, 1, stdout);
  }

  free(line);
}

void file_with_stdio(char* filepath) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  fp = fopen(filepath, "r");

  while ((nread = getline(&line, &len, fp)) != -1) {
     printf(line, nread, 1, stdout);
     printf(line, nread, 1, stdout);
  }

  free(line);
  fclose(fp);
}

ssize_t getlin(char* line, int fp) {
  int pos = 0;

  while(read(fp, &line[pos], 1)) {
    if (line[pos] == *"\n" || line[pos] == *"\0") {
      return pos+1;
    }
    pos++;
  }

  return -1;
}

void file_with_syscalls(char* filepath) {
  char *line = malloc(124);
  int fp = open(filepath, O_RDONLY);
  ssize_t nread;

  while ((nread = getlin(line, fp)) != -1) {
    write(STDOUT_FILENO, line, nread);
    write(STDOUT_FILENO, line, nread);
  }

  free(line);
  close(fp);
}

void stdin_with_syscalls() {
  char *line = malloc(124);
  ssize_t nread;

  while ((nread = getlin(line, STDIN_FILENO)) != -1) {
    write(STDOUT_FILENO, line, nread);
    write(STDOUT_FILENO, line, nread);
  }

  free(line);
}

int main(int argc, char *argv[], char *envp[]) {
  if (argc == 2) {
    file_with_stdio(argv[1]);
    file_with_syscalls(argv[1]);
  }
  else {
    stdin_with_stdio();
    stdin_with_syscalls();
  }
  
  return 0;
}