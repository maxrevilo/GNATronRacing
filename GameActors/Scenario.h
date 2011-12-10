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
#include "../GLM/Model3D.h"

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
    Model3D *skyMap;
    EffectParameter skyMapWVPMat;
    
    Model3D *model;
    EffectParameter modelWVPMat[6];
    
    
    struct Floor {
        Effect    *effect;
        
        VertexDeclaration vd;
        IndexBuffer       ibo;
        VertexBuffer      vbo;
        
        EffectParameter Ep_WVP;
        EffectParameter Ep_W;
        EffectParameter Ep_CamPos;
        
        Floor() {};
        Floor(Game* game);
        void Draw(Camera* camera, Matrix world, GraphicDevice *gd, DrawOptions option);
        
    } floor;
    
    //HeightMap array:
    struct Mesh{
    public:
        static void initializeBuffers(GraphicDevice *gd);
        
        Mesh(); Mesh(TiXmlNode *node, ContentManager *cm);
        void Draw(Camera* camera, GraphicDevice *gd, DrawOptions option);
        ~Mesh();
    private:
        static VertexDeclaration vd;
        static IndexBuffer       ibo512;
        static VertexBuffer      Svbo512;
        
        VertexBuffer             vbo512;
        
        Effect    *effect;
        Vector3    position, dimension;
        Texture2D *heightMap, *colorMap, *normalMap;
    } *meshes;
    int meshes_length;
    
};

#endif	/* SCENARIO_H */

