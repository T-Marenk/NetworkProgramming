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
  int max = 100;

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

// Function, that doubles each row
void doubler(int fd_in, int fd_out) {
  char *line = malloc(124); // Allocate space for reading lines
  ssize_t nread;

  // Read the pipe while there is input
  while ((nread = getlin(line, fd_in)) != -1) {
    write(fd_out, line, nread); // Print the lines to standard output
    write(fd_out, line, nread);
  }

  free(line); // Free the allocated space
  close(fd_in); // Close the pipe
}

int main(int argc, char *argv[], char *envp[]) {
  int readfd, writefd;
  // Check if there are pipes and if not, create them
  if ( (mknod(FIFO1, S_IFIFO | 0666, 0) < 0) && (errno != EEXIST)) {
    fprintf(stderr, "can't create fifo 1: %s", FIFO1); exit(1);
  }
  if ( (mknod(FIFO2, S_IFIFO | 0666, 0) < 0) && (errno != EEXIST)) {
    unlink(FIFO1);
    fprintf(stderr,"can't create fifo 2: %s", FIFO2); exit(1);
  }
  // Open pipe for reading
  if ((readfd = open(FIFO2, O_RDONLY)) < 0) {
    perror("Can open write fifo"); exit(1);
  }
  doubler(readfd, STDOUT_FILENO);
  
  return 0;
}
