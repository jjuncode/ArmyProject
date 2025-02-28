#pragma once
#include "SFML/Graphics.hpp"
#include "../struct.h"

enum class MouseState{
    kLeftTap,
    kLeftHold,
    kLeftUp,
    
    kRightTap,
    kRightHold,
    kRightUp,

    kNone,
    END
};

class InputMgr{
    private:
        static MouseState mouse_state;
        static Vec2 mouse_pos;


    public:
        static void UpdateInputState(sf::Event _event);

        static MouseState GetMouseState(){return mouse_state;}
        static Vec2 GetMousePos(){return mouse_pos;}

    private:
        static void UpdateMouseState(sf::Event event);

};