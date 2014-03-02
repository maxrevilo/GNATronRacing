#include "../PostProcess.h"

PostProcess::PostProcess(Game *game) 
        : spriteBatch(game->graphicDevice) 
{
    graphicDevice = game->graphicDevice;
    
    active = true;
    
    // <editor-fold defaultstate="collapsed" desc="Capture Targets">
    int width = graphicDevice->getViewPort().width, height = graphicDevice->getViewPort().height;

    color = new RenderTarget2D(graphicDevice, width, height, true, BGRA,
            (RenderTarget2D::DepthFormat) 0, 0, (RenderTarget2D::RenderTargetUsage)0);

    luminance = new RenderTarget2D(graphicDevice, width, height, true, BGRA,
            (RenderTarget2D::DepthFormat) 0, 0, (RenderTarget2D::RenderTargetUsage)0);
    luminance->setSamplerAddressUV(Clamp);
    
    
    RTArray[0] = color;
    RTArray[1] = luminance; 
    
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Bloom">
    int targetNumber = 3;
    int samplesNumber = 18;
    
    halfBlur = new RenderTarget2D(graphicDevice, width/2.0, height/2.0, true, BGRA,
                (RenderTarget2D::DepthFormat) 0, 0, (RenderTarget2D::RenderTargetUsage)0);
    halfBlur->setSamplerAddressUV(Clamp);
    
    targes  = new RenderTarget2D*[targetNumber];
    int div = 2.0;
    
    for(int i = 0; i < targetNumber; i ++) {
        targes[i] =
                new RenderTarget2D(graphicDevice, width/div, height/div, true, BGRA,
                (RenderTarget2D::DepthFormat) 0, 0, (RenderTarget2D::RenderTargetUsage)0);
        targes[i]->setSamplerAddressUV(Clamp);
        div *= 2;
        //NVidia needs mipmap generation on target creation, later we don't need it
        targes[i]->UseMipMap(false);
    }
    
    // <editor-fold defaultstate="collapsed" desc="Bloom Effect Set">
    blurEffect   = game->Content->Load<Effect> ("Shaders/GaussianBlurD.prog");
    blurSource   = blurEffect->getParameter("source");
    
    texScale     = blurEffect->getParameter("texScale");
    samples      = blurEffect->getParameter("samples");
    level        = blurEffect->getParameter("level");
    dir          = blurEffect->getParameter("dir");
    
    textureScale(1.0f / width);
    SampleNumber(samplesNumber);
    
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Print Effect Set">
    printEffect = game->Content->Load<Effect> ("Shaders/GaussPrint.prog"); 
    
    brightStr   = printEffect->getParameter("brightStr");
    flare       = printEffect->getParameter("flare");
    
    printEffect->getParameter("scene").SetValue((Texture2D *)color);
    
    char name[3];
    for(int i = 0; i < targetNumber; i ++) {
        sprintf(name, "b%d", i);
        printEffect->getParameter(name).SetValue((Texture2D *)targes[i]);
    }
    
    BrightStrenght(1.f);
    // </editor-fold>
    // </editor-fold>

}

void PostProcess::Begin() {
    graphicDevice->SetRenderTargets(2, RTArray);
    graphicDevice->Clear(Color::Grey);
}

static void DrawToVP() {
    glDisableClientState(GL_INDEX_ARRAY);
    
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex2i(-1, -1);
        glNormal3b(0,0,0);
        
        glTexCoord2i(1, 0);
        glVertex2i(1, -1);
        glNormal3b(0,0,0);
        
        glTexCoord2i(1, 1);
        glVertex2i(1, 1);
        glNormal3b(0,0,0);
        
        glTexCoord2i(0, 1);
        glVertex2i(-1, 1);
        glNormal3b(0,0,0);
    glEnd();
    
    glEnableClientState(GL_INDEX_ARRAY);
}
    
void PostProcess::End() {
    if(active) {
        // <editor-fold defaultstate="collapsed" desc="Bloom">
        int targetNumber = 3;
        float flareVal;
        
        luminance->GenerateMipMaps();

        graphicDevice->DeepBufferEnabled(false);
        graphicDevice->DeepBufferWritteEnabled(false);
        graphicDevice->setRasterizerState(RasterizerState::CullNone);
        graphicDevice->setBlendState(BlendState::Opaque);
        
        ViewPort actualVP = graphicDevice->getViewPort();
        savedVP = actualVP;
        
        actualVP.width /= 2;
        actualVP.height /= 2;
        
        blurSource.SetValue((Texture2D *) luminance);
        dir.SetValue(&Vector2::UnitY);
        blurEffect->Begin();
        for (int i = 0; i < targetNumber; i++) {
            graphicDevice->setViewPort(actualVP);
            graphicDevice->SetRenderTarget(targes[i]);
            
            float iLevel = (float) i;
            level.SetValue(&iLevel);
            DrawToVP();
            
            actualVP.width /= 2;
            actualVP.height /= 2;
        }
        blurEffect->End();
        
        graphicDevice->SetRenderTarget(halfBlur);

        actualVP = savedVP;
        actualVP.width /= 2;
        actualVP.height /= 2;
        graphicDevice->setViewPort(actualVP);


        printEffect->Begin();
        flareVal = 0.f;
        flare.SetValue(&flareVal);
        DrawToVP();
        printEffect->End();

        halfBlur->GenerateMipMaps();


        blurSource.SetValue((Texture2D *) halfBlur);
        dir.SetValue(&Vector2::UnitX);
        blurEffect->Begin();
        for (int i = 0; i < targetNumber; i++) {
            graphicDevice->setViewPort(actualVP);
            graphicDevice->SetRenderTarget(targes[i]);

            float iLevel = (float) i;
            level.SetValue(&iLevel);

            DrawToVP();

            actualVP.width /= 2;
            actualVP.height /= 2;
        }
        blurEffect->End();

        graphicDevice->SetRenderTarget(NULL);
        graphicDevice->setViewPort(savedVP);
        
        printEffect->Begin();
        flareVal = 1.f;
        flare.SetValue(&flareVal);
        DrawToVP();
        printEffect->End();
        
        // </editor-fold>
    } else {
        graphicDevice->SetRenderTarget(NULL);
        graphicDevice->DeepBufferEnabled(false);
        graphicDevice->DeepBufferWritteEnabled(false);
        graphicDevice->setRasterizerState(RasterizerState::CullNone);
        graphicDevice->setBlendState(BlendState::Opaque);
        spriteBatch.Begin();
        spriteBatch.DrawFullScreenYInv(color, Color::White);
        spriteBatch.End();
    }
}

void PostProcess::textureScale(float value) {
   texScale.SetValue(&value);
}

void PostProcess::BrightStrenght(float value) {
   brightStr.SetValue(&value);
}

void PostProcess::SampleNumber(int value) {
    float fvalue = (float) value;
    samples.SetValue(&fvalue);
}
    
PostProcess::~PostProcess() {
    
}