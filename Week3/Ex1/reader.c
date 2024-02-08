#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

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

// Read lines from file
void reader(int fd_in, int fd_out) {
  char *line = malloc(124); // Allocate space for reading lines
  ssize_t nread;

  // Loop, while there are lines left in the file//input
  while ((nread = getlin(line, fd_in)) != -1) {
    if (nread <= 100) { // Discard the line if it is too long
      write(fd_out, line, nread); // Copy the lines to standard output
    }
  }

  close(fd_out); // Close the pipe for writing
}

int main(int argc, char *argv[], char *envp[]) {
  int readfd, writefd;
  // Check that there is a pipe and if not, create it
  if ( (mknod(FIFO1, S_IFIFO | 0666, 0) < 0) && (errno != EEXIST)) {
    fprintf(stderr, "can't create fifo 1: %s", FIFO1); exit(1);
  }
  // Check that there is a pipe and if not, create it
  if ( (mknod(FIFO2, S_IFIFO | 0666, 0) < 0) && (errno != EEXIST)) {
    unlink(FIFO1);
    fprintf(stderr,"can't create fifo 2: %s", FIFO2); exit(1);
  }
  // Open pipe for writing
  if ((writefd = open(FIFO1, O_WRONLY)) < 0) {
    perror("Can open write fifo"); exit(1);
  }
  reader(STDIN_FILENO, writefd);
  
  return 0;
}      