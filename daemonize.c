#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

void q (char* m) {
    fprintf(stderr, "%s\n", m);
    exit(1);
}

void qtoofewargs () {
    q("[daemonize] Too few arguments.\nUsage: daemonize [--chdir] [--noclose] <command>");
}

char* lowerdup (char* a) {
    char* b = strdup(a);
    int i;
    for (i = 0; b[i] != 0; i++) {
        b[i] = tolower(b[i]);
    }
    return b;
}

int main (int argc, char** argv) {
    if (argc < 2) qtoofewargs();
    int i, cmdlen = 0, buff_size = 256, nochdir = 1, noclose = 0;
    char* buffer = malloc(buff_size);
    
    for (i = 1; i < argc; i++) {
        char* cmp = lowerdup(argv[i]);
        if (strcmp(cmp, "--chdir") == 0) nochdir = 0;
        else if (strcmp(cmp, "--noclose") == 0) noclose = 1;
        else {
            int arglen = strlen(argv[i]);
            if (cmdlen+arglen+1 > buff_size) {
                buff_size *= 2;
                buffer = realloc(buffer, buff_size);
                if (buffer == NULL) q("[daemonize] Out of memory parsing arguments.");
            }
            strncpy(&buffer[cmdlen], argv[i], arglen);
            cmdlen += arglen+1;
            buffer[cmdlen-1] = ' ';
        }
        free(cmp);
    }
    buffer[cmdlen-1] = 0;
    
    if (cmdlen < 1) qtoofewargs();

    daemon(nochdir, noclose);
    system(buffer);
}
