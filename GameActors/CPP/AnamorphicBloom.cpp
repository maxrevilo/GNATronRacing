#include "../AnamorphicBloom.h"

bool AnamorphicBloom::SBegan = false;

AnamorphicBloom::AnamorphicBloom(
            Game *game,
            const char *effectsPath,
            int width, 
            int height, 
            int sampleNumber,
            bool vertical,
            int nTargets) {
    
    this->vertical     = vertical;
    this->game         = game;
    this->targetNumber = nTargets;
    
    scene = new RenderTarget2D(game->graphicDevice, width, height, true, BGRA,
                (RenderTarget2D::DepthFormat) 0, 0, (RenderTarget2D::RenderTargetUsage)0);
    scene->setSamplerAddressUV(Clamp);
    
    targes  = new RenderTarget2D*[nTargets];
    int div = 1;
    
    for(int i = 0; i < nTargets; i ++) {
        targes[i] =
                new RenderTarget2D(game->graphicDevice, width/div, height/div, false, BGRA,
                (RenderTarget2D::DepthFormat) 0, 0, (RenderTarget2D::RenderTargetUsage)0);
        targes[i]->setSamplerAddressUV(Clamp);
        div *= 2;
    }
    
    
    char path[256];
    
    // <editor-fold defaultstate="collapsed" desc="Bloom Effect Set">
    sprintf(path, "%s/AnamorphicBlur.prog", effectsPath);
    blurEffect = game->Content->Load<Effect> (path);
    blurEffect->getParameter("scene").SetValue((Texture2D *) scene);

    sprintf(path, "%s/AnamorphicFun.bmp", effectsPath);
    anamorphicFun = game->Content->Load<Texture2D> (path);
    blurEffect->getParameter("gaussFun").SetValue(anamorphicFun);

    texScale     = blurEffect->getParameter("texScale");
    minGasussStr = blurEffect->getParameter("minGasussStr");
    samples      = blurEffect->getParameter("samples");
    level        = blurEffect->getParameter("level");


    minGaussStrenght(0.2f);
    textureScale(2.f / (float) (vertical ? height : width));
    SampleNumber(sampleNumber); 
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Print Effect Set">
    sprintf(path, "%s/AnamorphicPrint.prog", effectsPath);
    printEffect = game->Content->Load<Effect> (path); 
    
    brightStr   = printEffect->getParameter("brightStr");
    
    char name[3];
    for(int i = 0; i < nTargets; i ++) {
        sprintf(name, "b%d", i);
        printEffect->getParameter(name).SetValue((Texture2D *)targes[i]);
    }
    
    BrightStrenght(2.f);
    // </editor-fold>

    
}

void AnamorphicBloom::minGaussStrenght(float value) {
   minGasussStr.SetValue(&value);
}

void AnamorphicBloom::textureScale(float value) {
   texScale.SetValue(&value);
}

void AnamorphicBloom::BrightStrenght(float value) {
   brightStr.SetValue(&value);
}

void AnamorphicBloom::SampleNumber(int value) {
    float fvalue = (float) value;
    samples.SetValue(&fvalue);
}


Texture2D *AnamorphicBloom::getCaputedImage(){
    return (Texture2D *) scene;
}


void AnamorphicBloom::Begin() {
    if(SBegan)
        throw new InvalidOperationException("AnamorphicBloom already began.");
    SBegan = true;
    
    savedVP = game->graphicDevice->getViewPort();
    
    game->graphicDevice->setViewPort(ViewPort(0, 0, scene->getWidth(), scene->getHeight()));
    
    game->graphicDevice->SetRenderTarget(scene);
    game->graphicDevice->Clear(Color::Black);
}


static void DrawToVP(ViewPort VP) {
    RectangleF rect = RectangleF(-VP.width, -VP.height, 2.0*VP.width,  2.0*VP.height);
    
    glBegin(GL_QUADS);
        //Bottom-left vertex (corner)
        glTexCoord2i(0, 1);
        glVertex3f(rect.X, rect.Y, 0.0f);

        //Bottom-right vertex (corner)
        glTexCoord2i(1, 1);
        glVertex3f(rect.X + rect.width, rect.Y, 0.f);

        //Top-right vertex (corner)
        glTexCoord2i(1, 0);
        glVertex3f(rect.X + rect.width, rect.Y + rect.height, 0.f);

        //Top-left vertex (corner)
        glTexCoord2i(0, 0);
        glVertex3f(rect.X, rect.Y + rect.height, 0.f);
    glEnd();
}
    
void AnamorphicBloom::End() {
    if(!SBegan)
        throw new InvalidOperationException("AnamorphicBloom has not begun.");
    SBegan = false;
    
    
    scene->GenerateMipMaps();
    
    game->graphicDevice->DeepBufferEnabled(false);
    game->graphicDevice->DeepBufferWritteEnabled(false);
    game->graphicDevice->setRasterizerState(RasterizerState::CullNone);
    game->graphicDevice->setBlendState(BlendState::Opaque);
    
    ViewPort actualVP = game->graphicDevice->getViewPort();
    blurEffect->Begin();
    for(int i = 0; i < targetNumber; i++) {
        game->graphicDevice->setViewPort(actualVP);
        game->graphicDevice->SetRenderTarget(targes[i]);
        
        float iLevel = (float) i;
        level.SetValue(&iLevel);
        
        
        DrawToVP(actualVP);
        
        
        actualVP.width  /= 2;
        actualVP.height /= 2;
    }
    blurEffect->End();
    
    game->graphicDevice->SetRenderTarget(NULL);
    game->graphicDevice->setViewPort(savedVP);
    game->graphicDevice->setBlendState(BlendState(BlendState::One, BlendState::One, BlendState::Add));
    
    printEffect->Begin();
    DrawToVP(savedVP);
    printEffect->End();
    
    game->graphicDevice->DeepBufferEnabled(true);
    game->graphicDevice->DeepBufferWritteEnabled(true);
    game->graphicDevice->setRasterizerState(RasterizerState::CullNone);
    game->graphicDevice->setBlendState(BlendState::Opaque);
    
}


AnamorphicBloom::~AnamorphicBloom() {
    delete [] targes;
    delete [] scene;
    delete blurEffect;
    delete printEffect;
    delete anamorphicFun;
}

