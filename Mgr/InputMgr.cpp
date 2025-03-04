#include "InputMgr.h"
#include "../pch.h"
#include "../Core.h"

MouseState InputMgr::mouse_state{MouseState::kNone};
std::array<KeyState,sf::Keyboard::KeyCount> InputMgr::key_state{KeyState::kNone};

Vec2 InputMgr::mouse_pos{};

void InputMgr::UpdateInputState(sf::Event _event)
{
    UpdateMouseState(_event);
    UpdateKeyState(_event);
}

void InputMgr::UpdateKeyNone()
{
    for (int i=0; i<sf::Keyboard::KeyCount; ++i){
        if (key_state[i] == KeyState::kUp){
            key_state[i] = KeyState::kNone;
        }
    }
}

void InputMgr::UpdateMouseState(sf::Event event)
{
    auto window = Core::GetWindowContext();
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

    mouse_pos.x = mousePos.x;
    mouse_pos.y = mousePos.y;

    // 마우스 버튼이 눌렸을 때 감지
    if (event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left){
            if ( mouse_state == MouseState::kNone){
                // 아무것도 아니었으면 TAP
                mouse_state = MouseState::kLeftTap;
            }
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            if ( mouse_state == MouseState::kNone){
                // 아무것도 아니었으면 TAP
                mouse_state = MouseState::kRightTap;
            }
        }
    }
    // 마우스 버튼이 떼어졌을 때 감지
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left){
            mouse_state = MouseState::kLeftUp;
        }
        else if ( event.mouseButton.button == sf::Mouse::Right){
            mouse_state = MouseState::kRightUp;
        }
    }
    else{
        // 안떨어짐 
        if ( mouse_state == MouseState::kLeftTap ){
            mouse_state = MouseState::kLeftHold;
        }
        else if ( mouse_state == MouseState::kRightTap ){
            mouse_state = MouseState::kRightHold;
        }

        if ( mouse_state == MouseState::kLeftUp || mouse_state == MouseState::kRightUp)
            mouse_state = MouseState::kNone;
    }
}

void InputMgr::UpdateKeyState(sf::Event event)
{
    if ( event.type == sf::Event::KeyPressed){
        if ( key_state[event.key.code] == KeyState::kNone ){
            key_state[event.key.code] = KeyState::kTap;
        }
    }
    else if ( event.type == sf::Event::KeyReleased){
        if ( key_state[event.key.code] == KeyState::kHold){
            key_state[event.key.code] = KeyState::kUp;
        }
    }
    else{
        for (int i=0; i<sf::Keyboard::KeyCount; ++i){
            if ( key_state[i] == KeyState::kTap ){
                key_state[i] = KeyState::kHold;
            }
            
            if (key_state[i] == KeyState::kUp){
                key_state[i] = KeyState::kNone;
            }
        }
    }
}
