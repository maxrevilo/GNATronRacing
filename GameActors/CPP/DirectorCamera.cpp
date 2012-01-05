#include "../DirectorCamera.h"

static DirectorCamera::Controls defaultConstrols() {
    DirectorCamera::Controls r;
    r.moveUp       = Keyboard::E;
    r.moveDown     = Keyboard::Q;
    r.moveForward  = Keyboard::W;
    r.moveBackward = Keyboard::S;
    r.moveLeft     = Keyboard::A;
    r.moveRight    = Keyboard::D;
    
    r.openFOV      = Keyboard::R;
    r.closeFOV     = Keyboard::F;
    
    r.incFarPlane  = Keyboard::Z;
    r.decFarPlane  = Keyboard::X;
    
    r.incNearPlane = Keyboard::C;
    r.decNearPlane = Keyboard::V;
    
    return r;
}


DirectorCamera::DirectorCamera(GraphicDevice *graphicDevice, Camera camera) {
    this->graphicDevice = graphicDevice;
    this->camera        = camera;
    
    moveSpeed           = 1.f;
    FOVChangeSpeed      = 5.f;
    PlanesChangeSpeed   = 5.f;
    rotationSpeed       = 90.0/400.0;
    Y_Axix_Inverted     = false;
    
    controls = defaultConstrols();
}

void DirectorCamera::Update(GameTime gameTime) {
    KeyboardState kbs  = Keyboard::getState();
    MouseState    mss  = Mouse::getState();
    float         time = gameTime.elapsedTime.toSecondsF();
    
    // <editor-fold defaultstate="collapsed" desc="Camera Movement">
    Vector3 mov = Vector3::Zero;

    if (kbs.IsKeyDown(controls.moveForward)) {
        mov += camera.getForward();
    } else if (kbs.IsKeyDown(controls.moveBackward)) {
        mov += camera.getForward() * -1;
    }

    if (kbs.IsKeyDown(controls.moveRight)) {
        mov += camera.getRight() * -1;
    } else if (kbs.IsKeyDown(controls.moveLeft)) {
        mov += camera.getRight();
    }

    if (kbs.IsKeyDown(controls.moveUp)) {
        mov += camera.getUp();
    } else if (kbs.IsKeyDown(controls.moveDown)) {
        mov += camera.getUp() * -1;
    }

    camera.setPosition(camera.getPosition() + mov * moveSpeed * time);
    camera.setFocus(camera.getFocus() + mov * moveSpeed * time); // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Camera Proyection">
    Frustum frust = camera.getFrustum();

    if (kbs.IsKeyDown(controls.openFOV)) {
        frust.setFovY(frust.getFovY() + FOVChangeSpeed * time);
    } else if (kbs.IsKeyDown(controls.closeFOV)) {
        frust.setFovY(frust.getFovY() - FOVChangeSpeed * time);
    }

    if (kbs.IsKeyDown(controls.incFarPlane)) {
        frust.setBack(frust.getBack() + PlanesChangeSpeed * time);
    } else if (kbs.IsKeyDown(controls.decFarPlane)) {
        frust.setBack(frust.getBack() - PlanesChangeSpeed * time);
    }

    if (kbs.IsKeyDown(controls.incNearPlane)) {
        frust.setFront(frust.getFront() + PlanesChangeSpeed * time);
    } else if (kbs.IsKeyDown(controls.decNearPlane)) {
        frust.setFront(frust.getFront() - PlanesChangeSpeed * time);
    }

    camera.setFrustum(frust);
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Camera Rotation">
    
    //HACER LA ROTACION DE LA CAMARA
    
    // </editor-fold>

}

DirectorCamera::~DirectorCamera() {
    
}

