#ifndef EFFECTPARAMETER_H
#define	EFFECTPARAMETER_H

//#include "Effect.h"
#include "Texture2D.h"
#include "Geom.h"

namespace GNAFramework {
    class Effect;
    
    class EffectParameter {
        friend class Effect;
        public:
            
            Texture2D *getTexture();
            
            template <class T>
            void SetValue(const T *value);
            
            virtual ~EffectParameter();
        private:
            EffectParameter();
            EffectParameter(GLint ubicacion, const char *nombre, int *actualTexturePosition);
            static int texturePosition;
            int *actualTexturePosition;
            
            char *name;
            GLint location;
    };
}
#endif	/* EFFECTPARAMETER_H */

