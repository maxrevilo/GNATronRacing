#ifndef DIRECTORCAMERA_H
#define	DIRECTORCAMERA_H

#include "../GNAFramework/Keyboard.h"
#include "../GNAFramework/Mouse.h"
#include "../GNAFramework/GraphicDevice.h"
#include "../GNAFramework/Game.h"

#include "Camera.h"

using namespace GNAFramework;

class DirectorCamera {
public:
    
    struct Controls {
        //To Move the camera on 3 dimensions.
        Keyboard::Key moveUp;
        Keyboard::Key moveDown;
        Keyboard::Key moveForward;
        Keyboard::Key moveBackward;
        Keyboard::Key moveLeft;
        Keyboard::Key moveRight;
        
        //To open and Clonse the Field of View (FOV).
        Keyboard::Key openFOV;
        Keyboard::Key closeFOV;
        
        //To increase and decrease the Far Plane.
        Keyboard::Key incFarPlane;
        Keyboard::Key decFarPlane;
        
        //To increase and decrease the Near Plane.
        Keyboard::Key incNearPlane;
        Keyboard::Key decNearPlane;
    };
    
    Controls controls;
    Camera   camera;
    
    /**
     * The speed of movement of the camera on the world.
     * (Units on world units over seconds).
     * (1.0 by default).
     */
    float moveSpeed;
    
    /**
     * The quanty of degres that chage de FOV.
     * (Units on degres over seconds).
     * (5.0 by default).
     */
    float FOVChangeSpeed;
    
    /**
     * The speed of change of de near and far plane.
     * (Units on world units over seconds).
     * (0.5 by default).
     */
    float PlanesChangeSpeed;
    
    /**
     * The speed of rotation of the camera relative to de mouse movement.
     * (Units on degres over mouse displacement).
     * (90.0/400.0 by default).
     */
    float rotationSpeed;
    
    /**
     * If active the Y axis of the mouse will be inverted.
     * (false by default).
     */
    bool Y_Axix_Inverted;
    
    
    DirectorCamera(GraphicDevice *graphicDevice, Camera camera);
    
    void Update(GameTime gameTime);
    
    virtual ~DirectorCamera();
private:
    GraphicDevice *graphicDevice;
    
};

#endif	/* DIRECTORCAMERA_H */

