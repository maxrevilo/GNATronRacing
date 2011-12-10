#ifndef EFFECTPARAMETER_H
#define	EFFECTPARAMETER_H

//#include "Effect.h"
#include "Texture2D.h"
#include "Geom.h"
#include "Effect.h"

namespace GNAFramework {
    class Effect;
    
    class EffectParameter {
        friend class Effect;
        public:
            
            Texture2D *getTexture();
            
            template <class T>
            void SetValue(const T *value);
            
            EffectParameter();
            EffectParameter(const EffectParameter& orig);
            
            virtual ~EffectParameter();
        private:
            EffectParameter(Effect *effect, const char *name);
            
            void activateEffect();
            void deactivateEffect();
            
            int texturePosition;
            Effect *effect;
            char *name;
            GLint location;
    };
}
#endif	/* EFFECTPARAMETER_H */

