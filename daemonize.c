#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>

const struct option longopts[] = {
  {"chdir", no_argument, NULL, 'd'},
  {"noclose", no_argument, NULL, 'n'},
  {"log-stdout", required_argument, NULL, '1'},
  {"log-stderr", required_argument, NULL, '2'},
  {0, 0, 0, 0}
};
const char* optstring = "dn1:2:";

void q (char* m) {
    fprintf(stderr, "%s\n", m);
    exit(1);
}

void qusage () {
    q("[daemonize] Usage: daemonize [-d|--chdir] [-n|--noclose] [-1|--log-stdout /path/to/log] [-2|--log-stderr /path/to/log] <command>");
}

void qtoofewargs () {
    fprintf(stderr, "[daemonize] Too few arguments.\n");
    qusage();
}

void qinvalidarg (char* arg) {
    fprintf(stderr, "[daemonize] Unrecognized argument: %s\n", arg);
    qusage();
}

char* lowerdup (char* a) {
    char* b = strdup(a);
    int i;
    for (i = 0; b[i] != 0; i++) {
        b[i] = tolower(b[i]);
    }
    return b;
}

struct opts {
    int nochdir;
    int noclose;
    FILE* stdlog;
    FILE* errlog;
};

void parseargs(int argc, char** argv, struct opts* savedopts) {
    if (argc < 2) qtoofewargs();
    int c;

    while ((c = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
        switch (c) {
            case 'd':
                savedopts->nochdir = false;
                break;
            case 'n':
                savedopts->noclose = true;
                break;
            case '1':
                savedopts->stdlog = fopen(optarg, "a");
                if (!savedopts->stdlog) q("[daemonize] Unable to open stdout log, quitting.");
                break;
            case '2':
                savedopts->errlog = fopen(optarg, "a");
                if (!savedopts->errlog) q("[daemonize] Unable to open stderr log, quitting.");
                break;
            default:
                // getopt will have printed an error already
                qusage();
    }
  }
}

int main (int argc, char** argv) {
    struct opts options = {true, false, NULL, NULL};
    parseargs(argc, argv, &options);

    int cmdlen = 0, buff_size = 256;
    char* buffer = malloc(buff_size);

    while (optind < argc) {
        int arglen = strlen(argv[optind]);
        while (cmdlen+arglen+1 > buff_size) {
            buff_size *= 2;
            buffer = realloc(buffer, buff_size);
            if (buffer == NULL) q("[daemonize] Out of memory parsing arguments.");
        }
        strncpy(&buffer[cmdlen], argv[optind++], arglen);
        cmdlen += arglen+1;
        buffer[cmdlen-1] = ' ';
    }
    buffer[cmdlen-1] = 0;

    if (cmdlen < 1) qtoofewargs();

    // Set up logs as appropriate
    int logs = options.stdlog || options.errlog;
    if (options.stdlog) {
        dup2(fileno(options.stdlog), STDOUT_FILENO);
        fclose(options.stdlog);
    }
    if (options.errlog) {
        dup2(fileno(options.errlog), STDERR_FILENO);
        fclose(options.errlog);
    }

    if (logs && !options.noclose) {
        int devnull_out = open("/dev/null", O_WRONLY);
        int devnull_in = open("/dev/null", O_RDONLY);
        if (!options.errlog)
            dup2(devnull_out, STDERR_FILENO);
        if (!options.stdlog)
            dup2(devnull_out, STDOUT_FILENO);
        dup2(devnull_in, STDIN_FILENO);
        close(devnull_in);
        close(devnull_out);
    }

    daemon(options.nochdir, options.noclose | logs);
    system(buffer);
}
