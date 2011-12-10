#include "../Trail.h"
#include <math.h>
#include "../../GNAFramework/MathHelper.h"

Model3D::Mesh *Trail::iniTrail = NULL;
Model3D::Mesh *Trail::midTrail = NULL;
EffectParameter  Trail::meshWVPMat;
EffectParameter  Trail::meshColor;


Trail::Trail(Game *game, Vector4 *color) {
    this->game = game;
    this->color = color;
    lastTrail = 0;
    
    for(int i = 0; i < MAXTRAIL; i++) lenghts[i] = 0.f;
    
    if(iniTrail == NULL) {
        Model3D *model = new Model3D(game->graphicDevice, "Content/Vehicles/Trail.obj");
        
        if (model->Meshes() - 1 != 2) {
            throw new GNAException("El modelo se cambio pero no se ajusto la cantidad e EffectParameter en Trail.");
        }
        iniTrail = (*model)["F"];
        midTrail = (*model)["S"];
        
        Effect *effect = game->Content->Load<Effect>("Shaders/trail.prog");
        
        iniTrail->effect = effect;
        midTrail->effect = effect;
        
        Texture2D *diffMap = game->Content->Load<Texture2D > ("Vehicles/trailb.bmp");
        
        effect->getParameter("diffMap").SetValue(diffMap);
        
        meshWVPMat = effect->getParameter("rMatWorldViewProjection");
        meshColor  = effect->getParameter("color");
        
        
        delete model;
    }
}

void Trail::Update(Matrix world, float length) {
    this->world[lastTrail]   = world;
    this->lenghts[lastTrail] = length;
    
    lastTrail = (lastTrail + 1) % MAXTRAIL;
    
}

void Trail::Draw(Camera *camera) {
    Matrix reorient = Matrix::Zeros;
    reorient.M13(1.f);
    reorient.M21(1.f);
    reorient.M32(1.f);
    reorient.M44(1.f);
    
    meshColor.SetValue(color);
    
    Matrix vp = camera->viewMatrix() * camera->projectionMatrix();
    int firstTrail = (MAXTRAIL + lastTrail - 1) % MAXTRAIL;
    
    Matrix wvp = reorient * world[firstTrail] * vp;
    
    float cos80 = cos(80.f/180.f*Pi);
    Vector3 pos, camPos = camera->getFocus() - camera->getPosition();
    
    meshWVPMat.SetValue(&wvp);
    iniTrail->Draw();
    
    reorient = Matrix::CreateTranslation(-109.621, 0.f, 0.f) * reorient;
    
    for(int i = 0; i < MAXTRAIL; i++) {
        if(i != lastTrail && lenghts[i] > 0.9) {
            pos = world[i].Translation() - camera->getPosition();
            
            if(Vector3::Dot(pos, camPos) > 0.f) {
                wvp = Matrix::CreateScale(lenghts[i], 1.f, 1.f) * reorient * world[i] * vp;
                meshWVPMat.SetValue(&wvp);
                midTrail->Draw();
            }
        }
    }
    
    
    
}

Trail::~Trail() {
}

