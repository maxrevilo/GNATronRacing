#ifndef CAMERA_H
#define	CAMERA_H

#include "../GNAFramework/Geom.h"

using namespace GNAFramework;

class Camera {
public:
    
    
    Camera() {
        view_changed = false;
        pers_changed = false;
    }

    Camera( Vector3 position,
            Vector3 focus,
            Vector3 up,
            Frustum frustum) {
        
        this->position = position;
        this->focus    = focus;
        this->up       = up;
        view = Matrix::CreateLookAt(position, focus, up);
        view_changed   = false;
        
        this->frustum = frustum;
        perspective   = Matrix::CreatePerspective(frustum);
        pers_changed  = false;
        
    }
    
    Matrix viewMatrix() {
        if(view_changed) {
            view = Matrix::CreateLookAt(position, focus, up);
            view_changed = false;
        }
        
        return view;
    }
    
    Matrix projectionMatrix() {
        if(pers_changed) {
            perspective = Matrix::CreatePerspective(frustum);
            pers_changed = false;
        }
        
        return perspective;
    }
    
    Vector3 getFocus()    const { return focus;    }
    Vector3 getPosition() const { return position; }
    Vector3 getUp()       const { return up;       }
    Frustum getFrustum()  const { return frustum; }
    
    
    void setFocus(Vector3 value) {
        focus = value;
        view_changed = true;
    }
    void setPosition(Vector3 value) {
        position = value;
        view_changed = true;
    }
    void setUp(Vector3 value) {
        up = value;
        view_changed = true;
    }
    void setFrustum(Frustum value) {
        frustum = value;
        pers_changed = true;
    }
    
private:
    Vector3 focus;
    Vector3 position;
    Vector3 up;
    
    Matrix view;
    bool view_changed;
    
    Frustum frustum;
    
    Matrix perspective;
    bool pers_changed;
};

#endif	/* CAMERA_H */

