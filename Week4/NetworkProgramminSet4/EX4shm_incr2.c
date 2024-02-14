#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
			/* default permissions for new files */
#define LOCKFILE       "/tmp/luckfile" 

void my_lock_init(char *pathname);
void my_lock_wait();
void my_lock_release();

int
main(int argc, char **argv)
{
	int		fd, i, nloop, zero = 0;
	int		*index;
	char 	*buf, *char_index;
	int 	tmp;
	pid_t		pid;

	if (argc != 4) {
		perror("usage: incr2 <pathname> <char> <#loops>"); exit(1); }
	nloop = atoi(argv[3]);

		/* 4open file, initialize to 0, map into memory */
	if ((fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) < 0) {
		perror("open failed"); exit(1); }
	if (write(fd, &zero, sizeof(int)) <0) {
		perror("write failed"); exit(1); }
	index = mmap(NULL, sizeof(int) + 1241*sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (index == MAP_FAILED) {
		perror("mmap failed"); exit(1); }
	char_index = (char*)index + sizeof(int);
	buf = (char*)index + sizeof(int) + sizeof(char);
	close(fd);

	char_index[0] = *"A";

	/* 4create, initialize, and unlink file lock */
	my_lock_init(LOCKFILE);

	i = 0;
	while (i < nloop) {
		my_lock_wait();
		if (char_index[0] == *argv[2]) {
			my_lock_release();
			continue;
		}
		buf[(*index)++] = *argv[2];
		char_index[0] = *argv[2];
		++i;
		my_lock_release();
	}

	buf[*index] = *"\0";
	printf("%s\n", buf);
	unlink(LOCKFILE);
	exit(0);

}
