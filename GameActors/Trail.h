#ifndef TRAIL_H
#define	TRAIL_H

#include "../GLM/Model3D.h"
#include "../GNAFramework/Geom.h"
#include "../GNAFramework/Color.h"
#include "../GNAFramework/Effect.h"
#include "../GNAFramework/Game.h"
#include "Camera.h"

#define MAXTRAIL 240

class Trail {
public:
    Trail(Game *game, Vector4 *color);
    
    void Update(Matrix world, float length);
    
    void Draw(Camera *camera);
    
    virtual ~Trail();
private:
    static Model3D::Mesh *iniTrail;
    static Model3D::Mesh *midTrail;
    static EffectParameter meshWVPMat;
    static EffectParameter meshColor;
    
    Game *game;
    Vector4 *color;
    
    int lastTrail, firstTrail;
    Matrix world[MAXTRAIL];
    float  lenghts[MAXTRAIL];
};

#endif	/* TRAIL_H */

