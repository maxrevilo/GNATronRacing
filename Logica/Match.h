#ifndef MATCH_H
#define	MATCH_H


#include "../GNAFramework/Game.h"
#include "../GNAFramework/SpriteBatch.h"
#include "../GNAFramework/SpriteFont.h"
#include "../InputManager.h"
#include "../Graficas/GraphicManager.h"
#include "MatchDescriptors.h"
#include "MatchActors.h"
#include "Level.h"
#include "Ray.h"

using namespace GNAFramework;

class GraphicManager;

#define MAX_DISCS 128

class Match {
    friend class GraphicManager;
public:
    struct {
        int level;
        
        struct {
            Ray   ray;
            float angL;
            float angV;
            Vector3 pos;
        } cam;
    } playerStats;
    
    bool pause;
    
    Obstacle *obstacles;
    Player   *players;
    bool     *teams;
    Disc      discs[MAX_DISCS];
    Plane     scenario[6];
    
    Match(Game *game);
    
    void Update(GameTime gameTime);
    
    void Draw(GameTime gameTime);
    
    Level_d *level();
    
    virtual ~Match();
private:
    enum _STATE{
        toLoad,
        Loading,
        Runing,
        Defeat,
        Victory
    };
    
    Game        *game;
    InputManager im;
    
    void winer(int i);
        
    _STATE match_state;
};

#endif	/* MATCH_H */

