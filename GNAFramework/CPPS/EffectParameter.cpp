#include "../EffectParameter.h"
#include <string.h>
#include "../Texture2D.h"
#include "AdaptacionesDeFunciones.h"
#include <limits.h>

using namespace GNAFramework;

Texture2D *EffectParameter::getTexture() {
    return NULL;
}

EffectParameter::EffectParameter(Effect *effect, const char *name) {
    
    int nLenght = strlen(name);
    
    if(nLenght > 31) 
        throw ArgumentOutOfRangeException("The EffectParameter name is too large.");
    
    strcpy(this->name, name);
    this->name[nLenght] = '\0';
    
    this->effect = effect;
    location = glGetUniformLocationARB(effect->program, name);
    texturePosition = -1;
}

EffectParameter::EffectParameter() {
    location = INT_MAX;
    name[0] = '\0';
    texturePosition = -1;
}

void EffectParameter::activateEffect() {
    if(!effect) 
        throw InvalidOperationException("This effect parameter is not Initialized.");
    
    if(!effect->begin) {
        if(Effect::effectStarted)
            throw InvalidOperationException("Cannot use a Parameter while other effect has began.");
        
        glUseProgramObjectARB(effect->program);
    }
}

void EffectParameter::deactivateEffect() {
    if(!effect) 
        throw InvalidOperationException("This effect parameter is not Initialized.");
    
    if(!effect->begin) {
        if(Effect::effectStarted)
            throw InvalidOperationException("Cannot use a Parameter while other effect has began.");
        
        glUseProgramObjectARB(0);
    }
}


EffectParameter::~EffectParameter() {
}




namespace GNAFramework {

    template <>
    void EffectParameter::SetValue<Texture2D>(const Texture2D *value) {
        activateEffect();
        
        if (texturePosition < 0) {
            int i;
            for (i = 0; i < effect->texPosSize; i++) {
                if (effect->texPos[i].parameter == location) break;
            }
            
            if (i == effect->texPosSize) {
                if (effect->texPosSize >= 17) {
                    throw InvalidOperationException("Texture limit per effect reached: 17.");
                }

                texturePosition = effect->texPosSize++;
                effect->texPos[texturePosition].parameter = location;
            } else {
                texturePosition = i;
            }
        }

        effect->texPos[texturePosition].texture = value;
        
        deactivateEffect();
    }

    template <>
    void EffectParameter::SetValue<int>(const int *value) {
        activateEffect();
        glUniform1iARB(location, *value);
        deactivateEffect();
    }

    template <>
    void EffectParameter::SetValue<float>(const float *value) {
        activateEffect();
        glUniform1fARB(location, *value);
        deactivateEffect();
    }

    template <>
    void EffectParameter::SetValue<Vector2>(const Vector2 *value) {
        activateEffect();
        glUniform2fARB(location, value->X, value->Y);
        deactivateEffect();
    }

    template <>
    void EffectParameter::SetValue<Vector3>(const Vector3 *value) {
        activateEffect();
        glUniform3fARB(location, value->X, value->Y, value->Z);
        deactivateEffect();
    }

    template <>
    void EffectParameter::SetValue<Vector4>(const Vector4 *value) {
        activateEffect();
        glUniform4fARB(location, value->X, value->Y, value->Z, value->W);
        deactivateEffect();
    }

    template <>
    void EffectParameter::SetValue<Color>(const Color *value) {
        Vector4 vec = value->toVector4();
        SetValue<Vector4 > (&vec);
    }

    template <>
    void EffectParameter::SetValue<Matrix>(const Matrix *value) {
        activateEffect();
        float values[16] = {
            value->M11(), value->M12(), value->M13(), value->M14(),
            value->M21(), value->M22(), value->M23(), value->M24(),
            value->M31(), value->M32(), value->M33(), value->M34(),
            value->M41(), value->M42(), value->M43(), value->M44(),
        };

        glUniformMatrix4fvARB(location, 1, (GLboolean) true, values);
        deactivateEffect();
    }

}

