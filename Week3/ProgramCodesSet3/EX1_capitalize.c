#include <ctype.h>
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

  /* Loop until it is the end of the line or the file ends
   * The loop also ends, if the buffer becomes full
   * The read byte is added to the allocated line array */
  while(read(fp, &line[pos], 1)) {
    line[pos] = toupper(line[pos]); // Capitalize the letter
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

  // Loop, while there are lines left in the pipe
  while ((nread = getlin(line, fd_in)) != -1) {
    write(fd_out, line, nread); // Copy the lines to the pipe
  }

  close(fd_in); // Close pipe for reading
  close(fd_out); // Close pipe for writing
}

int main(int argc, char *argv[], char *envp[]) {
  int readfd, writefd;
  // Check if there is pipe and if not, create it
  if ( (mknod(FIFO1, S_IFIFO | 0666, 0) < 0) && (errno != EEXIST)) {
    fprintf(stderr, "can't create fifo 1: %s", FIFO1); exit(1);
  }
  // Check if there is pipe and if not, create it
  if ( (mknod(FIFO2, S_IFIFO | 0666, 0) < 0) && (errno != EEXIST)) {
    unlink(FIFO1);
    fprintf(stderr,"can't create fifo 2: %s", FIFO2); exit(1);
  }
  // Open the second pipe for writing
  if ((writefd = open(FIFO2, O_WRONLY)) < 0) {
    perror("Can't open write fifo"); exit(1);
  }
  // Open the first pipe for reading
  if ((readfd = open(FIFO1, O_RDONLY)) < 0) {
    close(writefd);
    perror("Can't open read fifo"); exit(1);
  }
  reader(readfd, writefd);
  
  return 0;
} 