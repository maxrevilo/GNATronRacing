#ifndef GAMEACTOR_H
#define	GAMEACTOR_H

#include "../GNAFramework/Game.h"
#include "../tinyxml/tinyxml.h"


using namespace GNAFramework;

enum DrawOptions {
    DrawColor  = 1,
    DrawBright = 2,
};

class GameActor {
public:
    Game *game;
    
    GameActor(Game *game) {
        this->game = game;
    }
    
    virtual void Draw(GameTime gameTime, DrawOptions option) = 0;
    
    virtual void Update(GameTime gameTime) = 0;
protected:
    static Vector3 XML2VEC3(TiXmlNode* node){
        double x = 0.0, y = 0.0, z = 0.0;
        TiXmlElement *elm = node->ToElement();

        elm->Attribute("x", &x); elm->Attribute("y", &y);elm->Attribute("z", &z);

        return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
    }
};


#endif	/* GAMEACTOR_H */

