#ifndef EFFECTPARAMETERCOLLECTION_H
#define	EFFECTPARAMETERCOLLECTION_H

#include "EffectParameter.h"

namespace GNAFramework {
    class EffectParameterCollection {
    public:
        EffectParameterCollection();
        
        EffectParameter Get(char *);
        
        int Count();
        
        virtual ~EffectParameterCollection();
    private:
        //Effect efecto;
    };
}

#endif	/* EFFECTPARAMETERCOLLECTION_H */

