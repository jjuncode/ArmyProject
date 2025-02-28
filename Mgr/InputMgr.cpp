#include "InputMgr.h"
#include "../pch.h"
#include "../Core.h"

MouseState InputMgr::mouse_state{MouseState::kNone};
Vec2 InputMgr::mouse_pos{};

void InputMgr::UpdateInputState(sf::Event _event)
{
    UpdateMouseState(_event);
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
