#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// Function, when no file is given and reading happens from standard input
void stdin_with_stdio() {
  // Variables needed for getline
  char *line = NULL; // Line, that has been read
  size_t len = 0; // Length of the line
  ssize_t nread; // Amount of read bytes

  // Loop while there are lines left in the standard input
  while ((nread = getline(&line, &len, stdin)) != -1) {
    printf("%s", line); // Print the read line
    printf("%s", line);
  }

  free(line); // Free the allocated space
}

// Function, when a file is given. Mostly the same as with standard input
void file_with_stdio(char* filepath) {
  FILE *fp; // This is the variable to which the opened file gets attached
  // These are the same as with stdin
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  fp = fopen(filepath, "r"); // Open the file

  // Same as with stdin
  while ((nread = getline(&line, &len, fp)) != -1) {
     printf(line, nread, 1, stdout);
     printf(line, nread, 1, stdout);
  }

  free(line);
  fclose(fp); // Close the file
}

int main(int argc, char *argv[], char *envp[]) {
  // Check how many arguments there are. If there are two, then a file has been given
  // as a argument
  if (argc == 2) {
    file_with_stdio(argv[1]);
  }
  else {
    stdin_with_stdio();
  }
  
  return 0;
}