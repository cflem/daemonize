# daemonize
A utility to send shell commands to the background.

### Quick Install
```
curl -s https://raw.githubusercontent.com/cflem/daemonize/master/daemonize.c | gcc -xc -o daemonize -
sudo mv daemonize /usr/bin
```

### Usage
```
daemonize <command> [--chdir] [--noclose]
```
__command__: *(Required)* The command to execute as a daemon.

__noclose__: *(Optional)* Leaves streams to the current terminal session open.

__chdir__: *(Optional)* Changes directory to / before executing <command>
