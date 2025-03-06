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
        static void UpdateKeyState();
        
        static MouseState GetMouseState(){return mouse_state;}
        static Vec2 GetMousePos(){return mouse_pos;}
        
        static KeyState GetKeyState(sf::Keyboard::Key _key ) { return key_state[_key];}
        
        static bool IsTap(sf::Keyboard::Key _key ) {
            if ( GetKeyState(_key ) == KeyState::kTap){
                return true;
            }
            return false;
        }
        static bool IsHold(sf::Keyboard::Key _key ) {
            if ( GetKeyState(_key ) == KeyState::kHold){
                return true;
            }
            return false;
        }
        static bool IsUp(sf::Keyboard::Key _key ) {
            if ( GetKeyState(_key ) == KeyState::kUp){
                return true;
            }
            return false;
        }
        
        private:
        static void UpdateMouseState(sf::Event event);
        static void UpdateKeyState(sf::Event event);  
        

};