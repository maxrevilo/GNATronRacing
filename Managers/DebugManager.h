#ifndef DEBUGMANAGER_H
#define	DEBUGMANAGER_H

#include <stdio.h>
#include <stdlib.h>

class DebugManager {
public:
    static bool verbose;
    static bool graphic;
    
    static void debugInfo(const char * msg){
        if(verbose) {
            printf("%s\n", msg);
            fflush(stdout);
        }
    }
    
    static void debugInfo(const char * msg, int i){
        if(verbose) {
            printf(msg, i);
            printf("\n");
            fflush(stdout);
        }
    }
    
private:

};

#endif	/* DEBUGMANAGER_H */

