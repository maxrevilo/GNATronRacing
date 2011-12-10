#ifndef EFFECT_H
#define	EFFECT_H

#include "GraphicDevice.h"
#include "EffectParameter.h"
#include "Texture2D.h"

#define MAX_PARMETERS 32

namespace GNAFramework {
    class EffectParameter;
    
    class Effect {
        friend class EffectParameter;
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
        static bool effectStarted;
        bool begin;
        
        //EffectParameterCollection parameters;
        
        struct TexturePosition{
            const Texture2D *texture;
            int        parameter;
        } texPos[17];
        int texPosSize;
        
        GLenum vert;
        GLenum frag;
        GLenum program;
        
        void instanciar();
    };
}

#endif	/* EFFECT_H */

