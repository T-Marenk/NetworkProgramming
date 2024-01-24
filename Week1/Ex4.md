# Child process inherits

When fork is run, the child process is a exact copy of the parent process, i.e it is the same
process but in another memory space. However, some important diferencess remain between the
parent process and the child process.

- The child process has its own PID and PPID is the current processes PID

- The child does not inherit the parent processes memory locks, so if the current process has
claimed some space in memory, the child process can not access that information

- The child process does not inherit the pending signals that the current process might have

- The semaphore adjustments, meaning that it does not have the same amount of cores/threads
to be used as the current process

- The child does inherit open file description locks, meaning that it can use the same files
as the current process, however, it does not inherit process-associated record locks.