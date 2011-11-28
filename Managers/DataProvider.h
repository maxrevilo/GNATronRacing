#ifndef DATAPROVIDER_H
#define	DATAPROVIDER_H

#include "../GNAFramework/GNAExeptions.h"
#include "../GNAFramework/Geom.h"
#include "../GNAFramework/Color.h"

using namespace GNAFramework;

class DataManager {
public:
    static void loadLevels(const char *path);
    
    static void loadGameDescription(const char *path);
    
private:
    static ContentLoadException badFormatException;
    
    
    DataManager();
    virtual ~DataManager();
};

#endif

