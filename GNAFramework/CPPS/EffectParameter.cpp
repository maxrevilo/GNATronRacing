#include "../EffectParameter.h"
#include <string.h>
#include "../Texture2D.h"
#include "AdaptacionesDeFunciones.h"

using namespace GNAFramework;


int EffectParameter::texturePosition = 0;

GLenum getGLTexturePosition(int pos){
    switch(pos){
        case 0 : return GL_TEXTURE0_ARB;
        case 1 : return GL_TEXTURE1_ARB;
        case 2 : return GL_TEXTURE2_ARB;
        case 3 : return GL_TEXTURE3_ARB;
        case 4 : return GL_TEXTURE4_ARB;
        case 5 : return GL_TEXTURE5_ARB;
        case 6 : return GL_TEXTURE6_ARB;
        case 7 : return GL_TEXTURE7_ARB;
        case 8 : return GL_TEXTURE8_ARB;
        case 9 : return GL_TEXTURE9_ARB;
        case 10: return GL_TEXTURE10_ARB;
        case 11: return GL_TEXTURE11_ARB;
        case 12: return GL_TEXTURE12_ARB;
        case 13: return GL_TEXTURE13_ARB;
        case 14: return GL_TEXTURE14_ARB;
        case 15: return GL_TEXTURE15_ARB;
        case 16: return GL_TEXTURE16_ARB;
    }
}

Texture2D *EffectParameter::getTexture(){
    return NULL;
}

EffectParameter::EffectParameter(GLint ubicacion, const char *nombre, int *actualTexturePosition) {
    this->location = ubicacion;
    
    this->actualTexturePosition = actualTexturePosition;
    *(this->actualTexturePosition) = -1;
    
    this->name = new char[strlen(nombre)];
    strcpy(this->name, nombre);
}

EffectParameter::EffectParameter(){
    this->location= -2;
    this->name = NULL;
    actualTexturePosition = NULL;
}


EffectParameter::~EffectParameter() {
}

namespace GNAFramework {
    
    template <>
    void EffectParameter::SetValue<Texture2D>(const Texture2D *value){
        //printf("Look texture %d on %d effect:%s\n", *actualTexturePosition, actualTexturePosition, name);fflush(stdout);
        if(*actualTexturePosition < 0) {
           *actualTexturePosition = EffectParameter::texturePosition++;
        }
        
        glActiveTextureARB(getGLTexturePosition(*actualTexturePosition));
        glBindTexture(GL_TEXTURE_2D,value->Pointer());
        
        //printf("Set texture %d on %d effect:%s\n", *actualTexturePosition, actualTexturePosition, name);fflush(stdout);
        glUniform1iARB(location, *actualTexturePosition);
    }
    
    template <>
    void EffectParameter::SetValue<float>(const float *value){
        glUniform1fARB(location, *value);
    }
    
    template <>
    void EffectParameter::SetValue<Vector2>(const Vector2 *value){
        glUniform2fARB(location, value->X, value->Y);
    }
    
    template <>
    void EffectParameter::SetValue<Vector3>(const Vector3 *value){
        glUniform3fARB(location, value->X, value->Y, value->Z);
    }
    
    template <>
    void EffectParameter::SetValue<Vector4>(const Vector4 *value){
        glUniform4fARB(location, value->X, value->Y, value->Z, value->W);
    }
    
    template <>
    void EffectParameter::SetValue<Matrix>(const Matrix *value){
        fflush(stdout);
        float values[16] = {
            value->M11(), value->M12(), value->M13(), value->M14(),
            value->M21(), value->M22(), value->M23(), value->M24(), 
            value->M31(), value->M32(), value->M33(), value->M34(), 
            value->M41(), value->M42(), value->M43(), value->M44(), 
        };
        
        glUniformMatrix4fvARB(location, 1, (GLboolean) true, values);
    }
    
}

