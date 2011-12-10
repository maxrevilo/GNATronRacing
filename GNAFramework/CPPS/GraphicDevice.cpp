#include "../GraphicDevice.h"
#include "AdaptacionesDeFunciones.h"
#if defined(__WIN32__)
    #include <windows.h>
#endif

using namespace GNAFramework;




GraphicDevice::GraphicDevice(int width, int height, bool fullScreen) {
    ViewPort viewPort;
    viewPort.X = 0;
    viewPort.Y = 0;
    viewPort.width = width;
    viewPort.height = height;

    BlendState blendState;
    //Para forzar a cambiar el BlendState:
    this->blendState.SourceBlend = (BlendState::Blend) 0;
    
    RasterizerState rasterizerState;
    //Para forzar a cambiar el RasterizerState:
    this->rasterizerState.cullMode = (RasterizerState::CullMode) 0;

    gameWindow = new GameWindow(width, height, (char *) "Game", (char *) "GameIcon.bmp");
    
    
    #ifdef WIN32
    glBlendEquation = (PFNGLBLENDEQUATIONPROC) SDL_GL_GetProcAddress("glBlendEquation");
    if (glBlendEquation == NULL) {
        fprintf(stderr, "BlendEquation Support not present.\n");
        exit(1);
    }
    glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) SDL_GL_GetProcAddress("glDrawRangeElements");
    #endif
    
    //Funciones de buffers:
    GNAglBindBufferARB = (PFNGLBINDBUFFERARBPROC) SDL_GL_GetProcAddress("glBindBufferARB");
    GNAglGenBuffersARB = (PFNGLGENBUFFERSARBPROC) SDL_GL_GetProcAddress("glGenBuffersARB");    
    GNAglBufferDataARB = (PFNGLBUFFERDATAARBPROC) SDL_GL_GetProcAddress("glBufferDataARB");
    
    //Funciones de Shaders:
    char errors[128];
    if(!setupExtensions(errors)) {
        #if defined(__WIN32__)
            MessageBoxA(NULL, errors,"GNA: Error en la Tarjeta Grafica",MB_OK|MB_ICONERROR);
        #else
            perror(errors);
        #endif
        
    }
    
    glEnableClientState(GL_INDEX_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_TEXTURE_2D);
    
    DeepBufferEnabled(true);
    DeepBufferWritteEnabled(true);
    

    clearColor = new Color(.0f, .0f, .0f, .0f);
    glClearColor(.0f, .0f, .0f, .0f);

    setViewPort(viewPort);

    setBlendState(blendState);
    setRasterizerState(rasterizerState);

    glClear(GL_COLOR_BUFFER_BIT);    
}

void GraphicDevice::setViewPort(ViewPort viewPort) {

    if (this->viewPort.X != viewPort.X ||
            this->viewPort.Y != viewPort.Y ||
            this->viewPort.height != viewPort.height ||
            this->viewPort.width != viewPort.width) {

        this->viewPort = viewPort;
        glViewport(viewPort.X, viewPort.Y, viewPort.width, viewPort.height);
    }
}

ViewPort GraphicDevice::getViewPort() {
    return viewPort;
}

bool GraphicDevice::isFullScreen() {
    return fullScreen;
}

void GraphicDevice::setFullScreen(bool fullScreen) {
    perror("GraphicDevice::setFullScreen: No soportado.");
    exit(1);
}

void GraphicDevice::setBlendState(BlendState blendState) {

    if (this->blendState != blendState) {
        this->blendState = blendState;

        if (blendState.DestinationBlend == BlendState::Zero
                && blendState.SourceBlend == BlendState::One) {
            glDisable(GL_BLEND);
        } else {
            glEnable(GL_BLEND);
            glBlendFunc((GLenum) blendState.SourceBlend, (GLenum) blendState.DestinationBlend);
            glBlendEquation((GLenum) blendState.ColorBlendFunction);

        }
    }

}

BlendState GraphicDevice::getBlendState() {
    return blendState;
}



void GraphicDevice::setRasterizerState(RasterizerState rasterizerState) {

    if (this->rasterizerState != rasterizerState) {
        this->rasterizerState = rasterizerState;

        glFrontFace(GL_CCW);
        if(rasterizerState.cullMode == RasterizerState::None) {
            glDisable(GL_CULL_FACE);
        } else {
            glEnable(GL_CULL_FACE);
            glCullFace((GLenum)rasterizerState.cullMode);
        }
        
        
        glPolygonMode(GL_FRONT_AND_BACK, (GLenum)rasterizerState.fillMode);
    }

}

RasterizerState GraphicDevice::getRasterizerState() {
    return rasterizerState;
}

void GraphicDevice::DeepBufferEnabled(bool enabled) {
    if(enabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void GraphicDevice::DeepBufferWritteEnabled(bool enabled){
    glDepthMask((GLboolean) enabled);
}





IndexBuffer GraphicDevice::getIndices() {
    return indexBuffer;
}

void GraphicDevice::setIndices(IndexBuffer indexBuffer){
    this->indexBuffer = indexBuffer;
}

VertexBuffer GraphicDevice::getVertexBuffer() {
    return vertexBuffer;
}

void GraphicDevice::setVertexBuffer(VertexBuffer vertexBuffer){
    
    this->vertexBuffer = vertexBuffer;
    
    int i;
    VertexDeclaration vd = vertexBuffer.vertexDeclaration;
    const VertexElement *ves = vd.GetVertexElements();
    GLint size; GLenum type; GLsizei stride; const GLvoid *ptr;
    
    GNAglBindBufferARB(GL_ARRAY_BUFFER, vertexBuffer.pointer);
    for(i=0; i < vd.getElementNumber(); i++){
        size   = VertexElement::GL_Type_Number(ves[i].vertexElementFormat);
        type   = VertexElement::GL_Type(ves[i].vertexElementFormat);
        stride = vd.getVertexStride();
        ptr    = (char*)NULL + ves[i].Offset;
        
        
        
        switch (ves[i].vertexElementUsage){
            case VertexElement::Position:
                //printf("glVertexPointer(%d, %d, %d, 0+%d);\n", size, type, stride, ves[i].Offset);
                glVertexPointer(size, type, stride, ptr);
                break;
            
            case VertexElement::Color   :
                //printf("glColorPointer(%d, %d, %d, 0+%d);\n", size, type, stride, ves[i].Offset);
                glColorPointer(size, type, stride, ptr);
                break;
                
            case VertexElement::Normal  :
                glNormalPointer(type, stride, ptr);
                break;
            
            case VertexElement::TextureCoordinate:
                glTexCoordPointer(size, type, stride, ptr);
                break;
        }
    }
    
    GNAglBindBufferARB(GL_ARRAY_BUFFER, 0);
}



void GraphicDevice::DrawIndexedPrimitives (
                         PrimitiveType primitiveType,
                         //int baseVertex,
                         int minVertexIndex,
                         int numVertices,
                         //int startIndex,
                         int primitiveCount
                        ){
    GNAglBindBufferARB(GL_ARRAY_BUFFER, 0);
    
    GNAglBindBufferARB(GL_ARRAY_BUFFER, vertexBuffer.pointer);
    GNAglBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.pointer);
    
    
    GLenum mask;
    if(indexBuffer.indexElementSize == IndexBuffer::ThirtyTwoBits) mask = GL_UNSIGNED_INT;
    else mask = GL_UNSIGNED_SHORT;
    
    glDrawRangeElements((GLenum) primitiveType, 
                        minVertexIndex, 
                        minVertexIndex+numVertices,
                        primitiveCount,
                        mask,
                        (const GLvoid *)0);
    
    GNAglBindBufferARB(GL_ARRAY_BUFFER, 0);
    GNAglBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void GraphicDevice::Clear(ClearOptions Mask, Color color, GLclampd depth, int stencil) {
    switch (Mask) {
        case Target:
            if (color != (*clearColor)) {
                Vector4 fColor = color.toVector4();
                glClearColor(fColor.X, fColor.Y, fColor.Z, fColor.W);
                *clearColor = color;
                glClearDepth( depth);
            }
            break;
        default:
            
            break;
    }

    glClear(Mask);
}

void GraphicDevice::Clear(Color color) {
    Clear(Target, color, 1.0, 0);
}

GameWindow *GraphicDevice::getGameWindow() {
    return gameWindow;
}


//GraphicDevice::GraphicDevice(const GraphicDevice& orig) { }

GraphicDevice::~GraphicDevice() {
    glDisableClientState(GL_INDEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisable(GL_TEXTURE_2D);
    
    delete gameWindow;
}

