#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rewriter.h"
#include "utils.h"

#define True    1
#define MAX_LEN 1000

int main(int argc, char **argv) {
    char recv_data[MAX_LEN];
    char response [MAX_LEN];

    /* TEST */
    json_load_file("redirections.json");
    //////////
    
    while (fgets(recv_data, MAX_LEN-1, stdin) != NULL) {
        memset(response, 0, MAX_LEN);
        rewrite_url(recv_data, response, MAX_LEN);
        fprintf(stdout, "%s", response);
        fflush(stdout);
    }
    return 0;
}