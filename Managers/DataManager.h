#ifndef DATAMANAGER_H
#define	DATAMANAGER_H

#include "../GNAFramework/GNAExeptions.h"
#include "../GNAFramework/Geom.h"
#include "../GNAFramework/Color.h"

using namespace GNAFramework;

class DataManager {
public:
    static char **levels_paths;
    static int levels_paths_lenght;
    
    /**
     * The Size of a unit os measure.
     */
    static float unit_size;
    
    static void loadGameDescription(const char *path);
    
private:
    
};

#endif //DATAMANAGER

