#ifndef CUBE_H
#define	CUBE_H

#include "../GNAFramework/Geom.h"
#include "Sphere.h"

using namespace GNAFramework;

struct Cube {
public:
    Vector3 p0, p1;

    Cube(){
        p0 = Vector3();
        p1 = Vector3();
    }

    Cube(Vector3 P0, Vector3 P1){
        p0 = P0; p1 = P1;
    }
    
    Cube(float size){
        p0 = Vector3(-0.5,  0.5, -0.5) * size;
        p1 = Vector3( 0.5, -0.5,  0.5) * size;
    }
    
    bool Intersects(BoundingSphere value) const{
        return Contains(value.Center());
    }
    
    bool Contains(Vector3 value) const{
        bool a = value.X > p0.X && value.Y < p0.Y && value.Z > p0.Z;
        bool b = value.X < p1.X && value.Y > p1.Y && value.Z < p1.Z;
        
        return a & b;
    }
    
    void Move(Vector3 pos) {
        p0 = p0 + pos;
        p1 = p1 + pos;
    }
    
    Vector3 Center() {
        return (p0 + p1) / 2.f;
    }

    virtual ~Cube() {}
private:

};


#endif	/* CUBE_H */

