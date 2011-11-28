#include "../InputManager.h"

InputManager::InputManager() {
}

void InputManager::Initialize(){
    kbs = Keyboard::getState();
    mss = Mouse::getState();
}


void InputManager::Update() {
    kbs_prev = kbs;
    kbs = Keyboard::getState();
    
    mss_prev = mss;
    mss = Mouse::getState();
}


bool InputManager::KeyPressed(int key){
    return kbs.IsKeyDown(key);
}

bool InputManager::KeyGetPressed(int key){
    return kbs.IsKeyDown(key) && kbs_prev.IsKeyUp(key);;
}

InputManager::~InputManager() {
}

