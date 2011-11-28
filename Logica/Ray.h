#ifndef RAY_H
#define	RAY_H

#include "../GNAFramework/Geom.h"
#include "Plane.h"

using namespace GNAFramework;

struct Ray {
public:
    Vector3 Direction;
    Vector3 Position;

    Ray(){ }
    
    Ray (Vector3 position, Vector3 direction) {
        Position = position; Direction = direction;
    }
    
    void Intersects (Plane plane, float **result, float presition=0.0000001f) const {
        float div = Vector3::Dot(Direction, plane.Normal);
        if(div > presition || -div > presition ) {
            **result = Vector3::Dot(plane.Position - Position, plane.Normal) / div;
        } else {
            *result = NULL;
        }
    }
    
    Vector3 point_at(float t) const {
        return Position + Direction * t;
    }

    virtual ~Ray() {}
private:

};


#endif	/* RAY_H */

