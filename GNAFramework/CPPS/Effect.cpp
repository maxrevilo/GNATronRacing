#include <ios>

#include "../Effect.h"
#include "AdaptacionesDeFunciones.h"

using namespace GNAFramework;


bool Effect::effectStarted = false;
 
Effect::Effect(const Effect *clone) {
    lastParameterIndex = 0;
    texPosSize         = 0;
    begin              = false;
    throw new NotImplementedException("Funcion no soportada");
}

Effect::Effect(GraphicDevice *grapchicsDevice, const char *progCode){
    const char *vert_sources[2] = { "#define VERTEX\n", progCode },
               *frag_sources[2] = { "#define FRAGMENT\n", progCode};
    
    lastParameterIndex = 0;
    texPosSize         = 0;
    begin              = false;
    
    
    this->grapchicsDevice = grapchicsDevice;
    
    program = glCreateProgramObjectARB();
    vert    = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    frag    = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    
    
    
    glShaderSourceARB(vert, 2, (const GLcharARB**)vert_sources, NULL);
    glShaderSourceARB(frag, 2, (const GLcharARB**)frag_sources, NULL);
    
    
    char infobuffer[1000];
    int infobufferlen = 0;
    GLint compiled;
    
    glCompileShaderARB(vert);
    glGetObjectParameterivARB(vert, GL_COMPILE_STATUS, &compiled);
    
    if(!compiled) {
        glGetInfoLogARB(vert, 999, &infobufferlen, infobuffer);
        infobuffer[infobufferlen] = 0;
        printf("Log de VertexShader:\n%s", infobuffer);
        throw new GNAException("No se pudo compilar el effecto.");
    }
    
    glCompileShaderARB(frag);
    glGetObjectParameterivARB(frag, GL_COMPILE_STATUS, &compiled);
    
    if(!compiled) {
        glGetInfoLogARB(frag, 999, &infobufferlen, infobuffer);
        infobuffer[infobufferlen] = 0;
        printf("Log de FragmentShader:\n%s", infobuffer);
        throw new GNAException("No se pudo compilar el effecto.");
    }
    
    GLint linked;

    glAttachObjectARB(program, vert);
    glAttachObjectARB(program, frag);
    
    glLinkProgramARB(program);
    /*
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    
    if(!linked) {
        glGetInfoLogARB(program, 999, &infobufferlen, infobuffer);
        infobuffer[infobufferlen] = 0;
        printf("Log de ProgramShader:\n%s", infobuffer);
        throw new GNAException("No se pudo compilar el effecto.");
    }*/
    
    //printf("Programa creado %d\n", (int)program);
}

void Effect::instanciar() {
}


EffectParameter Effect::getParameter(const char * name) {
    return EffectParameter(this, name);
}



GLenum getGLTexturePosition(int pos);

void Effect::Begin(){
    if(effectStarted)
        throw new InvalidOperationException("Attempt to Begin() one effect while other has began.");
    
    if(begin)
        throw new InvalidOperationException("Attempt to Begin() one effect while has already began.");
    
    glUseProgramObjectARB(program);
    effectStarted = true;
    begin = true;
    
    for(int i = 0; i < texPosSize; i++) {
        glActiveTextureARB(getGLTexturePosition(i));
        glBindTexture(GL_TEXTURE_2D, texPos[i].texture->Pointer());
        
        glUniform1iARB(texPos[i].parameter, i);
    }
}

void Effect::End(){
    if(!begin)
        throw new InvalidOperationException("this effect haven't began.");
    
    glUseProgramObjectARB(0);
    effectStarted = false;
    begin         = false;
}

void Effect::Apply() {
    
}

Effect *Effect::Clone(){
    throw new NotImplementedException("Funcion no soportada");
}

Effect::~Effect() {
}



GLenum getGLTexturePosition(int pos) {
    switch (pos) {
        case 0: return GL_TEXTURE0_ARB;
        case 1: return GL_TEXTURE1_ARB;
        case 2: return GL_TEXTURE2_ARB;
        case 3: return GL_TEXTURE3_ARB;
        case 4: return GL_TEXTURE4_ARB;
        case 5: return GL_TEXTURE5_ARB;
        case 6: return GL_TEXTURE6_ARB;
        case 7: return GL_TEXTURE7_ARB;
        case 8: return GL_TEXTURE8_ARB;
        case 9: return GL_TEXTURE9_ARB;
        case 10: return GL_TEXTURE10_ARB;
        case 11: return GL_TEXTURE11_ARB;
        case 12: return GL_TEXTURE12_ARB;
        case 13: return GL_TEXTURE13_ARB;
        case 14: return GL_TEXTURE14_ARB;
        case 15: return GL_TEXTURE15_ARB;
        case 16: return GL_TEXTURE16_ARB;
    }
}
