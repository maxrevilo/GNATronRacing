#ifndef SCENARIO_H
#define	SCENARIO_H

#include "GameActor.h"
#include "../GNAFramework/Geom.h"
#include "../GNAFramework/Texture2D.h"
#include "../GNAFramework/Effect.h"
#include "../GNAFramework/EffectParameter.h"
#include "../GNAFramework/IndexBuffer.h"
#include "../GNAFramework/VertexBuffer.h"
#include "../GNAFramework/VertexDeclaration.h"
#include "Camera.h"

class Scenario : public GameActor {
public:
    int width, height;
    Camera *camera;
    
    
    Scenario(Game* game, Camera *camera);
    
    void Initialize(TiXmlNode *node);
    
    void Draw(GameTime gameTime, DrawOptions option);
    
    void Update(GameTime gameTime);
    
    virtual ~Scenario();
private:
    VertexDeclaration vd;
    IndexBuffer       ibo512;
    VertexBuffer      vbo512;
    
    
    //HeightMap array:
    struct Mesh{
        Vector3 position, dimension;
        Texture2D *heightMap, *colorMap, *normalMap;
        
        Mesh(); Mesh(TiXmlNode *node, ContentManager *cm);
        void Draw(Camera* camera, Matrix world, DrawOptions option);
        ~Mesh();
    } *meshes;
    int meshes_leght;
    
    
    struct Floor {
        Texture2D *texture;
        Effect    *effect;
        
        EffectParameter Ep_WVP;
        EffectParameter Ep_W;
        EffectParameter Ep_CamPos;
        
        Floor() {};
        Floor(Game* game);
        void Draw(Camera* camera, Matrix world, DrawOptions option);
    } floor;
    
};

#endif	/* SCENARIO_H */

