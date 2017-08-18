# daemonize
A utility to send shell commands to the background.

### Instant Install
Linux/BSD/\*nix other than OSX
```bash
curl -s https://raw.githubusercontent.com/cflems/daemonize/master/daemonize.c | sudo gcc -xc -o /usr/bin/daemonize -
```
OSX
```bash
curl -s https://raw.githubusercontent.com/cflems/daemonize/master/daemonize.c | sudo gcc -xc -o /usr/local/bin/daemonize -
```

### Usage
```bash
daemonize <command> [--chdir] [--noclose]
```
__command__: *(Required)* The command to execute as a daemon.

__noclose__: *(Optional)* Leaves streams to the current terminal session open.

__chdir__: *(Optional)* Changes directory to / before executing <command>
