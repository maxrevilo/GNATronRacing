#ifndef PLANE_H
#define	PLANE_H

#include "../GNAFramework/Geom.h"

using namespace GNAFramework;

struct Plane {
public:
    Vector3 Normal;
    Vector3 Position;

    Plane(){ }
    
    Plane(Vector3 Point, Vector3 Normal){
        this->Position = Point; this->Normal = Normal;
    }
    
    Vector3 NearestPoint(Sphere value) const{
        float t = Vector3::Dot(Position - value.Center(), Normal) / Vector3::Dot(Normal, Normal);
        return Normal * t + value.Center();
    }
    
    bool Contains(Vector3 value, float presition=0.0000001f) const{
        return abs(Vector3::Dot(Vector3::Normalize(value - Position), Normal));
    }

    virtual ~Plane() {}
private:

};


#endif	/* PLANE_H */

