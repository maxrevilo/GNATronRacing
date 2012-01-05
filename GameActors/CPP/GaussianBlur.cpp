#include "../GaussianBlur.h"

#define TARGET_NUMBER 3

GaussianBlur::GaussianBlur(
            Game *game,
            const char *effectsPath,
            Texture2D *source,
            int samplesNumber,
            int divFactor) {
    this->game = game;
    this->source = source;
    
    int width = source->getWidth(), height = source->getHeight();
    
    halfBlur = new RenderTarget2D(game->graphicDevice, width, height, true, BGRA,
                (RenderTarget2D::DepthFormat) 0, 0, (RenderTarget2D::RenderTargetUsage)0);
    halfBlur->setSamplerAddressUV(Clamp);
    
    targes  = new RenderTarget2D*[TARGET_NUMBER];
    int div = divFactor;
    
    for(int i = 0; i < TARGET_NUMBER; i ++) {
        targes[i] =
                new RenderTarget2D(game->graphicDevice, width/div, height/div, false, BGRA,
                (RenderTarget2D::DepthFormat) 0, 0, (RenderTarget2D::RenderTargetUsage)0);
        targes[i]->setSamplerAddressUV(Clamp);
        div *= 2;
    }
    
    
    char path[256];
    
    // <editor-fold defaultstate="collapsed" desc="Bloom Effect Set">
    sprintf(path, "%s/GaussianBlurD.prog", effectsPath);
    blurEffect   = game->Content->Load<Effect> (path);
    blurSource   = blurEffect->getParameter("source");
    
    texScale     = blurEffect->getParameter("texScale");
    minGasussStr = blurEffect->getParameter("minGasussStr");
    samples      = blurEffect->getParameter("samples");
    level        = blurEffect->getParameter("level");
    dir          = blurEffect->getParameter("dir");
    
    sprintf(path, "%s/GaussFun.bmp", effectsPath);
    gaussFun = game->Content->Load<Texture2D> (path);
    blurEffect->getParameter("gaussFun").SetValue(gaussFun);
    
    minGaussStrenght(0.2f);
    textureScale(1.f / ((width+height) / 2));
    SampleNumber(samplesNumber);
    
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Print Effect Set">
    sprintf(path, "%s/GaussPrint.prog", effectsPath);
    printEffect = game->Content->Load<Effect> (path); 
    
    brightStr   = printEffect->getParameter("brightStr");
    flare       = printEffect->getParameter("flare");
    
    char name[3];
    for(int i = 0; i < TARGET_NUMBER; i ++) {
        sprintf(name, "b%d", i);
        printEffect->getParameter(name).SetValue((Texture2D *)targes[i]);
    }
    
    BrightStrenght(2.f);
    // </editor-fold>
}

void GaussianBlur::minGaussStrenght(float value) {
   minGasussStr.SetValue(&value);
}

void GaussianBlur::textureScale(float value) {
   texScale.SetValue(&value);
}

void GaussianBlur::BrightStrenght(float value) {
   brightStr.SetValue(&value);
}

void GaussianBlur::SampleNumber(int value) {
    float fvalue = (float) value;
    samples.SetValue(&fvalue);
}

static void DrawToVP() {
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex2i(-1, -1);
        
        glTexCoord2i(1, 0);
        glVertex2i(1, -1);
        
        glTexCoord2i(1, 1);
        glVertex2i(1, 1);
        
        glTexCoord2i(0, 1);
        glVertex2i(-1, 1);
    glEnd();
}

void GaussianBlur::Apply() {
    source->UseMipMap(true);
    //source->GenerateMipMaps();
    
    game->graphicDevice->DeepBufferEnabled(false);
    game->graphicDevice->DeepBufferWritteEnabled(false);
    game->graphicDevice->setRasterizerState(RasterizerState::CullNone);
    game->graphicDevice->setBlendState(BlendState::Opaque);
    
    ViewPort actualVP = game->graphicDevice->getViewPort();
    savedVP = actualVP;
    
    actualVP.width  /= 2;
    actualVP.height /= 2;
    
    blurSource.SetValue((Texture2D *) source);
    dir.SetValue(&Vector2::UnitX);        
    blurEffect->Begin();
    for(int i = 0; i < TARGET_NUMBER; i++) {
        game->graphicDevice->setViewPort(actualVP);
        game->graphicDevice->SetRenderTarget(targes[i]);
        
        float iLevel = (float) i;
        level.SetValue(&iLevel);
        
        DrawToVP();
        
        actualVP.width  /= 2;
        actualVP.height /= 2;
    }
    blurEffect->End();
    
    game->graphicDevice->SetRenderTarget(halfBlur);
    
    actualVP = savedVP;
    actualVP.width /= 2;
    actualVP.height /= 2;
    game->graphicDevice->setViewPort(actualVP);
    
    
    printEffect->Begin();
    float flareVal = 0.f;
    flare.SetValue(&flareVal);
    DrawToVP();
    printEffect->End();
    
    halfBlur->GenerateMipMaps();
    
    
    blurSource.SetValue((Texture2D *) halfBlur);
    dir.SetValue(&Vector2::UnitY);
    blurEffect->Begin();
    for(int i = 0; i < TARGET_NUMBER; i++) {
        game->graphicDevice->setViewPort(actualVP);
        game->graphicDevice->SetRenderTarget(targes[i]);
        
        float iLevel = (float) i;
        level.SetValue(&iLevel);
        
        DrawToVP();
        
        actualVP.width  /= 2;
        actualVP.height /= 2;
    }
    blurEffect->End();
    
    
    game->graphicDevice->SetRenderTarget(NULL);
    game->graphicDevice->setViewPort(savedVP);
    game->graphicDevice->setBlendState( BlendState(BlendState::One, BlendState::One, BlendState::Add));
    
    targes[2]->GenerateMipMaps();
    //printEffect->getParameter("b0").SetValue((Texture2D *)halfBlur);
    printEffect->Begin();
    flareVal = 1.f;
    flare.SetValue(&flareVal);
    DrawToVP();
    printEffect->End();
    
    game->graphicDevice->DeepBufferEnabled(true);
    game->graphicDevice->DeepBufferWritteEnabled(true);
    game->graphicDevice->setRasterizerState(RasterizerState::CullNone);
    game->graphicDevice->setBlendState(BlendState::Opaque);
}

GaussianBlur::~GaussianBlur() {
}

