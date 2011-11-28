#ifndef EFFECT_H
#define	EFFECT_H

#include "GraphicDevice.h"
#include "EffectParameter.h"

#define MAX_PARMETERS 32

namespace GNAFramework {
    //class EffectParameter;
    
    class Effect {
    public:
        Effect(const Effect *clone);
        Effect(GraphicDevice *grapchicsDevice, const char *progCode);
        
        void Begin();
        void End();
        
        void Apply();
        
        EffectParameter getParameter(const char *name);
        
        Effect *Clone();
        
        virtual ~Effect();
    private:
        GraphicDevice *grapchicsDevice;
        int lastParameterIndex;
        //EffectParameterCollection parameters;
        
        int             texPos[MAX_PARMETERS];
        EffectParameter parameters[MAX_PARMETERS];
        
        GLenum vert;
        GLenum frag;
        GLenum program;
        
        void instanciar();
        EffectParameter *searchParameter(GLint location);
    };
}

#endif	/* EFFECT_H */

