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
    
    static float aceleration;
    static float turboAceleration;
    static float fuelRegeneration;
    static float fuelConsumption;
    static float speed;
    static float turboSpeed;
    static float turnRate;
    static float maxTurnRate;
    static float turnSlowRate;
    
    static Vector3 LightPos;
    
    static bool useShaderHeightmap;
    
    
    
    static void loadGameDescription(const char *path);
    
private:
    
};

#endif //DATAMANAGER

