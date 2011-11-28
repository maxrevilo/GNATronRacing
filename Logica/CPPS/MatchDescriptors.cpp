#include "../MatchDescriptors.h"

Vector3 XML2VEC3(TiXmlNode* node){
    double x = 0.0, y = 0.0, z = 0.0;
    TiXmlElement *elm = node->ToElement();
    
    elm->Attribute("x", &x);
    elm->Attribute("y", &y);
    elm->Attribute("z", &z);
    
    return Vector3(
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(z)
            );
}


Pathway_d::Pathway_d(TiXmlNode* node){
    TiXmlElement *elm = node->ToElement();
    if(elm->QueryFloatAttribute("velocidad", &speed) != TIXML_SUCCESS)
        throw new ArgumentException("En el nodo para Pathway_d no se encontro el atributo \"velocidad\"");

    if(elm->QueryIntAttribute("puntos", &length) != TIXML_SUCCESS)
        throw new ArgumentException("En el nodo para Pathway_d no se encontro el atributo \"puntos\"");

    TiXmlNode* p_node;
    int i = 0;
    points = new Vector3[length];
    for ( p_node = node->FirstChild(); p_node != 0; p_node = p_node->NextSibling()) 
    {
        if(i >= length)
            throw new ArgumentException("Hay mas puntos de los especificados en \"puntos\"");
        
        points[i++] = XML2VEC3(p_node);
        
    }
}



Player_d::Player_d(TiXmlNode* node){
    TiXmlElement *elm = node->ToElement();
    
    if(elm->QueryFloatAttribute("altura"   , &height)  != TIXML_SUCCESS) height  = DEFAULT_PLAYER_HEIGHT;

    if(elm->QueryFloatAttribute("disparo", &shoot) != TIXML_SUCCESS)
        throw new ArgumentException("En el nodo para Player_d no se encontro el atributo \"disparo\"");
    
    TiXmlNode *pathway_node = node->FirstChild("trayectoria");
    if(!pathway_node)
        throw new ArgumentException("En el nodo para Player_d no se encontro el hijo \"trayectoria\"");
    pathway = Pathway_d(pathway_node);
}



Enemys_d::Enemys_d(TiXmlNode* node){
    TiXmlElement *elm = node->ToElement();

    if(elm->QueryIntAttribute("cantidad", &length) != TIXML_SUCCESS)
        throw new ArgumentException("En el nodo para Enemys_d no se encontro el atributo \"cantidad\"");

    TiXmlNode* e_node;

    int i = 0;
    list = new Enemy_d[length];
    for ( e_node = node->FirstChild(); e_node != 0; e_node = e_node->NextSibling()) 
    {
        if(i >= length)
            throw new ArgumentException("Hay mas enemigos de los especificados en \"cantidad\"");
        
        list[i++] = Player_d(e_node);
    }
}


Obstacle_d::Type Obstacle_d::getType(const char *str) {
    if(strcmp(str, "esfera")==0)    return Obstacle_d::Sphere;
    else if(strcmp(str, "cubo")==0) return Obstacle_d::Cube;
    else if(strcmp(str, "maya")==0) return Obstacle_d::Mesh;
    else return (Obstacle_d::Type)-1;
}

Obstacle_d::Obstacle_d(TiXmlNode* node) {
    TiXmlNode* object = node->FirstChild();
    
    if(  !object
       || object->Type() != TiXmlNode::TINYXML_ELEMENT
       ||(type = getType(object->ToElement()->Value())) == -1
      )
        throw new ArgumentException("En el nodo para Obstacle_d no se encontraron los posibles hijos \"maya\" o \"esfera\" o \"cubo\"");
    
    switch(type){
        case Obstacle_d::Sphere:
        case Obstacle_d::Cube:
            mesh_path = NULL;
            break;
        case Obstacle_d::Mesh:
            const char *try_mesh = object->ToElement()->Attribute("archivo");
            if( !try_mesh )
                throw new ArgumentException("En el nodo para Obstacle_d (maya) no se encontro el atributo \"archivo\"");
            mesh_path = new char[strlen(try_mesh)];
            strcpy(mesh_path, try_mesh);
            break;
    }
    
    if(object->ToElement()->QueryFloatAttribute("tamano", &size) != TIXML_SUCCESS)
        size = 5.f;
    
    TiXmlNode* p_node = object->FirstChild();
    
    if(  !p_node 
       || p_node->Type() != TiXmlNode::TINYXML_ELEMENT 
       || strcmp(p_node->ToElement()->Value(), "punto") != 0
      )
        throw new ArgumentException("En el nodo interno a Obstacle_d no se encontro el hijo \"punto\"");
    
    position = XML2VEC3(p_node);    
}



Obstacles_d::Obstacles_d(TiXmlNode* node){
    TiXmlElement *elm = node->ToElement();

    if(elm->QueryIntAttribute("cantidad", &length) != TIXML_SUCCESS)
        throw new ArgumentException("En el nodo para Obstacles_d no se encontro el atributo \"cantidad\"");

    TiXmlNode* o_node;

    int i = 0;
    list = new Obstacle_d[length];
    for ( o_node = node->FirstChild(); o_node != 0; o_node = o_node->NextSibling()) 
    {
        if(i >= length)
            throw new ArgumentException("Hay mas obstaculos de los especificados en \"cantidad\"");
        
        list[i++] = Obstacle_d(o_node);
    }
}



Level_d::Level_d(TiXmlNode* node) {
    TiXmlElement *elm = node->ToElement();

    if(elm->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
        throw new ArgumentException("En el nodo para Level_d no se encontro el atributo \"id\"");

    if(elm->QueryIntAttribute("tiempo_juego", &game_time) != TIXML_SUCCESS)
        throw new ArgumentException("En el nodo para Level_d no se encontro el atributo \"tiempo_juego\"");

    if(elm->QueryIntAttribute("ancho"   , &width)  != TIXML_SUCCESS) width  = DEFAULT_LEVEL_WIDTH;
    if(elm->QueryIntAttribute("profundo", &deep)   != TIXML_SUCCESS) deep   = DEFAULT_LEVEL_DEEP;
    if(elm->QueryIntAttribute("alto"    , &height) != TIXML_SUCCESS) height = DEFAULT_LEVEL_HEIGHT;
    
    TiXmlNode *user_node = node->FirstChild("jugador");
    if(!user_node)
        throw new ArgumentException("En el nodo para Level_d no se encontro el hijo \"jugador\"");
    user = Player_d(user_node);

    TiXmlNode *enemys_node = node->FirstChild("contrincantes");
    if(!enemys_node)
        throw new ArgumentException("En el nodo para Level_d no se encontro el hijo \"contrincantes\"");
    enemys = Enemys_d(enemys_node);

    TiXmlNode *obstacles_node = node->FirstChild("objetos");
    if(!obstacles_node)
        throw new ArgumentException("En el nodo para Level_d no se encontro el hijo \"objetos\"");
    obstacles = Obstacles_d(obstacles_node);

}



Levels_d::Levels_d(TiXmlNode* node){
    TiXmlElement *elm = node->ToElement();

    if(elm->QueryIntAttribute("cantidad", &length) != TIXML_SUCCESS)
            throw new ArgumentException("En el nodo para Levels_d no se encontro el atributo \"cantidad\"");

    list = new Level_d[length];

    TiXmlNode* level_node;

    for ( level_node = node->FirstChild(); level_node != 0; level_node = level_node->NextSibling()) 
    {
        int index;
        if(level_node->ToElement()->QueryIntAttribute("id", &index) != TIXML_SUCCESS)
            throw new ArgumentException("En el nodo para Level_d no se encontro el atributo \"id\"");
        
        if(index > length || index < 1)
            throw new ArgumentException("El atributo \"id\" de Level_d esta fuera de los limites: [1, cantidad]");
        
        list[index-1] = Level_d(level_node);
    }
}