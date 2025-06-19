#pragma once
#include "../struct.h"
#include "Object.h"

class Camera {
    private:
        uint32_t m_target;

    public:
        Camera(uint32_t _target) :m_target(_target){
        }
};