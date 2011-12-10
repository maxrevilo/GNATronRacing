#include "../DataManager.h"
#include "../../tinyxml/tinyxml.h"
#include "../DebugManager.h"
#include "../../GNAFramework/MathHelper.h"

#define MAX_LEVELS 20

char ** DataManager::levels_paths        = NULL;
int     DataManager::levels_paths_lenght = 0;

bool    DataManager::useShaderHeightmap  = false;


float DataManager::unit_size             = 1000.f;

float DataManager::aceleration           = DataManager::unit_size / 10.f;
float DataManager::turboAceleration      = DataManager::unit_size / 2.f;
float DataManager::fuelRegeneration      = 20.f; //20 of fuel per second
float DataManager::fuelConsumption       = 30.f;
float DataManager::speed                 = DataManager::unit_size / 2.f;
float DataManager::turboSpeed            = DataManager::unit_size;
float DataManager::turnRate              = 25.f / 90.f * Pi; //10 degres per second
float DataManager::maxTurnRate           = 25.f / 90.f * Pi; //25 degres per second
float DataManager::turnSlowRate          = 25.f / 90.f * Pi;

Vector3 DataManager::LightPos            = Vector3(6000.f, 10000.f, 10000.f);

void DataManager::loadGameDescription(const char *path) {
    TiXmlDocument doc(path);
    
    if(doc.LoadFile()){
        TiXmlNode *base = doc.FirstChild("conf");
        if(base == NULL) throw new ArgumentException("conf Tag not found. Check the Game Description's XML.");
        
        
        // <editor-fold defaultstate="collapsed" desc="Load Debuger Options">
        TiXmlNode *debuger = base->FirstChild("debuger");
        if(debuger != NULL) {
            const char *verbose = debuger->ToElement()->Attribute("verbose");
            if(verbose != NULL) DebugManager::verbose = strcmp("true", verbose) == 0;
        }
        // </editor-fold>
        
        
        // <editor-fold defaultstate="collapsed" desc="Load Graphic Options">
        TiXmlNode *graphics = base->FirstChild("graphics");
        if(graphics != NULL) {
            TiXmlNode *shaderheightmap = graphics->FirstChild("shaderheightmap");
            if(shaderheightmap != NULL) {
                const char *active = shaderheightmap->ToElement()->Attribute("active");
                if(active != NULL) useShaderHeightmap = strcmp("true", active) == 0;
            }
        }
        // </editor-fold>

        
        // <editor-fold defaultstate="collapsed" desc="Load Leves paths">
        if(levels_paths != NULL) delete []levels_paths;
        
        TiXmlNode *campaing = base->FirstChild("campaing");
        if(campaing == NULL) throw new ArgumentException("campaing Tag not found. Check the Game Description's XML.");
        
        levels_paths = new char *[MAX_LEVELS];
        levels_paths_lenght = 0;
        TiXmlElement *e;
        for (TiXmlNode *o_node = campaing->FirstChild(); o_node != 0; o_node = o_node->NextSibling()) {
            e = o_node->ToElement();
            if(levels_paths_lenght > MAX_LEVELS)
                throw new ArgumentException("The Campaing have to much Levels. Check the Game Description's XML.");
            if(e != NULL) {
                //Asign the path on levels_paths[levels_paths_lenght]:
                const char * path = e->Attribute("path");
                levels_paths[levels_paths_lenght] = new char[strlen(path)+1];
                strcpy(levels_paths[levels_paths_lenght], path);
                levels_paths_lenght++;
            }
        }
        // </editor-fold>

    } else {
        throw new ContentLoadException((char *)"Game Description file couldn't be loaded.");
    }
}

