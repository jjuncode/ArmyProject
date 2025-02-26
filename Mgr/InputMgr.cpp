#include "InputMgr.h"
#include "../pch.h"

void InputMgr::UpdateInputState(sf::Event _event)
{
    UpdateMouseState(_event);
}

void InputMgr::UpdateMouseState(sf::Event event)
{
    // 마우스 버튼이 눌렸을 때 감지
    if (event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left){
            if ( mouse_state == MouseState::kNone){
                // 아무것도 아니었으면 TAP
                mouse_state = MouseState::kLeftTap;
            }
            else if ( mouse_state == MouseState::kLeftTap){
                // TAP 이었으면 HOLD
                mouse_state== MouseState::kLeftHold;
            }
            mouse_pos.x = event.mouseButton.x; 
            mouse_pos.y = event.mouseButton.y;
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            if ( mouse_state == MouseState::kNone){
                // 아무것도 아니었으면 TAP
                mouse_state = MouseState::kRightTap;
            }
            else if ( mouse_state == MouseState::kRightTap){
                // TAP 이었으면 HOLD
                mouse_state== MouseState::kRightHold;
            }
            mouse_pos.x = event.mouseButton.x; 
            mouse_pos.y = event.mouseButton.y;
        }
    }
    // 마우스 버튼이 떼어졌을 때 감지
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left){
            mouse_state = MouseState::kLeftUp;
            mouse_pos.x = event.mouseButton.x; 
            mouse_pos.y = event.mouseButton.y;
        }
        else if ( event.mouseButton.button == sf::Mouse::Right){
            mouse_state = MouseState::kRightUp;
            mouse_pos.x = event.mouseButton.x; 
            mouse_pos.y = event.mouseButton.y;
        }
    }
    else{
        // 안눌림
        if ( mouse_state == MouseState::kLeftTap || mouse_state == MouseState::kLeftHold){
            mouse_state = MouseState::kLeftUp;
            mouse_pos.x = event.mouseButton.x; 
            mouse_pos.y = event.mouseButton.y;
        }
        else if ( mouse_state == MouseState::kRightTap || mouse_state == MouseState::kRightHold){
            mouse_state = MouseState::kRightUp;
            mouse_pos.x = event.mouseButton.x; 
            mouse_pos.y = event.mouseButton.y;
        }
        else{
            mouse_state = MouseState::kNone;
        }
    }
}
