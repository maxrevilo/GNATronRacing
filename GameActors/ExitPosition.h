#ifndef EXITPOSITION_H
#define	EXITPOSITION_H
#include "GameActor.h"


class ExitPosition : public GameActor{
public:
    Vector3 position;
    
    ExitPosition(Game *game);
    
    virtual void Initialize(TiXmlNode *node);
    
    virtual void Draw(GameTime gameTime, DrawOptions option);
    
    virtual void Update(GameTime gameTime);
    
    virtual ~ExitPosition();
private:

};

#endif	/* EXITPOSITION_H */

