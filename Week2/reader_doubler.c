#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

// Function to read a line from the line
ssize_t getlin(char* line, int fp) {
  int pos = 0; // Indicate how many bytes have been read
  int max = 123;

  // Loop until it is the end of the line or the file ends
  // The loop also ends, if the buffer becomes full
  // The read byte is added to the allocated line array
  while(read(fp, &line[pos], 1)) {
    if (line[pos] == *"\n" || line[pos] == *"\0" || pos == max) {
      return pos+1; // Return the amount of read bytes
    }
    pos++; // Increase the position
  }

  return -1; // Nothing has been read
}

void reader(int fd_in, int fd_out) {
  char *line = malloc(124); // Allocate space for reading lines
  ssize_t nread;

  // Loop, while there are lines left in the given file
  while ((nread = getlin(line, fd_in)) != -1) {
    write(fd_out, line, nread); // Copy the lines to the pipe
  }

  close(fd_out);
}

// Function, that doubles each row
void doubler(int fd_in, int fd_out) {
  char *line = malloc(124); // Allocate space for reading lines
  ssize_t nread;

  while ((nread = getlin(line, fd_in)) != -1) {
    write(fd_out, line, nread); // Print the lines to standard output
    write(fd_out, line, nread);
  }

  free(line); // Free the allocated space
  close(fd_in); // Close the file
}

int main(int argc, char *argv[], char *envp[]) {
  int childpid, pipe1[2];
  if (pipe(pipe1) < 0) {
    write(STDOUT_FILENO, "Error while creating pipe", 25);
    exit(1);
  }
  if ((childpid = fork()) < 0) {
    exit(1);
  } else if (childpid > 0) {
    close(pipe1[0]);
    reader(STDIN_FILENO, pipe1[1]);
    wait(NULL);
  } else {
    close(pipe1[1]);
    doubler(pipe1[0], STDOUT_FILENO);
  }
  
  return 0;
}