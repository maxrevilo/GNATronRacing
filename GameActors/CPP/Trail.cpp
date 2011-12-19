#include "../Trail.h"
#include <math.h>
#include "../../GNAFramework/MathHelper.h"
#include "GL/gl.h"
#include <limits.h>

Model3D::Mesh *Trail::iniTrail = NULL;
Model3D::Mesh *Trail::midTrail = NULL;
EffectParameter Trail::meshWVPMat;
EffectParameter Trail::meshColor;

Trail::Trail(Game *game, Vector4 *color) {
    this->game = game;
    this->color = color;
    lastTrail = 0;
    ini = false;

    if (iniTrail == NULL) {
        Model3D *model = new Model3D(game->graphicDevice, "Content/Vehicles/Trail.obj");

        if (model->Meshes() - 1 != 2) {
            throw new GNAException("El modelo se cambio pero no se ajusto la cantidad e EffectParameter en Trail.");
        }
        iniTrail = (*model)["F"];
        midTrail = (*model)["S"];

        Effect *effect = game->Content->Load<Effect > ("Shaders/trail.prog");

        iniTrail->effect = effect;
        midTrail->effect = effect;

        Texture2D *diffMap = game->Content->Load<Texture2D > ("Vehicles/trailb.bmp");

        effect->getParameter("diffMap").SetValue(diffMap);

        meshWVPMat = effect->getParameter("rMatWorldViewProjection");
        meshColor = effect->getParameter("color");


        delete model;
    }
}

void Trail::Update(Matrix world) {
    lastPosWorld = world;
    
    upTrailPos[lastTrail] = world * Vector3(0.f, -109.64f, 61.258f);
    loTrailPos[lastTrail] = world * Vector3(0.f, -109.64f, 0.f);
    
    if (!ini) {
        for (int i = 0; i < MAXTRAIL; i++) {
            upTrailPos[i] = upTrailPos[lastTrail];
            loTrailPos[i] = loTrailPos[lastTrail];
        }
        ini = true;
    }

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

    Matrix wvp = reorient * lastPosWorld * vp;

    meshWVPMat.SetValue(&wvp);
    iniTrail->Draw();


    midTrail->effect->Begin();
    meshWVPMat.SetValue(&vp);
    glBegin(GL_QUADS);
    for (int i = 0; i < MAXTRAIL; i++) {
        int i2 = (MAXTRAIL + i + 1) % MAXTRAIL;
        if (i2 != lastTrail) {

            glVertex3fv((float *) &upTrailPos[i]);
            glTexCoord2f(0.18f, 0.f);

            glVertex3fv((float *) &loTrailPos[i]);
            glTexCoord2f(0.2f, 0.f);

            glVertex3fv((float *) &loTrailPos[i2]);
            glTexCoord2f(0.2f, 1.f);

            glVertex3fv((float *) &upTrailPos[i2]);
            glTexCoord2f(0.18f, 1.f);
        }
    }
    glEnd();
    midTrail->effect->End();
}

Trail::~Trail() {
}

