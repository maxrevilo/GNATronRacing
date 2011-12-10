#include "../Enemy.h"
#include "../../Managers/DataManager.h"
#include "../../Managers/DebugManager.h"
#include <algorithm>
#include <math.h>


Enemy::Enemy(Game *game, Camera *camera) : LightCycle(game, camera) {
    patrolPoint = -1;
}

void Enemy::Initialize(TiXmlNode *node) {
    LightCycle::Initialize(node);
    
    TiXmlNode *paNode = node->ToElement()->FirstChild("patrol");
    
    patrolPoint = -1;
    patrol.clear();
    
    if(paNode != NULL) {
        TiXmlElement *elm = paNode->ToElement();
    
        /*
        int length;
        if(elm->QueryIntAttribute("length", &length) != TIXML_SUCCESS)
            throw new ArgumentException("\"length\" attribute couldn't be found on enemy->patrol tag. check the level's XML.");
        */

        TiXmlNode* p_node;
        for ( p_node = elm->FirstChild(); p_node != 0; p_node = p_node->NextSibling()) 
        {
            if(p_node != NULL & p_node->ToElement() != NULL) {
                Vector3 p = XML2VEC3(p_node) * DataManager::unit_size;
                patrol.push_back(p);
                
                //There is at least one point:
                patrolPoint = 0;
            }
        }
    }
}

void Enemy::Update(GameTime gameTime) {
    float time = gameTime.elapsedTime.toSecondsF();
    
    if(patrolPoint >= 0) {
        Vector3 p = patrol[patrolPoint];
        Vector3 d = p - pos;
        float distSq = d.LengthSquared();
        
        if(distSq <= 250.f*250.f) {
            patrolPoint = (patrolPoint + 1) % patrol.size();
        }
        
        Vector3 dir = rotation.Up();
        float angD = atan2(d.Y, d.X);
        float angDir = atan2(dir.Y, dir.X);
        
        if(angD < 0.f) angD += TwoPi;
        if(angDir < 0.f) angDir += TwoPi;
        
        float dAng = angD - angDir;
        if(dAng > Pi) dAng = dAng - TwoPi;
        
        float sign = dAng>0 ? 1.f : -1.f;
        
        if(fabs(turnAng / DataManager::turnSlowRate) < fabs(sign * dAng / turnAng)) {
            Turn(sign * std::min(sign * dAng, DataManager::turnRate) * time);
        }
        
    } else {
        Turn(DataManager::turnRate * time);
    }
    
    LightCycle::Update(gameTime);
}


void Enemy::Draw(GameTime gameTime, DrawOptions option) {
    LightCycle::Draw(gameTime, option);
    
    if(option & DrawColor) {
        
        if(DebugManager::graphic) {
            game->graphicDevice->setBlendState(BlendState::NonPremultiplied);
            dbgEffect->Begin();
            Matrix posVP    = camera->viewMatrix() * camera->projectionMatrix();
            for(int i = 0; i < patrol.size(); i++) {
                Matrix wvp   = Matrix::CreateScale(250.f) *Matrix::CreateTranslation(patrol[i]) * posVP;
                
                dbgEffMatCamParam.SetValue<Matrix>(&wvp);
                sphmesh.Draw();
            }
            dbgEffect->End();
            game->graphicDevice->setBlendState(BlendState::Opaque);
        }
    }
}

Enemy::~Enemy() {
}

