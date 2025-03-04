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

enum class KeyState {
    kNone,
    kTap,
    kHold,
    kUp,
    END
};

class InputMgr{
    private:
        static MouseState mouse_state;
        static std::array<KeyState,sf::Keyboard::KeyCount> key_state;
        static Vec2 mouse_pos;

    public:
        static void UpdateInputState(sf::Event _event);
        static void UpdateKeyNone();

        static MouseState GetMouseState(){return mouse_state;}
        static Vec2 GetMousePos(){return mouse_pos;}

    private:
        static void UpdateMouseState(sf::Event event);
        static void UpdateKeyState(sf::Event event);  

};