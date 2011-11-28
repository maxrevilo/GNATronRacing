#ifndef MATHHELPER_H
#define	MATHHELPER_H

#include "math.h"

#define Pi 3.141592653589793f
#define PiOver2 1.5707963267948966f
#define PiOver4 0.7853981633974483f
#define TwoPi 6.283185307179586f

namespace GNAFramework {

    class MathHelper {
    public:
        static float Max(float a, float b);

        static int Max(int a, int b);



        static float Min(float a, float b);

        static int Min(int a, int b);



        static float Truncate(float val, float min, float max);

        static float Lerp(float ini, float end, float val);

        static float LerpTrunc(float ini, float end, float val);


        static float ToRadians(float degrees);

        static float ToDegrees(float radians);
    };
    
}

#endif	/* MATHHELPER_H */

