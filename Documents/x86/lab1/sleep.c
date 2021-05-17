#include "kernel/types.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: please pass args");
        exit(1);
    } else {
        int ticks = atoi(argv[1]);
        if (ticks > 0) {
            sleep(ticks);
        } else {
            printf("Invalid sleep seconds");
        }
        exit(0);
        
    }
    
}