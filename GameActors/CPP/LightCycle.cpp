#include <cstdlib>
#include <algorithm>

#include "../LightCycle.h"
#include "../../Managers/DebugManager.h"
#include "../../Managers/DataManager.h"

Model3D *LightCycle::model = NULL;
EffectParameter LightCycle::modelWVPMat[MESHES];
EffectParameter LightCycle::modelWMat[MESHES];
EffectParameter LightCycle::modelCamPos[MESHES];
EffectParameter LightCycle::modelColor[MESHES];

EffectParameter LightCycle::modelMixVal[MESHES];
EffectParameter LightCycle::modelUVAdd[MESHES];

LightCycle::LightCycle(Game *game, Camera *camera) 
      : GameActor(game),
        sphmesh(game->graphicDevice, 8, 16),
        trail(game, &color)
{
    this->camera = camera;

    
    // <editor-fold defaultstate="collapsed" desc="Area Setting:">
    area[0].R = 59.2 / 2.f;
    area[0].setCenter(Vector3(0.0f, -52.306f, 28.891f));

    area[1].R = 56.612 / 2.f;
    area[1].setCenter(Vector3(0.0f, -12.990f, 36.134f));

    area[2].R = 59.2 / 2.f;
    area[2].setCenter(Vector3(0.0f, 26.182f, 40.758f));

    area[3].R = 56.155 / 2.f;
    area[3].setCenter(Vector3(0.0f, 61.141f, 27.515f));
    // </editor-fold>
    
    
}

void LightCycle::Initialize(TiXmlNode* node) {
    if (node == NULL)
        throw new ArgumentException("player Tag coundn't be found. check the level's XML.");

    TiXmlElement *e = node->ToElement();
    

    if (e->QueryFloatAttribute("strength", &strength) != TIXML_SUCCESS) strength = 1.f;

    TiXmlNode *n;

    n = node->FirstChild("position");
    if (n == NULL) throw new ArgumentException("position Tag not found under enemy/player Tag. check the level's XML.");
    pos = XML2VEC3(n) * DataManager::unit_size;

    n = node->FirstChild("direction");
    if (n == NULL) throw new ArgumentException("direction Tag not found under enemy/player Tag. check the level's XML.");
    Vector3 dir = Vector3::Normalize(XML2VEC3(n));

    n = node->FirstChild("color");
    if (n == NULL) throw new ArgumentException("color Tag not found under enemy/player Tag. check the level's XML.");
    Vector3 cA = XML2VEC3(n);
    color = Vector4(cA.X, cA.Y, cA.Z, 1.f);
    
    
    // <editor-fold defaultstate="collapsed" desc="Load Model">
    if (model == NULL) {
        model = new Model3D(game->graphicDevice, "Content/Vehicles/LightCycle.obj");

        if (model->Meshes() - 1 != MESHES) {
            throw new GNAException("El modelo se cambio pero no se ajusto la cantidad e EffectParameter en LightCycle.");
        }

        char path[256];
        bool N2T;
        for (int i = 0; i < model->Meshes() - 1; i++) {
            const char *name = (*model)[i]->Name;
            Effect *effect;
            N2T = strcmp(name, "Tire") == 0 || strcmp(name, "Engine") == 0;

            if (N2T) {
                effect = game->Content->Load<Effect > ("Shaders/Neon2Tex.prog");

                modelMixVal[i] = effect->getParameter("mixVal");
                modelUVAdd[i] = effect->getParameter("uvAdd");
            } else {
                effect = game->Content->Load<Effect > ("Shaders/Neon.prog");
            }

            modelWVPMat[i] = effect->getParameter("rMatWorldViewProjection");
            modelWMat[i] = effect->getParameter("rMatWorld");
            modelCamPos[i] = effect->getParameter("rCamPos");
            modelColor[i] = effect->getParameter("Color");


            sprintf(path, "Vehicles/LightCycle/%s/DIFF.bmp", name);
            Texture2D *diffMap = game->Content->Load<Texture2D > (path);

            sprintf(path, "Vehicles/LightCycle/%s/EMSS.bmp", name);
            Texture2D *emssMap = game->Content->Load<Texture2D > (path);

            Vector4 color;
            float val = 8.f;
            color = Vector4(0.1f, 0.1f, 0.1f, 1.f);
            effect->getParameter("Ambient").SetValue(&color);
            effect->getParameter("SpecPow").SetValue(&val);

            effect->getParameter("rPLuz0Pos").SetValue(&DataManager::LightPos);

            color = Vector4(0.9f, 0.9f, 0.9f, 1.f);
            effect->getParameter("rPLuz0Col").SetValue(&color);

            effect->getParameter("diffMap").SetValue(diffMap);
            effect->getParameter("emssMap").SetValue(emssMap);

            (*model)[i]->effect = effect;
        }
    }
    // </editor-fold>
    
    
    if (DebugManager::graphic) {
        dbgEffect = game->Content->Load<Effect > ("Shaders/dbgEffect.prog");
        dbgEffect->Begin();
        dbgEffMatCamParam = dbgEffect->getParameter("rMatWorldViewProjection");
        dbgEffect->End();
    }


    vel = Vector3::Zero;
    rotation = Matrix::CreateWorld(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY)
            * Matrix::CreateWorld(Vector3::Zero, Vector3::UnitZ, dir);

    turnAng = 0.f;
    turning = false;
    fuel = 0.f;
    life = 100.f * strength;
    acelerating = false;
    distance = 0.f;


    if (DebugManager::graphic) {
        dbgEffect->Begin();
        Vector4 dbgColor = color;
        dbgColor.W = 0.6f;
        dbgEffect->getParameter("Color").SetValue<Vector4 > (&dbgColor);
        dbgEffect->End();
    }
}

void LightCycle::Update(GameTime gameTime) {
    float time = gameTime.elapsedTime.toSecondsF();

    fuel += DataManager::fuelRegeneration * time;

    // <editor-fold defaultstate="collapsed" desc="Turning">
    float abs_turnAng = fabs(turnAng);
    if (abs_turnAng > 0.0001f) {
        float sign_turnAng = turnAng > 0 ? 1.f : -1.f;
        if (abs_turnAng > DataManager::maxTurnRate)
            turnAng = sign_turnAng * DataManager::maxTurnRate;

        float resultAng = atan2(vel.Y, vel.X) + turnAng * time;

        Vector2 vecTurn = Vector2(cos(resultAng), sin(resultAng));
        Vector2 vecVel = Vector2(vel.X, vel.Y);

        vecVel = vecTurn * Vector2::Dot(vecVel, vecTurn);

        vel.X = vecVel.X;
        vel.Y = vecVel.Y;

        rotation.setUp(Vector3(vecTurn.X, vecTurn.Y, 0.f));
        rotation.setForward(Matrix::CreateRotationY(turnAng / 2.f) * Matrix::CreateRotationZ(PiOver2 - resultAng) * Vector3::UnitZ);
        rotation.setRight(Vector3::Cross(rotation.Up(), rotation.Forward()));


        if (!turning) {
            turnAng -= sign_turnAng * std::min(DataManager::turnSlowRate * time, sign_turnAng * turnAng);
        }
        turning = false;
    } else turnAng = 0.f;
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Aceleration">
    if (acelerating && fuel > 0.f) {
        vel += rotation.Up() * DataManager::turboAceleration * time;

        fuel = std::max(0.f, fuel - DataManager::fuelConsumption * time);

        float velL = vel.Length();

        if (velL > DataManager::turboSpeed) {
            vel += Vector3::Normalize(vel) * (DataManager::turboSpeed - velL);
        }
    } else {
        vel += rotation.Up() * DataManager::aceleration * time;

        fuel = std::min(100.f, fuel + DataManager::fuelRegeneration * time);

        float velL = vel.Length();

        if (velL > 0.9f * DataManager::speed) {
            vel += Vector3::Normalize(vel) * (DataManager::speed - velL);
        }
    }
    // </editor-fold>
    
    
    pos += vel * time;
    
    float mVel = (vel * time).Length();
    distance += mVel;
    
    trail.Update(Matrix::CreateWorld(pos, rotation.Forward(), rotation.Up()));
}

void LightCycle::Draw(GameTime gameTime, DrawOptions option) {
    float time = gameTime.elapsedTime.toSecondsF();
    
    if (option & DrawColor) {

        if (DebugManager::graphic) {
            game->graphicDevice->setBlendState(BlendState::NonPremultiplied);
            dbgEffect->Begin();
            Matrix posVP = rotation * Matrix::CreateTranslation(pos) * camera->viewMatrix() * camera->projectionMatrix();
            for (int i = 0; i < 4; i++) {
                Matrix wvp = Matrix::CreateScale(area[i].R) * Matrix::CreateTranslation(area[i].Center()) * posVP;

                dbgEffMatCamParam.SetValue<Matrix > (&wvp);
                sphmesh.Draw();
            }
            dbgEffect->End();
        }
        
        
        float mixVal = (vel.Length()/DataManager::turboSpeed);
        Vector2 uvAdd = Vector2(0.0, -1.0) * distance / 300.f;

        game->graphicDevice->setBlendState(BlendState::Opaque);
        game->graphicDevice->setRasterizerState(RasterizerState::CullClockwise);

        Matrix reorient = Matrix::Zeros;
        reorient.M13(1.f);
        reorient.M21(1.f);
        reorient.M32(1.f);
        reorient.M44(1.f);

        Matrix World = reorient * rotation * Matrix::CreateTranslation(pos);
        Matrix WVP = World * camera->viewMatrix() * camera->projectionMatrix();
        
        Vector3 pos = camera->getPosition();
        
        bool N2T;
        for (int i = 0; i < model->Meshes() - 1; i++) {
            const char *name = (*model)[i]->Name;
            N2T = strcmp(name, "Tire") == 0 || strcmp(name, "Engine") == 0;

            if (N2T) {
                modelMixVal[i].SetValue(&mixVal);
                modelUVAdd[i].SetValue(&uvAdd);
            }
            
            modelWVPMat[i].SetValue(&WVP);
            modelWMat[i].SetValue(&World);
            modelCamPos[i].SetValue(&pos);
            modelColor[i].SetValue(&color);
            
            if(!N2T && strcmp(name, "Cristal") == 0) continue;
            
            (*model)[i]->Draw();
        }
        
        game->graphicDevice->setBlendState(BlendState::NonPremultiplied);
        (*model)["Cristal"]->Draw();
        game->graphicDevice->setBlendState(BlendState::Opaque);
        
        
        game->graphicDevice->setRasterizerState(RasterizerState::CullCounterClockwise);

    }
}

void LightCycle::DrawTrail(DrawOptions option) {
    trail.Draw(camera);
}


bool LightCycle::Alive() const {
    return life > 0.f;
}

void LightCycle::Turn(float ang) {
    turnAng += ang;
    turning = true;
}

void LightCycle::Acelerate(bool acelerate) {
    acelerating = acelerate;
}

Vector3 LightCycle::Position() const { return pos; }

void LightCycle::getFirstPersonView(Vector3 &camPos, Vector3 &camFoc, Vector3 &camUp) const {
    camPos = Vector3(0.0f, 39.458f, 57.843f);
    camFoc = Vector3(0.0f, 365.58f, 29.695f);

    Matrix world = rotation * Matrix::CreateTranslation(pos);

    camPos = world * camPos;
    camFoc = world * camFoc;
    camUp = rotation.Up();
}

void LightCycle::getThirdPersonView(Vector3 &camPos, Vector3 &camFoc, Vector3 &camUp) const {
    camPos = Vector3(0.f, -300.f, 180.f);
    camFoc = Vector3(0.0f, 300.f, 0.f);

    Matrix world = rotation * Matrix::CreateTranslation(pos);

    camPos = world * camPos;
    camFoc = world * camFoc;
    camUp = Vector3::UnitZ;
}

LightCycle::~LightCycle() {
}

