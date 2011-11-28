#ifndef SPHERE_H
#define	SPHERE_H

#include "../GNAFramework/Geom.h"

using namespace GNAFramework;

struct Sphere {
public:
    float X, Y, Z;
    float R;

    Sphere(){
        X = 0.f; Y = 0.f; Z = 0.f; R = 1.f;
    }
    
    Sphere(Vector3 pos, float radio){
        X = pos.X; Y = pos.Y; Z = pos.Z; R = radio;
    }

    Sphere(float x, float y, float z, float radio){
        X = x; Y = y; Z = z; R = radio;
    }
    
    Vector3 Center() const { return Vector3(X,Y,Z); }
    
    void setCenter(Vector3 center) {
        X = center.X; Y = center.Y; Z = center.Z;
    }
    
    float Radio()const { return R; }
    
    bool Intersects(Sphere value) const{
        float dX = X - value.X, dY = Y - value.Y, dZ = Z - value.Z;
        float dR = R + value.R;
        return dX*dX + dY*dY + dZ*dZ <= dR * dR;
    }
    
    bool Contains(Vector3 value) const{
        float dX = X - value.X, dY = Y - value.Y, dZ = Z - value.Z;
        return dX*dX + dY*dY + dZ*dZ <= R*R;
    }

    virtual ~Sphere() {}
private:

};


#endif	/* SPHERE_H */

