#ifndef POSTPROCESS_H
#define	POSTPROCESS_H

#include "../GNAFramework/Game.h"
#include "../GNAFramework/GraphicDevice.h"
#include "../GNAFramework/RenderTarget2D.h"
#include "../GNAFramework/SpriteBatch.h"

using namespace GNAFramework;

class PostProcess {
public:
    bool active;
    
    PostProcess(Game *game);
    
    void Begin();
    
    void End();
    
    void textureScale(float value);
    void SampleNumber(int value);
    void BrightStrenght(float value);
    
    
    virtual ~PostProcess();
private:
    GraphicDevice *graphicDevice;
    RenderTarget2D *color, *luminance, *RTArray[2];
    SpriteBatch spriteBatch;
    
    
    ViewPort        savedVP;
    RenderTarget2D *halfBlur, **targes;
    Effect         *blurEffect, *printEffect;
    EffectParameter blurSource, texScale, samples, level, dir;
    EffectParameter brightStr, flare;
};

#endif	/* POSTPROCESS_H */

