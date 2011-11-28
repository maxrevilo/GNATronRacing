#ifndef SCENARIO_H
#define	SCENARIO_H

#include "GameActor.h"

class Scenario : public GameActor {
public:
    Scenario(Game* game);
    
    void Draw(GameTime gameTime, DrawOptions option);
    
    void Update(GameTime gameTime);
    
    virtual ~Scenario();
private:
};

#endif	/* SCENARIO_H */

