#ifndef GRAPHICMANAGER_H
#define	GRAPHICMANAGER_H

#include "../GNAFramework/Game.h"
#include "../GNAFramework/SpriteBatch.h"
#include "../GNAFramework/SpriteFont.h"
#include "../GNAFramework/Geom.h"
#include "../GNAFramework/Effect.h"

#include "../Logica/Match.h"
#include "../DataManager.h"
#include "Camera.h"
#include "Cube_mesh.h"
#include "Sphere_mesh.h"

using namespace GNAFramework;

class Match;

class GraphicManager {
    friend class Match;
public:
    
    static void Initialize(Game *game){
        instance = new GraphicManager(game);
    }
    
    static GraphicManager *get(){
        return instance;
    }
    
    void DrawMatch(Match *match);
    
    void DrawMatchLoading();
    
    void DrawMatchDefeat();
    
    void DrawMatchVictory();
      
private:
    static GraphicManager *instance;
    
    Game *game;
    SpriteBatch *spriteBatch;
    Cube_mesh   *cube;
    Sphere_mesh *sphere;
    
    struct {
        SpriteFont *title;
        SpriteFont *title2;
        SpriteFont *text;
    } font;
    
    
    struct {
        bool ligth_load;
        bool heavy_load;
    } loads;
    
    Camera camera;
    
    Texture2D *loading_tex;
    
    struct {
        Effect *effect;
        Texture2D *floor_tex;
        Texture2D *cristal_tex;
        Texture2D *ambient;
    } scenario;
    
    Effect *cube_effect;
    Effect *player_effect;
    
    Effect *flecha_eff;
    Texture2D *flecha_tex;
    
    int obst_tex_length;
    struct _OBST_TEX{
        Texture2D *texture;
    } *obst_tex;
    
    
    
    GraphicManager(Game *game);
    
    void ligthLoad();
    
    void heavyLoad();
    
    void DrawLine(Vector3 v0, Vector3 v1);
};

#endif	/* GRAPHICMANAGER_H */

