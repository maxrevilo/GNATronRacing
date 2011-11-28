#ifndef GAMEACTOR_H
#define	GAMEACTOR_H

#include "../GNAFramework/Game.h"


using namespace GNAFramework;

enum DrawOptions {
    Color  = 1,
    Bright = 2,
};

class GameActor {
public:
    Game *game;
    
    GameActor(Game *game) {
        this->game = game;
    }
    
    void Draw(GameTime gameTime, DrawOptions option);
    
    void Update(GameTime gameTime);
};


#endif	/* GAMEACTOR_H */

