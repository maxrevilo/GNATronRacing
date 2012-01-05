#ifndef GAUSSIANBLUR_H
#define	GAUSSIANBLUR_H


#include "../GNAFramework/RenderTarget2D.h"
#include "../GNAFramework/Game.h"
#include "../GNAFramework/Effect.h"
#include "../GNAFramework/Texture2D.h"


using namespace GNAFramework;

class GaussianBlur {
public:
    GaussianBlur(
            Game *game,
            const char *effectsPath,
            Texture2D *source,
            int samplesNumber = 5,
            int divFactor = 1);
    
    void minGaussStrenght(float value);
    void textureScale(float value);
    void SampleNumber(int value);
    void BrightStrenght(float value);
    
    void Apply();
    
    virtual ~GaussianBlur();
private:
    Game           *game;
    Texture2D      *source;
    Texture2D      *gaussFun;
    RenderTarget2D *halfBlur;
    RenderTarget2D **targes;
    Effect         *blurEffect;
    Effect         *printEffect;
    ViewPort        savedVP;
    
    EffectParameter blurSource;
    EffectParameter minGasussStr;
    EffectParameter texScale;
    EffectParameter samples;
    EffectParameter level;
    EffectParameter dir;
    
    EffectParameter brightStr;
    EffectParameter flare;
};

#endif	/* GAUSSIANBLUR_H */

