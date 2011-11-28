#include "../DataProvider.h"
#include "../../tinyxml/tinyxml.h"


void DataManager::loadLevels(const char *path){
    TiXmlDocument doc(path);
    
    if(doc.LoadFile()){
        TiXmlNode *base = doc.FirstChild("proyecto")->FirstChild("niveles");
        if(!base) throw new ContentLoadException("Error de formato en el archivo");
        
        try {
            //levels = Levels_d(base);
        } catch(GNAException *e) {
            throw new ContentLoadException("No se pudo cargar el archivo de descripcion de Niveles.", e);
        }
        
    } else {
        throw new ContentLoadException((char *)"No se pudo cargar el archivo de descripcion de Niveles.");
    }
}

#include <stdio.h>
void DataManager::loadGameDescription(const char *path) {
    TiXmlDocument doc(path);
    
    if(doc.LoadFile()){
        
    } else {
        throw new ContentLoadException((char *)"No se pudo cargar el archivo de descripcion del juego.");
    }
}



DataManager::DataManager() {
}

DataManager::~DataManager() {
}

