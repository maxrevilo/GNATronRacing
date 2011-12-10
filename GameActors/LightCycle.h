#ifndef LIGHTCYCLE_H
#define	LIGHTCYCLE_H

#include "GameActor.h"
#include "../GNAFramework/Geom.h"
#include "../GNAFramework/Texture2D.h"
#include "../GNAFramework/Effect.h"
#include "../GNAFramework/EffectParameter.h"
#include "../GNAFramework/IndexBuffer.h"
#include "../GNAFramework/VertexBuffer.h"
#include "../GNAFramework/VertexDeclaration.h"
#include "Camera.h"
#include "Sphere_mesh.h"
#include "../GLM/Model3D.h"
#include "Trail.h"


class LightCycle : public GameActor {
public:
    
    LightCycle(Game *game, Camera *camera);
    
    virtual void Initialize(TiXmlNode *node);
    
    virtual void Draw(GameTime gameTime, DrawOptions option);
    
    void DrawTrail(DrawOptions option);
    
    virtual void Update(GameTime gameTime);
    
    Vector3 Position() const;
    
    void getFirstPersonView(Vector3 &camPos, Vector3 &camFoc, Vector3 &camUp) const;
    
    void getThirdPersonView(Vector3 &camPos, Vector3 &camFoc, Vector3 &camUp) const;
    
    void Turn(float ang);
    void Acelerate(bool acelerate);
    
    bool Alive() const;
    
    int Life() const { return life; }
    int Fuel() const { return fuel; }
    
    virtual ~LightCycle();
protected:
#define MESHES 8
    static Model3D *model;
    static EffectParameter modelWVPMat[MESHES];
    static EffectParameter modelWMat[MESHES];
    static EffectParameter modelCamPos[MESHES];
    static EffectParameter modelColor[MESHES];
    
    static EffectParameter modelMixVal[MESHES];
    static EffectParameter modelUVAdd[MESHES];
    
    Trail trail;
    
    
    //Graphic Debug:
    Sphere_mesh  sphmesh;
    Effect      *dbgEffect;
    EffectParameter dbgEffMatCamParam;
    
    
    Camera *camera;
    Vector4 color;
    float distance;
    
    Vector3 pos;
    Vector3 vel;
    
    Matrix rotation;
    
    float strength;
    float turnAng;
    bool turning;
    
    float fuel, life;
    bool acelerating;
    
    BoundingSphere area[4];
    
    
};

#endif	/* LIGHTCYCLE_H */

