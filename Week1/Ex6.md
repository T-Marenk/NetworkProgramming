# Unix shell commands

## Part a

The command

```bash
ps -aux
```

shows all currently running proccesses. This can also be done with the flag A, but this
gives more information about the running processes.

## Part b

To get information about a running process running a certain program, we can utilize the
unix command grep

```bash
ps -aux | grep {program}
```

## Part c

### 1)

To kill a certain process, we can utilize the kill command

```bash
kill {pid}
```

### 2)

We can use the pkill command with the f flag

```bash
pkill -f {program}
```

### 3)

Again, using the pkill command, using the u flag, we can kill all of my users processes

```bash
pkill -u UID
```

UID is a environment variable which holds the UID of the current user. Alternatively,
the users username can be used instead of UID.