#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// Function to read a line from the line
ssize_t getlin(char* line, int fp) {
  int pos = 0; // Indicate how many bytes have been read

  // Loop until it is the end of the line or the file ends
  // The read byte is added to the allocated line array
  while(read(fp, &line[pos], 1)) {
    if (line[pos] == *"\n" || line[pos] == *"\0") {
      return pos+1; // Return the amount of read bytes
    }
    pos++; // Increase the position
  }

  return -1; // Nothing has been read
}

// Function, that doubles each row
void doubler(int fd_in, int fd_out) {
  char *line = malloc(124); // Allocate space for reading lines
  ssize_t nread;

  // Loop, while there are lines left in the given file
  while ((nread = getlin(line, fd_in)) != -1) {
    write(fd_out, line, nread); // Print the lines to standard output
    write(fd_out, line, nread);
  }

  free(line); // Free the allocated space
  close(fd_in); // Close the file
}

int main(int argc, char *argv[], char *envp[]) {
  if (argc == 2) {
    int fd = open(argv[1], O_RDONLY); // Open the file
    doubler(fd, STDOUT_FILENO);
  }
  else {
    doubler(STDIN_FILENO, STDOUT_FILENO);
  }
  
  return 0;
}