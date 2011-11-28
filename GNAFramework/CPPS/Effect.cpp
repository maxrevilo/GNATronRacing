#include <ios>

#include "../Effect.h"
#include "AdaptacionesDeFunciones.h"

using namespace GNAFramework;

Effect::Effect(const Effect *clone) {
    lastParameterIndex = 0;
    throw new NotImplementedException("Funcion no soportada");
}

Effect::Effect(GraphicDevice *grapchicsDevice, const char *progCode){
    const char *vert_sources[2] = { "#define VERTEX\n", progCode },
               *frag_sources[2] = { "#define FRAGMENT\n", progCode};
    
    lastParameterIndex = 0;
    
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
    GLint u = glGetUniformLocationARB(program, name);
    EffectParameter *lep = searchParameter(u);
    if(lep == NULL) {
        parameters[lastParameterIndex] = EffectParameter(u, name, texPos+lastParameterIndex);
        return parameters[lastParameterIndex++];
    } else {
        return *lep;
    }
}

EffectParameter *Effect::searchParameter(GLint location){
    int i;
    for(i = 0; i < MAX_PARMETERS; i++){
        if(parameters[i].location == location) return &parameters[i];
    }
    
    return NULL;
}

void Effect::Begin(){
    glUseProgramObjectARB(program);
}

void Effect::End(){
    glUseProgramObjectARB(0);
}

void Effect::Apply() {
    
}

Effect *Effect::Clone(){
    throw new NotImplementedException("Funcion no soportada");
}

Effect::~Effect() {
}

