#include "../MathHelper.h"

using namespace GNAFramework;

float MathHelper::Max(float a, float b){
    if(a > b)   return a;
    else        return b;
}

int MathHelper::Max(int a, int b){
    if(a > b)   return a;
    else        return b;
}



float MathHelper::Min(float a, float b){
    if(a < b)   return a;
    else        return b;
}

int MathHelper::Min(int a, int b){
    if(a < b)   return a;
    else        return b;
}



float MathHelper::Truncate(float val, float min, float max){
    if(val < min) return min;
    else if(val > max) return max;
    else return val;
}

float MathHelper::Lerp(float ini, float end, float val){
    return (1 - val)*ini + val*end;
}

float MathHelper::LerpTrunc(float ini, float end, float val){
    return Truncate(Lerp(ini, end, val), ini, end);
}


float MathHelper::ToRadians(float  deg){
    return deg * 0.017453292519943295;
}

float MathHelper::ToDegrees(float rad){
    return rad * 57.29577951308232;
}



