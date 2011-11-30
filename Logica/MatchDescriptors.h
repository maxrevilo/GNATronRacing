#ifndef MATCHDESCRIPTORS_H
#define	MATCHDESCRIPTORS_H

#include <stdlib.h>
#include "../GNAFramework/Geom.h"
#include "../GNAFramework/GNAExeptions.h"
#include "../tinyxml/tinyxml.h"
#include "../GNAFramework/Color.h"

#define DEFAULT_LEVEL_WIDTH  100
#define DEFAULT_LEVEL_DEEP   100
#define DEFAULT_LEVEL_HEIGHT  10

#define DEFAULT_PLAYER_HEIGHT 0.7f

using namespace GNAFramework;

extern Vector3 XML2VEC3(TiXmlNode* node);

struct Pathway_d {
    float speed;
    int length;
    Vector3 *points;
    
    Pathway_d(){ length = 0; points = NULL; }
    
    Pathway_d(TiXmlNode* node);
};

struct Player_d {
    float shoot;
    Pathway_d pathway;
    Color color;
    
    float   height;
    
    Player_d(){ }
    
    Player_d(TiXmlNode* node);
};
typedef struct Player_d User_d; 
typedef struct Player_d Enemy_d; 

struct Enemys_d {
    int length;
    Enemy_d *list;
    
    Enemys_d() { length = 0; list = NULL; }
    
    Enemys_d(TiXmlNode* node);
};

struct Obstacle_d {
    float size;
    Vector3 position;
    enum Type{
        Sphere,
        Cube,
        Mesh
    } type;
    
    char *mesh_path;
    
    Obstacle_d() { size = 0.f; mesh_path = NULL; }
    
    Obstacle_d(TiXmlNode* node);
    
    static Type getType(const char *str);
};

struct Obstacles_d {
    int length;
    Obstacle_d *list;
    
    Obstacles_d() { length = 0; list = NULL; }
    
    Obstacles_d(TiXmlNode* node);
};

struct Level_d{
    int id;
    int game_time;
    int width;
    int deep;
    int height;
    User_d   user;
    Enemys_d enemys;
    Obstacles_d obstacles;
    
    Level_d(){ id = -1; }
    
    Level_d(TiXmlNode* node);
};

struct Levels_d{
    int length;
    Level_d *list;
    
    Levels_d(){ length=0; list = NULL; }
    
    Levels_d(TiXmlNode* node);
};

#endif	/* MATCHDESCRIPTORS_H */

