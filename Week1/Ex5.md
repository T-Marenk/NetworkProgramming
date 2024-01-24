# The exec system calls

## Part a

For all the exec system calls, the program that is being currently run to be replaced by a
new process with a newly initialized stack and heap.

For execve, on top of all the other exec system calls are build on, takes a certain file
based on its pathname and the arguments and enviroment variables are given to it as vectors.

The execl(), execlp() and execle() functions differ in the sence that instead of having the
arguments as a vector, they are given straight as pointers to null-terminated strings. So,
they are pointers, separeted form one another by a comma. This can be visualised by

`arg0,arg1,...,argn`

Now, the execl() and execle() differ from each other in the way they get the
environment variables. the execle() function gets the environment variables as a vector, just
like execve, but execl() gets the environment variables from an external variable *environ*.
This is also the case for all other exec functions without the e suffix.

The only difference between execv and execve is that execve gets the environment variables
from environ.

All the exec functions that have v after the exec get the arguments as an vector/array.

Now, all the exec functions, that include a p, are not given the executable file as it's pathname,
rather they are given the file name and it searches the places listed in the PATH environment
variable for the executable file.

## Part b

The heap, stack and data segments get changed. Everything else remains the same.