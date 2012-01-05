#ifndef ANAMORPHICBLOOM_H
#define	ANAMORPHICBLOOM_H

#include "../GNAFramework/RenderTarget2D.h"
#include "../GNAFramework/Game.h"
#include "../GNAFramework/Effect.h"

using namespace GNAFramework;

class AnamorphicBloom {
public:
    AnamorphicBloom(
            Game *game,
            const char *effectsPath,
            int width, 
            int height, 
            int sampleNumber = 20,
            bool vertical = false,
            int nTargets = 4
            );
    
    void minGaussStrenght(float value);
    void textureScale(float value);
    void SampleNumber(int value);
    void BrightStrenght(float value);
    
    
    Texture2D *getCaputedImage();
    
    void Begin();
    
    void End();
    
    virtual ~AnamorphicBloom();
private:
    Game            *game;
    RenderTarget2D  *scene;
    int              targetNumber;
    RenderTarget2D **targes;
    Effect          *blurEffect;
    Effect          *printEffect;
    Texture2D       *anamorphicFun;
    bool             vertical;
    ViewPort         savedVP;
    
    static bool SBegan;
    
    
    //Effect Params
    EffectParameter minGasussStr;
    EffectParameter texScale;
    EffectParameter samples;
    EffectParameter level;
    
    EffectParameter brightStr;
    
};
#endif	/* ANAMORPHICBLOOM_H */

