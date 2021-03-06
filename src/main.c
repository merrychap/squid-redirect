#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "rewriter.h"
#include "utils.h"

#define True      1
#define MAX_LEN   1000
#define DELIMITER "========================="

int main(int argc, char **argv) {
    rewriter_t rw;
    char recv_data[MAX_LEN];
    char response [MAX_LEN];

    openlog("squid-rewriter", LOG_PID | LOG_CONS, LOG_USER);

    rewriter_init(&rw);
    
    while (fgets(recv_data, MAX_LEN-1, stdin) != NULL) {
        syslog(LOG_INFO, DELIMITER);
        
        memset(response, 0, MAX_LEN);
        rewrite_url(&rw, recv_data, response, MAX_LEN);
        
        fprintf(stdout, "%s", response);
        fflush(stdout);
        
        syslog(LOG_INFO, "<- response: %s", response);
    }
    closelog();
    return 0;
}