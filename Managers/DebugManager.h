#ifndef DEBUGMANAGER_H
#define	DEBUGMANAGER_H

#include <stdio.h>
#include <stdlib.h>

class DebugManager {
public:
    static bool verbose;
    
    static void debugInfo(const char * msg){
        if(verbose) {
            printf("%s\n", msg);
            fflush(stdout);
        }
    }
    
private:

};

#endif	/* DEBUGMANAGER_H */

