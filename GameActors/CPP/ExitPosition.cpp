#include "../ExitPosition.h"

ExitPosition::ExitPosition(Game *game) : GameActor(game) {
    
}

void ExitPosition::Initialize(TiXmlNode *node) {
    if(node == NULL)
        throw new ArgumentException("exit_position Tag coundn't be found. check the level's XML.");
    
    position = XML2VEC3(node);
}
    
void ExitPosition::Draw(GameTime gameTime, DrawOptions option) {
    
}
    
void ExitPosition::Update(GameTime gameTime) {
    
}

ExitPosition::~ExitPosition() {
    
}

