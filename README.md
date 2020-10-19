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
daemonize <command> [-d|--chdir] [-n|--noclose] [-1|--log-stdout <logfile>] [-2|--log-stderr <logfile>]
```
__command__: *(Required)* The command to execute as a daemon.

__--noclose__: *(Optional)* Leaves input and output streams to the current terminal session open. Individual streams are overridden with logging options.

__--chdir__: *(Optional)* Changes directory to / before executing <command>

__--log-stdout <logfile>__: *(Optional)* Reroutes the program's stdout to the given file before daemonizing.

__--log-stderr <logfile>__: *(Optional)* Reroutes the program's stderr to the given file before daemonizing.
