#ifndef ENEMY_H
#define	ENEMY_H

#include "LightCycle.h"
#include <vector>

class Enemy : public LightCycle {
public:
    Enemy(Game *game, Camera *camera);
    
    void Initialize(TiXmlNode *node);
    
    void Update(GameTime gameTime);
    
    void Draw(GameTime gameTime, DrawOptions option);
    
    virtual ~Enemy();
private:
    std::vector<Vector3> patrol;
    int patrolPoint;
};

#endif	/* ENEMY_H */

